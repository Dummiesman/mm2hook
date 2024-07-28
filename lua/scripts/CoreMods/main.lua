local M = {}

M.info = {
  name = "Core Mods",
  author = "mm2hook Team",
  context = {"game", "menu"}
}

local function onModLoaded()
  modsystem.registerSubmodule(M, require("scripts/CoreMods/debugAi"), "debugAi")
  modsystem.registerSubmodule(M, require("scripts/CoreMods/debugLevel"), "debugLevel")
  modsystem.registerSubmodule(M, require("scripts/CoreMods/debugPhysics"), "debugPhysics")
  modsystem.registerSubmodule(M, require("scripts/CoreMods/debugPosition"), "debugPosition")
  modsystem.registerSubmodule(M, require("scripts/CoreMods/debugBuildDate"), "debugBuildDate")
  modsystem.registerSubmodule(M, require("scripts/CoreMods/freecam"), "freecam")
  modsystem.registerSubmodule(M, require("scripts/CoreMods/cameraExtensions"), "cameraExtensions")
  modsystem.registerSubmodule(M, require("scripts/CoreMods/lightExtensions"), "lightExtensions")
  modsystem.registerSubmodule(M, require("scripts/CoreMods/sirenExtensions"), "sirenExtensions")
  modsystem.registerSubmodule(M, require("scripts/CoreMods/richpresence"), "richpresence")
  modsystem.registerSubmodule(M, require("scripts/CoreMods/cheats"), "cheats")
  modsystem.registerSubmodule(M, require("scripts/CoreMods/menuLabel"), "menuLabel")
  modsystem.registerSubmodule(M, require("scripts/CoreMods/console"), "console")
end

--exports
M.onModLoaded = onModLoaded

return M