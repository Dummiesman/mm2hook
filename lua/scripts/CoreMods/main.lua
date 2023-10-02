local M = {}

M.info = {
  name = "Core Mods",
  author = "mm2hook Team",
  context = {"game", "menu"}
}

local function onModLoaded()
  modsystem.registerSubmodule(M, require("mods/CoreMods/freecam"), "freecam")
  modsystem.registerSubmodule(M, require("mods/CoreMods/cameraExtensions"), "cameraExtensions")
  modsystem.registerSubmodule(M, require("mods/CoreMods/lightExtensions"), "lightExtensions")
  modsystem.registerSubmodule(M, require("mods/CoreMods/sirenExtensions"), "sirenExtensions")
  modsystem.registerSubmodule(M, require("mods/CoreMods/richpresence"), "richpresence")
  modsystem.registerSubmodule(M, require("mods/CoreMods/cheats"), "cheats")
  
  if datArgParser.Get("lConsoleTest") then
    modsystem.registerSubmodule(M, require("mods/CoreMods/console"), "console")
  end
end

--exports
M.onModLoaded = onModLoaded

return M