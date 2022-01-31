local M = {}
local imgui = require("imgui")

--exposed vars
M.useCache = false

--local vars
local allowMp = true
local modsPath = "lua/mods"

--mods table
local mods = {}

--helpers
local function charCount(str, chr) 
  local c = 0
  for i = 1, #str do
      if str:sub(i,i) == chr then c = c + 1 end
  end
  return c
end

local function startsWith(str,start)
   return str:sub(1,string.len(start)):lower() == start:lower()
end

local function endsWith(str, ending)
   return str:sub(-#ending):lower() == ending:lower()
end

local function fileExists(name)
   local f=io.open(name,"r")
   if f~=nil then io.close(f) return true else return false end
end

--init!
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


local function processMod(loadedMod)
    if type(loadedMod) ~= "table" then
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

local function loadModFromDisk(rootPath)
    print("Loading mod: " .. rootPath)
    
    local mainPath = rootPath .. "/main.lua"
    if not fileExists(mainPath) then
      Warningf("Can't load mod at " .. rootPath .. " because main.lua doesn't exist.")
      return
    end
    
    local convertedPath = mainPath:gsub("%/", "\\"):sub(1,-5)
    if M.useCache == false then
        package.loaded[convertedPath] = nil
    end

    local ok, mod = pcall(require, convertedPath) 
    if not ok then 
      local err = mod
      Errorf("An error occurred loading mod at " .. rootPath .. ": " .. err)
      return
    end

    processMod(mod)
end

local function loadModsFromDisk()
    local attrib = lfs.attributes(modsPath)
    if attrib == nil or attrib.mode ~= "directory" then
        Errorf("modsystem: cannot load mods because mods isn't a directory??")
        return
    end
        
    for file in lfs.dir(modsPath) do
        if file ~= "." and file ~= ".." then
            local fullPath = modsPath..'/'..file
            local attrib = lfs.attributes(fullPath)

            if attrib and attrib.mode == 'directory' then
                loadModFromDisk(fullPath)
            end
        end
    end
end

local function loadModFromArchive(path)
    print("Loading script: " .. path)

    local file = Stream.Open(path, true)
    local text = file:ReadAll()
    file:Close()

    local ok, mod = pcall(load, text) 
    if not ok then 
      local err = mod
      Errorf("An error occurred loading script at " .. path .. ": " .. err)
      return
    end

    -- get return value from script and load
    mod = mod()
    processMod(mod)
end

local function loadModsFromArchives()
    local loadCityScripts = checkContext({"game"}, getContexts())
    local cityScriptPath = loadCityScripts and "city/" .. MMSTATE.CityName .. "/" or nil
    
    -- load scripts
    for file, isDir in datAssetManager.EnumFiles("scripts", false) do
      local slashCount = charCount(file, "/")
      local isLuaFile = endsWith(file, ".lua")
      
      if isLuaFile then
        if slashCount == 0 then
          -- this file is directly in the scripts subfolder
          loadModFromArchive("scripts/" .. file)
        elseif loadCityScripts and slashCount == 2 and startsWith(file, cityScriptPath) then
          -- city specific script
          loadModFromArchive("scripts/" .. file)
        end
      end
    end
end

local function createModsDirectory()
    local attrib = lfs.attributes(modsPath)
    if attrib == nil then
        lfs.mkdir("lua/mods")
    end
end

local function init()
    createModsDirectory()
    
    Warningf("modsystem.init: loading mods")
    loadModsFromDisk()
    loadModsFromArchives()
    Warningf("modsystem.init: loading mods complete")
end

------------------------
------ MAIN HOOKS ------
------------------------
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