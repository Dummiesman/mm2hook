local M = {}

M.info = {
  name = "Level Debug",
  author = "Dummiesman",
  context = {'game'}
}

local enabled = false
local labelInstances = false
local labelBangers = false
local labelVehicles = false
local labelLandmarks = false
local labelFacades = false

local function onCull()
  if not enabled then return end

  if labelInstances then
    local imask = (labelBangers and 2 or 0) | (labelVehicles and 8 or 0) | (labelLandmarks and 256 or 0) | (labelFacades and 1024 or 0)
    Level.InstanceLabelMask = imask
    
    rglPushMatrix()
    rglWorldIdentity()
    Level:LabelInstances(Player.Car.Instance.CurrentRoom)
    rglPopMatrix()
  end
end

local function drawMenuBar()
  if imgui.BeginMenu("Debug") then
     if imgui.BeginMenu("Level") then
       if imgui.MenuItem("Enable", nil, enabled) then
          enabled = not enabled
       end
       imgui.Separator()
       
       if imgui.MenuItem("Label Instances", nil, labelInstances) then
          labelInstances = not labelInstances
       end
       if imgui.MenuItem("Label Bangers", nil, labelBangers) then
          labelBangers = not labelBangers
       end
       if imgui.MenuItem("Label Vehicles", nil, labelVehicles) then
          labelVehicles = not labelVehicles
       end
       if imgui.MenuItem("Label Landmarks", nil, labelLandmarks) then
          labelLandmarks = not labelLandmarks
       end
       if imgui.MenuItem("Label Facades", nil, labelFacades) then
          labelFacades = not labelFacades
       end
       imgui.EndMenu()
     end
    imgui.EndMenu()
  end
end

M.drawMenuBar = drawMenuBar
M.onCull = onCull

return M