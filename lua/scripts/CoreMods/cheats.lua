local M = {}

M.info = {
  name = "Core Mods : Cheat Submodule",
  author = "mm2hook Team",
  context = {"game"}
}

-- fly
local playerCanFly = false

-- nodamage
local restoreImpactThreshold = nil

-- bridge
local bridgeCheatStatus = false
local restoreBridgeSpeed = 0.05
local restoreBridgeAngle = math.rad(27.0)

-- puck/slide
local restoreWeatherFriction = 1.0
local restoreEggFriction = 1.0

-- launch prop
local launchProp = "sp_mailbox_f"
local launchPropSpeed = 25.0
local launchPropBangerId = -1
local launchPropGeomId = -1
local lastHornState = false

local function onModLoaded()
  -- get launch prop from config
  launchProp = HookConfig.GetString("LaunchProp", launchProp)
  launchPropSpeed = HookConfig.GetFloat("LaunchPropSpeed", launchPropSpeed)
  launchProp = "sp_tree1_s"
end

local function initLaunchProp()
  -- check if already initialized
  if launchPropGeomId >= 0 or launchPropBangerId >= 0 then return end
  
  -- create a temp banger to ensure the banger data 
  -- and geometry is loaded
  local tempBanger = dgUnhitBangerInstance.RequestBanger(launchProp, true)
  tempBanger:Init(launchProp, Matrix34(), 0)
  lvlLevel.Singleton:MoveToRoom(tempBanger, 0)
  
  --
  launchPropGeomId = tempBanger.GeomIndex
  launchPropBangerId = tempBanger.BangerType
end

local function onChatMessage(message)
  if message == "/grav" then
    dgPhysManager.Gravity = (dgPhysManager.Gravity < -10) and -9.8 or -19.6
  elseif message == "/fuzz" then
    HUD.Map.ShowAllCops = not HUD.Map.ShowAllCops
  elseif message == "/postal" then
    initLaunchProp()
  elseif message == "/nodamage" then
    Player.Car.CarDamage.ImpactThreshold = math.huge
  elseif message == "/damage" then
    Player.Car.CarDamage.ImpactThreshold = restoreImpactThreshold
  elseif message == "/fly" then
    playerCanFly = not playerCanFly
  elseif message == "/puck" then
    if vehWheel.WeatherFriction == 0.0 then 
      vehWheel.WeatherFriction = restoreWeatherFriction 
    else
      vehWheel.WeatherFriction = 0.0 
    end
  elseif message == "/bridge" then
    bridgeCheatStatus = not bridgeCheatStatus
    if bridgeCheatStatus then
      gizBridge.GoalAngle = 1.0
      gizBridge.LiftSpeed = 2.5
    else
      gizBridge.GoalAngle = restoreBridgeAngle
      gizBridge.LiftSpeed = restoreBridgeSpeed
    end
  end
end

local function launchAProp()
    local carInst = Player.Car:GetInst()
    local carIcs = Player.Car:GetICS()
    
    local matrix = Matrix34()
    matrix:Identity()
    
    local camMatrix = gfxRenderState.CameraMatrix:ToMatrix34()
    local camPos = camMatrix:GetRow(3)
    local camFwd = camMatrix:GetRow(2)
    matrix:SetRow(3, camPos)

    local banger = dgBangerManager.Instance:GetBanger() -- Get a dgHitBangerInstance
    banger.GeomIndex = launchPropGeomId
    banger.BangerType = launchPropBangerId
    banger:SetMatrix(matrix)
    banger:SetVariant(0)
    lvlLevel.Singleton:Reparent(banger)
    
    local active = banger:AttachEntity() -- This asks dgBangerActiveManager to return us a dgBangerActive. The physics representation.
    if active then
      local carVel = carInst:GetVelocity()
      local bangerIcs = active:GetICS()
      bangerIcs.Velocity = carVel + (camFwd * -1.0 * launchPropSpeed)
    end
end

local function onStateBegin()
  restoreImpactThreshold = Player.Car.CarDamage.ImpactThreshold
  restoreWeatherFriction = vehWheel.WeatherFriction
  restoreBridgeSpeed = gizBridge.LiftSpeed
  restoreBridgeAngle = gizBridge.GoalAngle
end

local function onUpdate()
  -- fly
  if playerCanFly then
    if Player.Car.CarSim.Engine.Throttle > 0 and Player.Car.CarSim.Speed < 112 and not Player:IsMaxDamaged() then
      local ics = Player.Car:GetICS()
      ics.Velocity = ics.Velocity * 1.03
    end
  end
  
  -- launch prop
  local hornState = Player.Car.Audio.HornPlaying
  if lastHornState ~= hornState and hornState and launchPropBangerId >= 0 and launchPropGeomId >= 0 then
    launchAProp()
  end
  lastHornState = hornState
end

--exports
M.onModLoaded = onModLoaded
M.onChatMessage = onChatMessage
M.onStateBegin = onStateBegin
M.onUpdate = onUpdate

return M