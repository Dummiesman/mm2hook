local M = {}

M.info = {
  name = "Position Debug",
  author = "Dummiesman",
  context = {'game'}
}

local enabled = false

local function drawMenuBar()
  if imgui.BeginMenu("Debug") then
    if imgui.MenuItem("Show Position", nil, enabled) then
      enabled = not enabled
      HUD:TogglePositionDisplay(enabled)
    end
    imgui.EndMenu()
  end
end

M.drawMenuBar = drawMenuBar

return M