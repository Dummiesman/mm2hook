local M = {}
local imgui = require("imgui") -- abstraction of imgui for ease of use in lua
local winOpen = false

local tilt = 0

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
  if winOpen then
    local window_flags = ImGuiWindowFlags_AlwaysAutoResize
    
    --begindraw
    winOpen = imgui.Begin("Freecam Control Panel", winOpen, window_flags)
  
    if Player == nil then
      imgui.TextColored(ImVec4(1, 0, 0, 1), "No player found. Freecam only works in ")
    else
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
          mtx:SetRow(3, Vector3(0,0,0)) -- don't rotate around current position
          mtx:Rotate(mtx:GetRow(2), (delta / 180.0) * 3.14) -- rotate around Z axis
          mtx:SetRow(3, pos) -- copy back the position
          
          freecam:SetMatrix(mtx)
          tilt = newtilt
        end
        
        if imgui.Button("Activate") then
          activateFreecam()
        end
        imgui.SameLine()
        if imgui.Button("Teleport Car To Freecam") then
          if Player and Player.Car then
            Player.Car:GetICS().Position = freecam:GetMatrix():GetRow(3)
          end
        end
      end
    end
    
    imgui.End()
  end
end

local function drawMenuBar()
  if imgui.MenuItem("Freecam Control Panel", nil, winOpen) then
    winOpen = not winOpen
  end
end

--exports
M.drawMenuBar = drawMenuBar
M.onRenderUi = onRenderUi
M.onChatMessage = onChatMessage

return M