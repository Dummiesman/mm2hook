local M = {}
require("dinput_keys")

local function onUpdate()
  if Game and not Game.Popup.IsEnabled then
    if ioKeyboard.GetKeyUp(DIK_L) then
      vehCarModel.ShowHeadlights = not vehCarModel.ShowHeadlights
    elseif ioKeyboard.GetKeyUp(DIK_COMMA) then
      vehCarModel.LeftSignalLightState = not vehCarModel.LeftSignalLightState
      vehCarModel.RightSignalLightState = false
      vehCarModel.HazardLightsState = false
    elseif ioKeyboard.GetKeyUp(DIK_PERIOD) then
      vehCarModel.RightSignalLightState = not vehCarModel.RightSignalLightState
      vehCarModel.LeftSignalLightState = false
      vehCarModel.HazardLightsState = false
    elseif ioKeyboard.GetKeyUp(DIK_SLASH) then
      vehCarModel.RightSignalLightState = false
      vehCarModel.LeftSignalLightState = false
      vehCarModel.HazardLightsState = not vehCarModel.HazardLightsState
    end
  end
end

--exports
M.onUpdate = onUpdate

return M