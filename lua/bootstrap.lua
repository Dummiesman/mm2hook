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

-- output functions
Warningf = function(fmt, ...)  Warning(string.format(fmt, ...)) end
Errorf = function(fmt, ...)  Error(string.format(fmt, ...)) end
Displayf = function(fmt, ...)  Display(string.format(fmt, ...)) end
Abortf = function(fmt, ...)  Abort(string.format(fmt, ...)) end
Printf = function(fmt, ...)  Print(string.format(fmt, ...)) end
Messagef = function(fmt, ...) Message(string.format(fmt, ...)) end
print = function(x) Message(tostring(x)) end

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