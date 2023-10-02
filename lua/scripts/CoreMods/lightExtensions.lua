local M = {}

M.info = {
  name = "Core Mods : Lights Submodule",
  author = "mm2hook Team",
  context = {"game"}
}

local headlightsToggleKey = DIK_L
local hazardsToggleKey = DIK_SLASH
local leftSignalToggleKey = DIK_COMMA
local rightSignalToggleKey = DIK_PERIOD

local function onModLoaded()
  headlightsToggleKey = HookConfig.GetInt("HeadlightsToggleKey", headlightsToggleKey)
  hazardsToggleKey = HookConfig.GetInt("HazardLightsToggleKey", hazardsToggleKey)
  leftSignalToggleKey = HookConfig.GetInt("LeftTurnSignalToggleKey", leftSignalToggleKey)
  rightSignalToggleKey = HookConfig.GetInt("RightTurnSignalToggleKey", rightSignalToggleKey)
end

local function onUpdate()
  if Game.Popup.IsEnabled then return end
  
  if ioKeyboard.GetKeyUp(headlightsToggleKey) then
    vehCarModel.ShowHeadlights = not vehCarModel.ShowHeadlights
  elseif ioKeyboard.GetKeyUp(leftSignalToggleKey) then
    vehCarModel.LeftSignalLightState = not vehCarModel.LeftSignalLightState
    vehCarModel.RightSignalLightState = false
    vehCarModel.HazardLightsState = false
  elseif ioKeyboard.GetKeyUp(rightSignalToggleKey) then
    vehCarModel.RightSignalLightState = not vehCarModel.RightSignalLightState
    vehCarModel.LeftSignalLightState = false
    vehCarModel.HazardLightsState = false
  elseif ioKeyboard.GetKeyUp(hazardsToggleKey) then
    vehCarModel.RightSignalLightState = false
    vehCarModel.LeftSignalLightState = false
    vehCarModel.HazardLightsState = not vehCarModel.HazardLightsState
  end
end

--exports
M.onModLoaded = onModLoaded
M.onUpdate = onUpdate

return M