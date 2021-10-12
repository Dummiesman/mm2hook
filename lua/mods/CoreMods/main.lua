local imgui = require("imgui") -- abstraction of imgui for ease of use in lua
local freecam = require("mods/CoreMods/freecam")
local lightsToggle = require("mods/CoreMods/lightstoggle")
local richPresence = require("mods/CoreMods/richpresence")
local cheats = require("mods/CoreMods/cheats")

local M = {}

M.info = {
  name = "Core Mods",
  author = "mm2hook Team",
  context = {"game", "menu"}
}

local function onRenderUi()
  if freecam then freecam.onRenderUi() end
end


local function drawMenuBar()
  if imgui.BeginMenu("Core") then
    if freecam then freecam.drawMenuBar() end
    imgui.EndMenu()
  end
end

local function onChatMessage(message)
  if freecam then freecam.onChatMessage(message) end
  if cheats then cheats.onChatMessage(message) end
end

local function onUpdate()
  if lightsToggle then lightsToggle.onUpdate() end
  if richPresence then richPresence.onUpdate() end
end

local function onGamePostInit()
  if richPresence then richPresence.onGamePostInit() end
end

local function onInit()
  if richPresence then richPresence.onInit() end
end

--exports
M.drawMenuBar = drawMenuBar
M.onRenderUi = onRenderUi
M.onUpdate = onUpdate
M.onInit = onInit
M.onGamePostInit = onGamePostInit
M.onChatMessage = onChatMessage

return M