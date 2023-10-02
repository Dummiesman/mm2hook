local M = {}

M.info = {
  name = "Core Mods : Siren Submodule",
  author = "mm2hook Team",
  context = {"game"}
}

local lightsToggleKey = DIK_K
local soundsToggleKey = DIK_J

local sirenSoundState = false -- TODO: Use actual playing state

local function onModLoaded()
  lightsToggleKey = HookConfig.GetInt("SirenLightsToggleKey", lightsToggleKey)
  soundsToggleKey = HookConfig.GetInt("SirenSoundsToggleKey", soundsToggleKey)
end

local function onUpdate()
  if Game.Popup.IsEnabled then return end
  
  if ioKeyboard.GetKeyUp(lightsToggleKey) then
    local siren = Player.Car.Siren
    siren.Active = not siren.Active
  elseif ioKeyboard.GetKeyUp(soundsToggleKey) then
    local audio = Player.Car.Audio:GetActiveAudio()
    if audio.___type == "class<vehPoliceCarAudio>" then
      sirenSoundState = not sirenSoundState
      if sirenSoundState then
        audio:StartSiren(0)
      else
        audio:StopSiren()
      end
    end
  end
end

--exports
M.onModLoaded = onModLoaded
M.onUpdate = onUpdate

return M