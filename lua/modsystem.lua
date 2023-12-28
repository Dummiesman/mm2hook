local M = {}

--exposed vars
M.useCache = false

--local vars
local allowMp = true
local modsPath = "lua/scripts"

--mods table
local mods = {}
local hookCache = {}

--helpers
local function callSafe(func, ...)
  local ok, err = pcall(func, ...)
  if not ok then
    Errorf(err)
    return false
  end
  return true
end

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

local function patchFunctionTable(fnTable, fnName, fnNewName)
  local fn = fnTable[fnName]
  if type(fn) == 'function' then
    local fnNew = fnTable[fnNewName]
    if type(fnNew) == 'function' then
      fnTable[fnNewName] = function() fnNew() fn() end
    else
      fnTable[fnNewName] = fn
    end
    fnTable[fnName] = nil
    Warningf('FIXME: Patching obsolete function "' .. fnName .. '" to "' .. fnNewName .. '". Please update your code as it uses deprecated API.')
  end
end

local function fileExists(name)
  local f=io.open(name,"r")
  if f~=nil then io.close(f) return true else return false end
end

local function findMod(mod)
  if type(mod) == 'table' then
    return mods[mod._modInternalName]
  elseif type(mod) == 'string' then
    return mods[mod]
  end
  return nil
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
  end
  
  if MMSTATE.NextState == STATE_GAME or Game ~= nil then
    table.insert(t, 'game')
    isGame = true
  elseif MMSTATE.NextState == STATE_MENU or Game == nil then
    table.insert(t, 'menu')
  end
  
  --if NETMGR.InSession then
  --  table.insert(t, 'multiplayer')
  --else
  --  table.insert(t, 'singleplayer')
  --end
  
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
    elseif MMSTATE.GameMode == GAMEMODE_CNR then
      table.insert(t, 'cnr')
    elseif MMSTATE.GameMode == GAMEMODE_CRASHCOURSE then
      table.insert(t, 'crash')
      table.insert(t, 'crashcourse')
    end
  end

  return t
end


