package.loaded.modsystem = nil
package.loaded.constants = nil
require("constants")
require("dinput_keys")
require("utils")
modsystem = require("modsystem")
imgui = require("imgui") -- abstraction of imgui for ease of use in lua
implot = require("implot")
imnodes = require("imnodes")
imguizmo = require("imguizmo")

local node = nil -- main node to get cull/update from
local addNodeToGame = false
local drawAllGUI = false -- if false, only draw persistent GUI

local lastCamRoom = 0
local lastPlayerRoom = 0

-- first init
if not gFirstInit then
  -- print to console
  print = function(x) Displayf(tostring(x)) end
  
  -- add searcher for zip files
  local vfsSearcher = function(libraryname) 
    local stream = Stream.Open(libraryname .. ".lua", true)
    if stream then
      local streamContent = stream:ReadAll()
      stream:Close()
      
      return load(streamContent)
    end
  end
  table.insert(package.searchers, 1, vfsSearcher)
  
  --
  gFirstInit = true
end

-- ui init
local function file_exists(name)
   local f=io.open(name,"r")
   if f~=nil then io.close(f) return true else return false end
end

function detectGlobalWrites()
  setmetatable(_G, {
    __newindex = function (t, key, val)
      rawset(_G, key, val)
      Warningf(debug.traceback('[Lua] set new global variable: "' .. tostring(key) .. '"  to "'  .. tostring(val) .. '"', 2, 1, false))
    end,
  })
end

local function initImgui()
  -- setup a style like in-game popups
  local style = imgui.GetStyle()

  style.FrameRounding = 0
  style.WindowRounding = 0
  style.ScrollbarRounding = 0
  style.WindowTitleAlign = ImVec2(0.5, 0.5)
  style.WindowBorderSize = 0
  style.FrameBorderSize = 1

  style:SetColor(ImGuiCol_WindowBg, ImVec4(0.06, 0.12, 0.36, 0.50))
  style:SetColor(ImGuiCol_Border, ImVec4(0.74, 0.74, 0.85, 0.50))
  style:SetColor(ImGuiCol_FrameBg, ImVec4(0.00, 0.00, 0.00, 0.33))
  style:SetColor(ImGuiCol_TitleBg, ImVec4(0.10, 0.10, 0.10, 1.00))
  style:SetColor(ImGuiCol_CheckMark, ImVec4(1.00, 1.00, 1.00, 1.00))
  style:SetColor(ImGuiCol_Button, ImVec4(0.26, 0.58, 0.98, 0.00))
  style:SetColor(ImGuiCol_ButtonHovered, ImVec4(0.60, 0.60, 0.60, 0.39))
  style:SetColor(ImGuiCol_ButtonActive, ImVec4(0.64, 0.64, 0.64, 1.00))

  local file = Stream.Open("fonts/Arimo-Bold.ttf", true)
  if file then
    local fileData = file:ReadAll()
    file:Close()
      
    local atlas = imgui.GetIO().Fonts
    UI_FONT_SMALL = atlas:AddFontFromMemoryTTF(fileData, fileData:len(), 14)
    UI_FONT_MEDIUM = atlas:AddFontFromMemoryTTF(fileData, fileData:len(), 26)
    UI_FONT_LARGE = atlas:AddFontFromMemoryTTF(fileData, fileData:len(), 38)
    atlas:Build()
  end
end

-- modsystem forwaraders
function onRenderUi()
  if UI_FONT_SMALL ~= nil then
    imgui.PushFont(UI_FONT_SMALL)
  end
  --imgui.ShowDemoWindow(true)
  modsystem.callHook("onRenderPersistentUi")
  if drawAllGUI then
    modsystem.onRenderUi()
  end
  if UI_FONT_SMALL ~= nil then
    imgui.PopFont()
  end
end

function onChatMessage(message)
  modsystem.callHook("onChatMessage", message)
end

function onSessionCreate()
  modsystem.callHook("onSessionCreate")
end

function onSessionJoin()
  modsystem.callHook("onSessionJoin")
end

function onDisconnect()
  modsystem.callHook("onDisconnect")
end

function onReset()
  modsystem.callHook("onReset")
end

function onRenderHudmap()
  modsystem.callHook("onRenderHudmap")
end

function onStateBegin()
  if addNodeToGame and Game then
    Game:AddChild(node)
  end
  modsystem.callHook("onStateBegin")
end

function onStateEnd()
  modsystem.callHook("onStateEnd")
end

function startup()
  GameState = MMSTATE.NextState
  modsystem.callHook("onStartup")
  initImgui()
end

function shutdown()
  modsystem.callHook("onShutdown")
end

local function onUpdate()
  if ioKeyboard.GetKeyDown(DIK_F10) then
    drawAllGUI = not drawAllGUI
    imgui.GetIO().MouseDrawCursor = drawAllGUI
  end
  
  if ROOT.Paused then
    modsystem.callHook("onUpdatePaused")
  else
    modsystem.callHook("onUpdate")
  end
  
  -- call room hooks
  if Player ~= nil then
    local level = lvlLevel.Singleton
    
    local camPos = Player.CamView.CurrentCamera:GetPosition()
    local camRoom = level:FindRoomId(camPos, lastCamRoom)
    if camRoom ~= lastCamRoom then
      modsystem.callHook("onCameraMovedRoom", lastCamRoom, camRoom)
      lastCamRoom = camRoom
    end
    
    local playerRoom = Player.Car.Instance.CurrentRoom
    if playerRoom ~= lastPlayerRoom then
      modsystem.callHook("onPlayerMovedRoom", lastPlayerRoom, playerRoom)
      lastPlayerRoom = playerRoom
    end
  end
end

local function onCull()
  modsystem.callHook("onCull")
end

function init()
  detectGlobalWrites()
  math.randomseed(os.time())
    
  -- create node
  node = luaNode("mm2hook")
  if Game ~= nil then
    Game:AddChild(node)
  elseif MMSTATE.NextState == STATE_GAME then
    -- Game hasn't been created yet
    -- delay AddChild until onGamePreInit
    addNodeToGame = true
  else
    ROOT:AddChild(node)
  end
  
  node.Update = onUpdate
  node.Cull = onCull
  node.AutoDeclareCullable = true
  
  --
  modsystem.init()
end


init()