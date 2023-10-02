local M = {}

M.info = {
  name = "Core Mods : Camera Submodule",
  author = "mm2hook Team",
  context = {"game"}
}

local enableLookAround = false
local lastPan = 0

local function onModLoaded()
  
end

local function what()
  local camView = Player.CamView
  local cams = {{"FreeCam", Player.FreeCam}, {"FarCam", Player.FarCam}, {"NearCam", Player.NearCam}, {"IndCam", Player.IndCam}, {"DashCam", Player.DashCam}, {"PovCam", Player.PovCam}, {"PointCam", Player.PointCam}, {"PreCam", Player.PreCam}, {"PostCam", Player.PostCam}, {"ThrillCam", Player.ThrillCam}, {"XCam", Player.XCam}, {"MPPostCam", Player.MPPostCam}}
  
  for _, ctbl in pairs(cams) do
    local name = ctbl[1]
    local value = ctbl[2]
    
    if camView:IsCurrentCamera(value) then  
      print("Current cam is: " .. name)
      break
    end
  end
end

local function onUpdate()
  --what()
  
  local pan = Input:GetCamPan()
  local cam1 = Player.FarCam
  local cam2 = Player.NearCam
  local camView = Player.CamView
  
  local activeCam = nil
  if camView:IsCurrentCamera(cam1) then
    activeCam = cam1
  elseif camView:IsCurrentCamera(cam2) then
    activeCam = cam2
  end
 
  if not activeCam then return end
  
  if pan ~= lastPan then
    if pan == 0.5 then
      -- look back
      activeCam.ReverseMode = -1
    else
      activeCam.ReverseMode = 1
    end
  end
  
  lastPan = pan
end

--exports
M.onModLoaded = onModLoaded
M.onUpdate = onUpdate

return M