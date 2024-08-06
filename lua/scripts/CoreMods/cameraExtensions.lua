local M = {}

M.info = {
  name = "Camera Extensions",
  author = "mm2hook Team",
  context = {'game'}
}

local invertedNear = nil
local invertedFar = nil

local function loadInvertedCam(name)
  local cam = camTrackCS()
  cam:Init(Player.Car, string.format("%s_%s", Player.Car.CarDamage.Name, name))
  cam.Target = Player.Car.CarSim:GetWorldMatrixPtr()
  cam.ReverseMode = -1
  cam.ApproachOn = false
  return cam
end

local function activateLookBehind()
  if Player.CamView:IsCurrentCamera(Player.NearCam) then
    Player.CamView:SetCam(invertedNear)
  elseif Player.CamView:IsCurrentCamera(Player.FarCam) then
    Player.CamView:SetCam(invertedFar)
  end
end

local function deactivateLookBehind()
  if Player.CamView:IsCurrentCamera(invertedNear) then
    Player.CamView:SetCam(Player.NearCam)
  elseif Player.CamView:IsCurrentCamera(invertedFar) then
    Player.CamView:SetCam(Player.FarCam)
  end
end

local function onStateBegin()
  -- have to save/load the CameraFar as it's overwritten from the camera file
  local prevCameraFar = camBaseCS.CameraFar
  invertedNear = loadInvertedCam("near")
  invertedFar = loadInvertedCam("far")
  camBaseCS.CameraFar = prevCameraFar
end

local function onUpdate()
  if not Player.CamView.IsTransitioning then
    if Input:GetCamPan() == 0.5 then
      activateLookBehind()
    else
      deactivateLookBehind()
    end
  end
end

M.onUpdate = onUpdate
M.onUpdatePaused = onUpdate
M.onStateBegin = onStateBegin
return M