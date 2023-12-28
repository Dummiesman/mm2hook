local M = {}

M.info = {
  name = "Show Build Date",
  author = "Dummiesman",
  context = {'game'}
}

local enabled = false
local function onCull()
  if not enabled then return end 
  
  local physmgr = dgPhysManager.Instance
  local level = lvlLevel.Singleton
  
  vglUnbindTexture()
  
  gfxDrawFont(8, 8, "Midtown Madness 2")
  gfxDrawFont(8, 16, "Angel: 3393 / Nov  3 2000 14:34:22")
  
  gfxDrawFont(8, 32, "MM2Hook")
  gfxDrawFont(8, 40, MM2Hook.BuildDate .. " " .. MM2Hook.BuildTime)
end

local function drawMenuBar()
  if imgui.BeginMenu("Debug") then
     if imgui.MenuItem("Show Build Date", nil, enabled) then
        enabled = not enabled
     end
    imgui.EndMenu()
  end
end

M.drawMenuBar = drawMenuBar
M.onCull = onCull
return M