local function processMod(modName, loadedMod)
    modName = modName:lower()
    if type(loadedMod) ~= "table" then
        Errorf("  invalid return value.")
        return false
    elseif mods[modName] then
        Errorf("  a mod named '" .. modName .. "' is already loaded.")
        return false
    else
        if type(loadedMod.info) ~= 'table' then
          Errorf("  invalid or missing info table, this mod will not be loaded.")
          return false
        end
        
        -- check if we should be loading this mod
        local context = loadedMod.info.context
        if not context or type(context) ~= 'table' then
          Errorf("  no context information. this mod will not be loaded.")
          return false
        end
        
        if not checkContext(context, getContexts()) then
          Displayf("  not specified to run in this context")
          return false
        end
        
        -- fix deprecated things
        patchFunctionTable(loadedMod, "onGameInit", "onStateBegin")
        patchFunctionTable(loadedMod, "onGamePostInit", "onStateBegin")
        patchFunctionTable(loadedMod, "onGamePreInit", "onStartup")
        patchFunctionTable(loadedMod, "onGameEnd", "onStateEnd")
        patchFunctionTable(loadedMod, "tick", "onUpdate")
        
        -- add our fields into the table
        loadedMod._modInternalName = modName
        loadedMod._modSubmodules = {}
        
        -- temporarily add mod to the mods table to allow for submodule registration
        mods[modName] = loadedMod
        
        -- call onModLoaded
        local loadResult = true
        if type(loadedMod.onModLoaded) == 'function' then
          local ok, loadResult = pcall(loadedMod.onModLoaded)
          if not ok then
            Errorf("  onModLoaded failed, mod will not be loaded")
            Errorf(loadResult)
            mods[modName] = nil
            return false
          end
          if type(loadResult) ~= "boolean" then loadResult = true end
        end
        
        -- remove if onModLoaded returned false
        if loadResult then
          -- load any persistent data
          print("[SERIALIZATION] Attempt to deserialize " .. loadedMod._modInternalName)
          if PersistentDataStore.Contains(loadedMod._modInternalName) and type(loadedMod.deserialize) == 'function' then
            print(" ... calling func")
            local data = PersistentDataStore.Retrieve(loadedMod._modInternalName)
            loadedMod.deserialize(json.decode(data))
          else
            print(" ... NOT found or no func")
          end
          PersistentDataStore.Delete(loadedMod._modInternalName)
          
          -- cache hooks
          for k,v in pairs(loadedMod) do
            if type(v) == 'function' then
              local cache = hookCache[k] or {}
              table.insert(cache, v)
              hookCache[k] = cache
            end
          end
          
          Displayf("  success")
          return true
        else
          Displayf("  skipped as requested by mod")
          mods[modName] = nil
          return false
        end
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

    -- process
    processMod(rootPath:sub(#modsPath+2), mod)
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

    -- process
    local modPathRelative = path:sub(9, #path-4)
    modPathRelative = modPathRelative:gsub('/','_')
    processMod(modPathRelative, mod())
end

local function loadModsFromArchives()
    local loadCityScripts = checkContext({"game"}, getContexts())
    local cityScriptPath = loadCityScripts and "city/" .. MMSTATE.CityName .. "/" or nil
    
    -- load scripts
    for file, isDir in datAssetManager.EnumFiles("scripts", false) do
      local slashCount = charCount(file, "/")
      local isLuaFile = endsWith(file:lower(), ".lua")
      
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

--api
local function getMod(name)
  local ret = findMod(name)
  if ret == nil then
    Errorf("modsystem.getmod: Cannot find " .. name)
    Errorf("Loaded mods:")
    for k,v in pairs(mods) do
      Errorf(k)
    end
  end
  return ret
end

local function registerSubmodule(parentMod, submodule, subpath)
  local parentModTable = findMod(parentMod)
  if parentModTable == nil then
    Errorf("registerSubmodule: invalid parentMod.")
  end
  
  if type(submodule) ~= 'table' then
    Errorf("registerSubmodule: invalid submodule. Must be mod table.")
    return
  end
  
  -- things look in check, lets register this as a submodule
  local submodulePath = parentModTable._modInternalName .. "_" .. subpath:lower()
  Displayf("  registering submodule " .. submodulePath)
  
  if not parentModTable then
    Errorf("registerSubmodule: could not find parent module " .. parentModTable._modInternalName .. ".")
    return
  end
  
  if processMod(submodulePath, submodule) then
    table.insert(parentModTable._modSubmodules, submodulePath)
  end
end

local function unload(mod, unloadSubmodules)
  local modTable = findMod(mod)
  if modTable == nil then
    Errorf("unload: invalid mod")
    return
  end
  
  -- unload submodules
  if unloadSubmodules == nil then unloadSubmodules = true end
  if unloadSubmodules and #modTable._modSubmodules ~= 0 then
    for _,submoduleName in pairs(modTable._modSubmodules) do
      unload(submoduleName)
    end
  end
  
  -- unload module
  if type(modTable.onModUnloaded) == 'function' then callSafe(modTable.onModUnloaded) end
  mods[modTable._modInternalName] = nil
  
  -- serialize persistent data
  if type(modTable.serialize) == 'function' then
    local serializedData = modTable.serialize()
    PersistentDataStore.Store(modTable._modInternalName, json.encode(serializedData))
  end
  
  -- clean hooks
  for k,v in pairs(modTable) do
    if type(v) == 'function' then
      local cache = hookCache[k]
      if cache then
        for i=#cache,1,-1 do
           if cache[i] == v then table.remove(cache, i) end
        end
      end
    end
  end
end

local function reload(mod)
  -- TODO
  local modTable = findMod(mod)
  if modTable == nil then
    Errorf("reload: invalid key or mod not loaded")
    return
  end
  
end

------------------------
------ MAIN HOOKS ------
------------------------
local function callHook(name, ...)
    local cache = hookCache[name]
    if not cache then return end
    
    for _, func in pairs(cache) do
        callSafe(func, ...)
    end
end

local function onRenderUi()
    --Render the main bar
    if imgui.BeginMainMenuBar() then
      callHook("drawMenuBar")
      imgui.EndMainMenuBar()
    end

    --Render mod windows etc
    callHook("onRenderUi")
end

local function onStartup()
  callHook("onStartup")
end

local function onShutdown()
  callHook("onShutdown")
  
  -- unload all mods which will also handle serialization
  for _,mod in pairs(mods) do
    unload(mod, false)
  end
end

--exports
M.init = init

M.registerSubmodule = registerSubmodule
M.getMod = getMod
M.unload = unload
M.reload = reload

M.callHook = callHook
M.onRenderUi = onRenderUi
M.onStartup = onStartup
M.onShutdown = onShutdown

return M