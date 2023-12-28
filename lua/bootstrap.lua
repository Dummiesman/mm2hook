-- This file runs on each Lua engine (vehicle/game) upon load
package.loaded.constants = nil

require("constants")
require("dinput_keys")
require("utils")

imgui = require("imgui")
implot = require("implot")
imnodes = require("imnodes")
imguizmo = require("imguizmo")

json = require("json")

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