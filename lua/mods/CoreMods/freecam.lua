local M = {}

M.info = {
  name = "Core Mods : Freecam Submodule",
  author = "mm2hook Team",
  context = {"game"}
}

local tilt = 0

-- imgui state
local winOpen = false
local drawWindow

local function activateFreecam()
  if Player and Player.Car then
    local playerMatrix = Player.Car.Instance:GetMatrix()
    local freecamPos = Player.FreeCam:GetPosition()
    local freecamDist = freecamPos:Dist(playerMatrix:GetRow(3))
    
    Player.CamView:SetCam(Player.FreeCam)
    Player.FreeCam:UpdateView()
    
    if freecamDist > 100 then
      local newFreecamPos = playerMatrix:GetRow(3) + (playerMatrix:GetRow(2) * 5)
      Player.FreeCam:SetPosition(newFreecamPos)
    end
  end
end

local function onChatMessage(message)
  if message == "/freecam" then
    activateFreecam()
  end
end

local function onRenderUi()
  if not winOpen then return end
  local window_flags = ImGuiWindowFlags_AlwaysAutoResize
    
  --begindraw
  drawWindow, winOpen = imgui.Begin("Freecam Control Panel", true, window_flags)
  if drawWindow then
    -- get freecam
    local freecam = Player.FreeCam
    if freecam then
      freecam.Speed = imgui.Drag("Speed", freecam.Speed, 0.1, 0, 100)
      freecam.RotationSpeed = imgui.Drag("Rotation Speed", freecam.RotationSpeed, 0.1, 0, 10)
      
      local newfov = imgui.Drag("FOV", freecam.FOV, 1, 1, 100)
      if newfov ~= freecam.FOV then
        freecam.FOV = newfov
        freecam:UpdateView()
      end
      
      local newtilt = imgui.Drag("Tilt", tilt, 1, -180, 180)
      if newtilt ~= tilt then
        local mtx = freecam:GetMatrix()
        local pos = mtx:GetRow(3)
        
        local delta = tilt - newtilt
        mtx:Rotate(mtx:GetRow(2), (delta / 180.0) * 3.14) -- rotate around Z axis
        
        freecam:SetMatrix(mtx)
        tilt = newtilt
      end
      
      if imgui.Button("Activate") then
        activateFreecam()
      end
      
      if not NETMGR.InSession then
        imgui.SameLine()
        if imgui.Button("Teleport Car To Freecam") then
          if Player and Player.Car then
            Player.Car:GetICS():SetPosition(freecam:GetMatrix():GetRow(3))
          end
        end
      end
    end
    
    imgui.End()
  end
end

local function drawMenuBar()
  if imgui.BeginMenu("Core") then
     if imgui.MenuItem("Freecam Control Panel", nil, winOpen) then
        winOpen = not winOpen
     end
    imgui.EndMenu()
  end
end

--exports
M.drawMenuBar = drawMenuBar
M.onRenderUi = onRenderUi
M.onChatMessage = onChatMessage

return M