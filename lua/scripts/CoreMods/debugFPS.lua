local M = {}

M.info = {
  name = "Show Build Date",
  author = "Dummiesman",
  context = {'game'}
}

local enabled = false
local function onCull()
  if not enabled then return end 

  vglUnbindTexture()
  gfxDrawFont(8, 8, string.format("%.0f FPS", datTimeManager.FPS))
end

local function drawMenuBar()
  if imgui.BeginMenu("Debug") then
     if imgui.MenuItem("Show FPS", nil, enabled) then
        enabled = not enabled
     end
    imgui.EndMenu()
  end
end

M.drawMenuBar = drawMenuBar
M.onCull = onCull
return M