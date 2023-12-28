local M = {}

M.info = {
  name = "Core Mods : About Menu Label",
  author = "mm2hook Team",
  context = {"menu"}
}

local function X(v) return v / 640.0 end
local function Y(v) return v / 480.0 end

local function onStateBegin()
  local aboutMenu = Interface.AboutMenu
  local hookLabel = aboutMenu:AddLabel(-1, "MM2Hook", X(41), Y(405), X(215), Y(16), 0, 16)
  hookLabel:SetText("MM2Hook Loaded")
  
  hookLabel = aboutMenu:AddLabel(-1, "MM2HookBD", X(41), Y(421), X(215), Y(16), 0, 16)
  hookLabel:SetText("Build date: " .. MM2Hook.BuildDate)
  
  hookLabel = aboutMenu:AddLabel(-1, "MM2HookBT", X(41), Y(437), X(215), Y(16), 0, 16)
  hookLabel:SetText("Build time: " .. MM2Hook.BuildTime)
end

--exports
M.onStateBegin = onStateBegin

return M