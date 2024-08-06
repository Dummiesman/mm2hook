local M = {}

M.info = {
  name = "Core Mods : Console Submodule",
  author = "mm2hook Team",
  context = {"menu", "game"},
  serializationMode = "file"
}

-- window vars
local winOpen = false
local setInputFocus = false
local delayOpenCheck = 0 -- frames to delay the key check in onUpdate, an ugly hack to prevent ~ from closing then immediately reopening the console

-- auto scroll vars
local autoScroll = true
local autoScrollLastMessageCount = -1
local autoScrollQueued = true

-- output vars
-- The last prefix/color isn't ever used by AGE, only by us
local prefixes = {"", "", "", "[WARN]", "[ERROR]", "[FATAL]", "> "}
local colors = {ImVec4(1.0, 1.0, 1.0, 1.0), ImVec4(1.0, 1.0, 1.0, 1.0), ImVec4(1.0, 1.0, 1.0, 1.0), 
                ImVec4(1.0, 0.9, 0.0, 1.0), ImVec4(1.0, 0.1, 0.1, 1.0), ImVec4(1.0, 0.1, 0.1, 1.0),
                ImVec4(0.75, 0.75, 0.75, 1.0)}

local messages = {}
local messageCapacity = 256
local messagesTotal = 0 -- a running total of the # of messages

local commandHistory = {}
local commandHistoryPos = nil
local commandHistoryCapacity = 32

local buffer = CharBox(4096) -- input buffer

local function trimMessages()
  while #messages >= messageCapacity do
    -- shift all the messages down 
    table.remove(messages, 1)
  end
end

local function trimCommandHistory()
  while #commandHistory >= commandHistoryCapacity do
    -- shift all the history down 
    table.remove(commandHistory, 1)
  end
end

local function insertDebugMessage(level, message)
  table.insert(messages, {level=level, message=message})
  messagesTotal = messagesTotal + 1
  trimMessages()
end

local function insertCommandMessage(command)
  table.insert(messages, {level=6, message=command})
  messagesTotal = messagesTotal + 1
  trimMessages()
end

local function historyPrev()
  if #commandHistory == 0 then return nil end
  if not commandHistoryPos then
    commandHistoryPos = #commandHistory
  else
    commandHistoryPos = commandHistoryPos - 1
    if commandHistoryPos <= 0 then
      commandHistoryPos = #commandHistory
    end
  end
  return commandHistory[commandHistoryPos]
end

local function historyNext()
  if #commandHistory == 0 then return nil end
  if not commandHistoryPos then
    commandHistoryPos = 1
  else
    commandHistoryPos = commandHistoryPos + 1
    if commandHistoryPos > #commandHistory then
      commandHistoryPos = 1
    end
  end
  return commandHistory[commandHistoryPos]
end

local function historyReset()
  commandHistoryPos = nil
end

local function executeCommand(cmd)
  -- insert into command history (if we didn't execute the last command over again)
  -- log command to console, and reset history position
  if #commandHistory == 0 or commandHistory[#commandHistory] ~= cmd then
    table.insert(commandHistory, cmd)
  end
  insertCommandMessage(cmd)
  trimCommandHistory()
  historyReset()
  
  -- execute command and display any errors that occur
  local tmpFn, err = load(cmd)
  if tmpFn then
    xpcall(tmpFn, Error)
  else
    Error(err)
  end
end

local function inputCb(data)
  if data.EventFlag == ImGuiInputTextFlags_CallbackHistory then
    local newValue
    if data.EventKey == ImGuiKey_UpArrow then
      newValue = historyPrev()
    elseif data.EventKey == ImGuiKey_DownArrow then
      newValue = historyNext()
    end
    
    if newValue then
      local inplen = string.len(newValue)
      
      data:DeleteChars(0, data.BufTextLen)
      data:InsertChars(0, newValue)
                      
      data.CursorPos = inplen
      data.SelectionStart = inplen
      data.SelectionEnd = inplen
      data.BufTextLen = inplen
      data.BufDirty = true
    end
  elseif data.EventFlag == ImGuiInputTextFlags_CallbackCharFilter then
    if data.EventChar == 96 then -- grave
      winOpen = false
      delayOpenCheck = 3
      return 1
    end
  end
  return 0
end

local function onRenderPersistentUi()
  if winOpen then
    --begindraw
    local drawWindow = imgui.Begin("Console##mm2hook_console")
    if drawWindow then
      -- draw header
      imgui.Text("Settings: ")
      imgui.SameLine()
      autoScroll = imgui.Checkbox("Auto Scroll", autoScroll)
      
      -- draw messages
      local region = imgui.GetContentRegionAvail()
      if imgui.BeginChild("##console_messages", ImVec2(region.x - 4, region.y - 30), true) then
        for _,item in ipairs(messages) do
          local formatted = string.format("%s %s", prefixes[item.level+1], item.message)
          imgui.TextColored(colors[item.level+1], formatted)
          imgui.Tooltip(formatted)
        end
        
        -- auto scroll handling
        local shouldAutoScroll = autoScrollLastMessageCount ~= messagesTotal
        if autoScroll and shouldAutoScroll then
          autoScrollQueued = true
        elseif autoScrollQueued then
          imgui.SetScrollY(imgui.GetScrollMaxY())
          autoScrollQueued = false
        end
        autoScrollLastMessageCount = messagesTotal
      
        imgui.EndChild()
      end
      
      -- draw bottom bar
      imgui.PushItemWidth(region.x - 69)
      if setInputFocus then 
        imgui.SetKeyboardFocusHere() 
        setInputFocus = false
      end
      local inputFlags = ImGuiInputTextFlags_EnterReturnsTrue | ImGuiInputTextFlags_EscapeClearsAll | ImGuiInputTextFlags_CallbackHistory | ImGuiInputTextFlags_CallbackCharFilter
      if imgui.InputTextBoxedCallback("", buffer, inputFlags, inputCb) and #buffer.Value > 0 then
        imgui.SetKeyboardFocusHere(-1)
        executeCommand(buffer.Value)
        buffer.Value = ""
      end
      imgui.PopItemWidth()
      imgui.SameLine()
      if imgui.Button("Execute") then
        executeCommand(buffer.Value)
        buffer.Value = ""
      end
      
      --
      imgui.End()
    end
  end
end

local function onUpdate()
  if delayOpenCheck <= 0 and ioKeyboard.GetKeyDown(DIK_GRAVE) then
    winOpen = not winOpen
    setInputFocus = winOpen
  end
  delayOpenCheck = delayOpenCheck - 1
end

local function serialize()
    return {history = commandHistory}
end

local function deserialize(data)
    if type(data.history) == 'table' then
        commandHistory = {}
        for _,v in pairs(data.history) do
            table.insert(commandHistory, v)
        end
        trimCommandHistory()
    end
end

--exports
M.onUpdate = onUpdate
M.onUpdatePaused = onUpdate
M.onRenderPersistentUi = onRenderPersistentUi
M.onDebugMessage = insertDebugMessage
M.serialize = serialize
M.deserialize = deserialize

return M