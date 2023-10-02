local M = {}

M.info = {
  name = "Core Mods : Console Submodule",
  author = "mm2hook Team",
  context = {"menu", "game"}
}

local winOpen = false
local history = {}

local function onRenderUi()
  if winOpen then
    local window_flags = 0 -- ImGuiWindowFlags_AlwaysAutoResize
    
    --begindraw
    winOpen = imgui.Begin("Console", winOpen, window_flags)
  
    
    
    imgui.End()
  end
end

local function onUpdate()
  if ioKeyboard.GetKeyDown(DIK_GRAVE) then
    winOpen = not winOpen
  end
end

--exports
M.onUpdate = onUpdate
M.onRenderUi = onRenderUi

return M