local M = {}
local imgui = require("imgui")

--exposed vars
M.useCache = false

--local vars
local allowMp = true
local modsPath = "lua/mods"

--mods table
local mods = {}

--init!
local function fileExists(name)
   local f=io.open(name,"r")
   if f~=nil then io.close(f) return true else return false end
end

local function prequire(m) 
  local ok, err = pcall(require, m) 
  if not ok then return nil, err end
  return err
end

local function checkContext(ctx, ctxs)
  for _,ctx1 in pairs(ctx) do
    for __,ctx2 in pairs(ctxs) do
      if ctx2 == ctx1 then return true end
    end
  end
  return false
end

local function getContexts()
  local t = {}
  local isGame = false
  
  if NETMGR.InSession and not allowMp then
    return t
  elseif MMSTATE.NextState == 1 or Game ~= nil then
    table.insert(t, 'game')
    isGame = true
  elseif MMSTATE.NextState == 0 or Game == nil then
    table.insert(t, 'menu')
  end
  
  if isGame then
    if MMSTATE.GameMode == GAMEMODE_BLITZ then 
      table.insert(t, 'blitz')
    elseif MMSTATE.GameMode == GAMEMODE_CHECKPOINT then
      table.insert(t, 'race')
      table.insert(t, 'checkpoint')
    elseif MMSTATE.GameMode == GAMEMODE_CIRCUIT then
      table.insert(t, 'circuit')
    elseif MMSTATE.GameMode == GAMEMODE_CRUISE then
      table.insert(t, 'roam')
      table.insert(t, 'cruise')
    elseif MMSTATE.GameMode == GAMEMODE_CRASHCOURSE then
      table.insert(t, 'crash')
      table.insert(t, 'crashcourse')
    end
  end

  return t
end


local function loadMod(path)
    local mainPath = path .. "/main.lua"
    if not fileExists(mainPath) then
      Warningf("Can't load mod at " .. path .. " because main.lua doesn't exist.")
      return
    end
    
    local convertedPath = mainPath:gsub("%/", "\\"):sub(1,-5)
    if M.useCache == false then
        package.loaded[convertedPath] = nil
    end

    Displayf("Loading mod " .. path)
    local loadedMod, err = prequire(convertedPath)
    if loadedMod == nil then
        Errorf("  " .. err .. ".")
    elseif type(loadedMod) ~= "table" then
        Errorf("  invalid return value.")
    else
        if not loadedMod.info or type(loadedMod.info) ~= 'table' then
          Errorf("  invalid or missing info table, this mod will not be loaded.")
          return
        end
        
        -- check if we should be loading this mod
        -- todo: always load CoreMods
        local context = loadedMod.info.context
        if not context or type(context) ~= 'table' then
          Errorf("  no context information. this mod will not be loaded.")
          return
        end
        
        if not checkContext(context, getContexts()) then
          Displayf("  not specified to run in this context")
          return
        end
        
        -- fix deprecated things
        if loadedMod.onGameInit and type(loadedMod.onGameInit) == 'function' then
          Warningf("  onGameInit is deprecated, now moved to onGamePreInit and onGamePostInit. Automatically patching to onGamePostInit.")
          loadedMod.onGamePostInit = loadedMod.onGameInit
          loadedMod.onGameInit = nil
        end
        if loadedMod.tick and type(loadedMod.tick) == 'function' then
          Warningf("  tick is deprecated, now moved to onUpdate. Automatically patching to onUpdate.")
          loadedMod.onUpdate = loadedMod.tick
          loadedMod.tick = nil
        end
        
        -- call init
        if loadedMod.onInit and type(loadedMod.onInit) == 'function' then
          loadedMod.onInit()
        end
        
        -- insert
        table.insert(mods, loadedMod)
        Displayf("  success")
    end
end

local function ends_with(str, ending)
   return ending == "" or str:sub(-#ending) == ending
end

local function loadMods(path)
    for file in lfs.dir(path) do
        if file ~= "." and file ~= ".." then
            local fullPath = path..'/'..file
            local attrib = lfs.attributes(fullPath)

            if attrib and attrib.mode == 'directory' then
                loadMod(fullPath)
            end
        end
    end
end

local function init()
    local attrib = lfs.attributes(modsPath)
    if attrib == nil then
        lfs.mkdir("lua/mods")
        attrib = lfs.attributes(modsPath)
    end

    if attrib == nil or attrib.mode ~= "directory" then
        Errorf("modsystem: cannot load mods because mods isn't a directory??")
    else
        Warningf("modsystem.init: loading mods")
        loadMods(modsPath)
        Warningf("modsystem.init: loading mods complete")
    end
end

--mods functions
local function tick()
    for _, mod in ipairs(mods) do
        if mod.tick then mod.tick() end
    end
end

local function initMods()
    for _, mod in ipairs(mods) do
        if mod.init then mod.init() end
    end
end

local function onChatMessage(message)
    for _, mod in ipairs(mods) do
        if mod.onChatMessage then mod.onChatMessage(message) end
    end
end

local function onGameEnd()
    for _, mod in ipairs(mods) do
        if mod.onGameEnd then mod.onGameEnd() end
    end
end

local function onGamePreInit()
    for _, mod in ipairs(mods) do
        if mod.onGamePreInit then mod.onGamePreInit() end
    end
end

local function onGamePostInit()
    for _, mod in ipairs(mods) do
        if mod.onGamePostInit then mod.onGamePostInit() end
    end
end

local function onSessionCreate(name, password, maxPlayers, details)
    for _, mod in ipairs(mods) do
        if mod.onSessionCreate then mod.onSessionCreate(name, password, maxPlayers, details) end
    end
end

local function onSessionJoin(a2, a3, a4)
    for _, mod in ipairs(mods) do
        if mod.onSessionJoin then mod.onSessionJoin(a2, a3, a4) end
    end
end

local function onDisconnect()
    for _, mod in ipairs(mods) do
        if mod.onDisconnect then mod.onDisconnect() end
    end
end

local function onReset()
    for _, mod in ipairs(mods) do
        if mod.onReset ~= nil then mod.onReset() end
    end
end

local function onUpdate()
    for _, mod in ipairs(mods) do
        if mod.onUpdate ~= nil then mod.onUpdate() end
    end
end

local function onUpdatePaused()
    for _, mod in ipairs(mods) do
        if mod.onUpdatePaused ~= nil then mod.onUpdatePaused() end
    end
end

local function onCull()
    for _, mod in ipairs(mods) do
        if mod.onCull ~= nil then mod.onCull() end
    end
end

local function onRenderUi()
    --Render the main bar
    if imgui.BeginMainMenuBar() then
      for _, mod in ipairs(mods) do
          if mod.drawMenuBar then mod.drawMenuBar() end
      end
      imgui.EndMainMenuBar()
    end

    --Render mod windows etc
    for _, mod in ipairs(mods) do
        if mod.onRenderUi then mod.onRenderUi() end
    end
end

local function onShutdown()
    for _, mod in ipairs(mods) do
        if mod.onShutdown then mod.onShutdown() end
    end
end

--exports
M.init = init

M.initMods = initMods
M.onChatMessage = onChatMessage
M.onRenderUi = onRenderUi
M.onCull = onCull
M.onUpdate = onUpdate
M.onUpdatePaused = onUpdatePaused
M.onGamePreInit = onGamePreInit
M.onGamePostInit = onGamePostInit
M.onGameEnd = onGameEnd
M.onSessionCreate = onSessionCreate
M.onSessionJoin = onSessionJoin
M.onDisconnect = onDisconnect
M.onReset = onReset
M.onShutdown = onShutdown

return M