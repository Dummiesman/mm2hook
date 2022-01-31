package.loaded.modsystem = nil
package.loaded.constants = nil
require("constants")
local modsystem = require("modsystem")

local node = nil -- main node to get cull/update from

-- first init
if not gFirstInit then
  -- print to console
  print = function(x) Displayf(tostring(x)) end
  
  -- add searcher for zip files
  local vfsSearcher = function(libraryname) 
    local stream = Stream.Open(libraryname .. ".lua", true)
    if stream then
      local streamContent = stream:ReadAll()
      stream:Close()
      
      return load(streamContent)
    end
  end
  table.insert(package.searchers, 1, vfsSearcher)
  
  --
  gFirstInit = true
end

-- modsystem forwaraders
function onRenderUi()
  modsystem.onRenderUi()
end

function onChatMessage(message)
    modsystem.onChatMessage(message)
end

function onGameEnd()
    modsystem.onGameEnd()
end

function onGamePreInit()
    modsystem.onGamePreInit()
end

function onGamePostInit()
    modsystem.onGamePostInit()
end

function onSessionCreate(name, password, maxPlayers, details)
    modsystem.onSessionCreate(name, password, maxPlayers, details)
end

function onSessionJoin(a2, a3, a4)
    modsystem.onSessionJoin(a2, a3, a4)
end

function onDisconnect()
    modsystem.onDisconnect()
end

function onReset()
    modsystem.onReset()
end

function shutdown()
    modsystem.onShutdown()
end

local function onUpdate()
  if ROOT.IsPaused then
    modsystem.onUpdatePaused()
  else
    modsystem.onUpdate()
  end
end

local function onCull()
  modsystem.onCull()
end

function init()
    -- create node
    node = luaNode("mm2hook")
    ROOT:AddChild(node)
    
    node.Update = onUpdate
    node.Cull = onCull
    node.AutoDeclareCullable = true
    
    --
    local vms, massagems = pcall(modsystem.init)
    if not vms then Errorf(massagems) end

    --Init modsystem
    modsystem.initMods()
end

local M = {}
return M