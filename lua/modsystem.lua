local M = {}
local imgui = require("imgui")

--exposed vars
M.useCache = false

--local vars
local modsPath = "lua/mods"

--mods table
local mods = {}

--init!
local function fileExists(name)
   local f=io.open(name,"r")
   if f~=nil then io.close(f) return true else return false end
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
    local loadedMod = require(convertedPath)
    if loadedMod == nil then
        Errorf("Failed to load mod: " .. convertedPath .. ".")
    elseif type(loadedMod) ~= "table" then
        Errorf("Failed to load mod: " .. convertedPath .. ", got invalid return value.")
    else
        -- fix deprecated things
        if loadedMod.onGameInit and type(loadedMod.onGameInit) == 'function' then
          Warningf("onGameInit is deprecated, now moved to onGamePreInit and onGamePostInit. Automatically patching to onGamePostInit.")
          loadedMod.onGamePostInit = loadedMod.onGameInit
          loadedMod.onGameInit = nil
        end
    
        table.insert(mods, loadedMod)
        Displayf("Loaded mod " .. path .. ".")
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

local function restart()
    for _, mod in ipairs(mods) do
        if mod.restart then mod.restart() end
    end
end

local function shutdown()
    for _, mod in ipairs(mods) do
        if mod.shutdown then mod.shutdown() end
    end
end

--exports
M.init = init

M.initMods = initMods
M.onChatMessage = onChatMessage
M.onRenderUi = onRenderUi
M.tick = tick
M.onGamePreInit = onGamePreInit
M.onGamePostInit = onGamePostInit
M.onGameEnd = onGameEnd
M.onSessionCreate = onSessionCreate
M.onSessionJoin = onSessionJoin
M.onDisconnect = onDisconnect
M.onReset = onReset
M.restart = restart
M.shutdown = shutdown

return M