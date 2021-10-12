local M = {}

local function onChatMessage(message)
  if message == "/grav" then
    dgPhysManager.Gravity = (dgPhysManager.Gravity < -10) and -9.8 or -19.6
  elseif message == "/fuzz" then
    HUD.Map.ShowAllCops = not HUD.Map.ShowAllCops
  end
end

--exports
M.onChatMessage = onChatMessage

return M