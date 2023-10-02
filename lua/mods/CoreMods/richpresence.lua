local M = {}

M.info = {
  name = "Core Mods : Discord Rich Presence Submodule",
  author = "mm2hook Team",
  context = {"menu", "game"}
}

-- locals
local netUpdateTimer = 0
local lastPlayerCount = 0
local gamemodeNames = {"Cruise", "Checkpoint", "Cops N' Robbers", "Circuit", "Blitz", nil, "Crash Course"}

-- cities
local defaultCityImageKey = "nocitydesc"
local cityImageKeys = {
  -- Default cities
  "sf",
  "london"
}

-- cars
local defaultCarImageKey = "nocardesc"
local carImageKeys = {
  -- Default vehicles
  "vpcoop",
  "vpbug",
  "vpcab",
  "vpcaddie",
  "vpford",
  "vpmustang99",
  "vpcop",
  "vpbullet",
  "vppanoz",
  "vpbus",
  "vpddbus",
  "vpcentury",
  "vpcoop2k",
  "vpdune",
  "vpvwcup",
  "vp4x4",
  "vpauditt",
  "vpdb7",
  "vppanozgt",
  "vplafrance",
  
  -- Beta vehicles
  "vpcaddie59",
  "vpeagle",
  "vpmoonrover",
  "vpmtruck",
  
  -- Miscellaneous
  "vpredcar"
}

local function tableContains(t, value)
  for _,v in pairs(t) do
    if v == value then return true end
  end
  return false
end

local function isRaceMode()
  return MMSTATE.GameMode == GAMEMODE_CHECKPOINT or
         MMSTATE.GameMode == GAMEMODE_CIRCUIT or
         MMSTATE.GameMode == GAMEMODE_BLITZ
end

local function getGameModeName()
  return gamemodeNames[MMSTATE.GameMode + 1]
end

local function getRaceName()
  local raceName = "Unknown"
  if CityList ~= nil then
    local raceNames = CityList:GetCityInfo(MMSTATE.CityName):GetRaceNames(MMSTATE.GameMode)
    raceName = raceNames[MMSTATE.RaceId + 1] or raceName
  end
  return raceName
end

local function updatePresence()
  -- set some defaults
  DiscordRichPresence.StartTimestamp = 0
  DiscordRichPresence.EndTimestamp = 0
  DiscordRichPresence.LargeImage = ""
  DiscordRichPresence.SmallImage = ""
  DiscordRichPresence.Details = ""
  DiscordRichPresence.CurrentPartySize = 0
  DiscordRichPresence.MaxPartySize = 0
  DiscordRichPresence.SmallText = ""
  DiscordRichPresence.LargeText = ""
  
  -- set real values
  if GameState == STATE_GAME then
    DiscordRichPresence.LargeImage = tableContains(cityImageKeys, MMSTATE.CityName) and MMSTATE.CityName or defaultCityImageKey
    DiscordRichPresence.SmallImage = tableContains(carImageKeys, MMSTATE.VehicleName) and MMSTATE.VehicleName or defaultCarImageKey
    
    if NETMGR.InSession then
      DiscordRichPresence.State = isRaceMode() and "Multiplayer Race" or "Multiplayer Game"
    else
      DiscordRichPresence.State = isRaceMode() and "Singleplayer Race" or "Singleplayer Game"
    end
    
    if isRaceMode() or MMSTATE.GameMode == GAMEMODE_CRASHCOURSE then
      DiscordRichPresence.Details = string.format("%s: %s", getGameModeName(), getRaceName())
    else
      DiscordRichPresence.Details = getGameModeName()
    end    
    
    -- set image text
    if VehicleList ~= nil then
      local vehicleInfo = VehicleList:GetVehicleInfo(MMSTATE.VehicleName)
      DiscordRichPresence.SmallText = vehicleInfo.Description
    end
    
    if CityList ~= nil then
      local cityInfo = CityList:GetCityInfo(MMSTATE.CityName)
      DiscordRichPresence.LargeText = cityInfo.LocalizedName
    end
  else
    if NETMGR.InSession then
      DiscordRichPresence.State = "In Multiplayer"
      DiscordRichPresence.Details = "Lobby"
      DiscordRichPresence.LargeImage = "mpmenu"
    else
      DiscordRichPresence.State = "In Main Menu"
      DiscordRichPresence.LargeImage = "menu"
    end
  end
  
  if NETMGR.InSession then
    local sessionInfo = NETMGR:GetSessionInfo()
    DiscordRichPresence.CurrentPartySize = sessionInfo.NumPlayers
    DiscordRichPresence.MaxPartySize = sessionInfo.MaxPlayers
  end

  -- and finally, update!
  DiscordRichPresence.Update()
end

local function onUpdate()
  DiscordRichPresence.RunCallbacks()
  
  -- update player count every once and a while if in a session
  if NETMGR.InSession then
    netUpdateTimer = netUpdateTimer + datTimeManager.Seconds
    local timerTarget = (Game ~= nil) and 30 or 10
    
    if netUpdateTimer >= timerTarget then
      netUpdateTimer = 0
      updatePresence()
    end
  end
end

local function onModLoaded()
  -- check config setting
  if not HookConfig.Get("UseRichPresence") then return end
  
  -- initialize
  if not DiscordRichPresence.Initialized then
    DiscordRichPresence.Initialize(379767166267817984)
  end
  
  if DiscordRichPresence.Initialized then
    M.onUpdate = onUpdate 
    M.onStartup = updatePresence
    M.onStateBegin = updatePresence
    updatePresence()
  end
end

local function onModUnloaded()
  -- todo: shutdown?
end

local function nop()
end

M.onModLoaded = onModLoaded
M.onModUnloaded = onModUnloaded
M.onUpdate = nop
M.onStateBegin = nop
M.onStartup = nop

return M