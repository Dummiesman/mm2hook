local M = {}

M.info = {
  name = "AIMAP Debug",
  author = "Dummiesman",
  context = {'game'}
}

local currentComponentType = nil
local currentComponentId = nil

local dbgOffset = Vector3(0.0, 0.2, 0.0)
local enabled = false
local showComponentInfo = false
local showObstacles = false
local drawPaths = false
local drawIntersections = false
local drawPedDebug = false
local drawAmbientDebug = false
local drawOpponentDebug = false

local cmpNameMap = {
  [0] = "AICOMPTYPE_NONE",
  [1] = "AICOMPTYPE_ROAD",
  [2] = "AICOMPTYPE_SHORTCUT",
  [3] = "AICOMPTYPE_INTERSECTION"
}

local function onUpdate() 
  if not enabled then return end
  
  local playerCar = Player.Car.Instance
  currentComponentType, currentComponentId = AIMAP:MapComponent(playerCar:GetPosition(), playerCar.CurrentRoom)
end

local function onStateBegin()
  
end

local function drawObstacle(obstacle, nonAccidentColor, radius)
  local obstacleMatrix = Matrix34.I
  local position = obstacle:GetPosition()
  obstacleMatrix:SetRow(3, position)
  
  local color = obstacle.InAccident and Vector4(1.0, 0.0, 0.0, 1.0) or nonAccidentColor
  rglWorldMatrix(obstacleMatrix)
  rglSetColor(color)
  
  rglDrawSphere(radius, 16)
end

local function drawObstaclesOnPath(path)
  vglUnbindTexture()
  rglPushMatrix()
  
  for i=1, path.NumVerts,1 do -- road section
    for j=0, 1, 1 do -- road side
      for obstacle in path:GetBangers(i-1, j) do 
        drawObstacle(obstacle, Vector4(0.0, 1.0, 0.0, 1.0), 1.0)
      end
      for obstacle in path:GetVehicles(i-1, j) do 
        drawObstacle(obstacle, Vector4(0.0, 0.0, 1.0, 1.0), 3.0)
      end
    end
  end
  
  rglPopMatrix()
end

local function drawObstaclesInIntersection(intersection)
  vglUnbindTexture()
  rglPushMatrix()
  
  for obstacle in intersection:GetBangers() do 
    drawObstacle(obstacle, Vector4(0.0, 1.0, 0.0, 1.0), 1.0)
  end
  for obstacle in intersection:GetVehicles() do 
    drawObstacle(obstacle, Vector4(0.0, 0.0, 1.0, 1.0), 3.0)
  end
  
  rglPopMatrix()
end

local function onCull()
  if not enabled then return end
  
  if showComponentInfo then
    vglUnbindTexture()
    gfxDrawFont(0, 32, "Current component type: " .. cmpNameMap[currentComponentType] .. " (" .. tostring(currentComponentType) .. ")")
    gfxDrawFont(0, 44, "Current component ID: " .. tostring(currentComponentId))
  end
  
  if drawPaths then
    rglPushMatrix()
    rglWorldIdentity()   
    vglUnbindTexture()
    
    for i=1,AIMAP.NumPaths,1 do
      local path = AIMAP:Path(i-1)
      if path then
        path:Draw()
        path:DrawNormals()
        --path:DrawId()
      end
    end
    rglPopMatrix()
  end
  
  if drawIntersections then
    for i=1,AIMAP.NumIntersections,1 do
      local intersection = AIMAP:Intersection(i-1)
      
      rglPushMatrix()
      rglWorldIdentity()      
      vglUnbindTexture()
      
      --intersection:DrawId()
      
      rglPopMatrix()
    end
  end
 
  if drawPedDebug then
    rglPushMatrix()
    rglWorldIdentity()    
    vglUnbindTexture()    
    for i=1,AIMAP.NumPedestrians,1 do
      local ped = AIMAP:Pedestrian(i-1)
      ped:DrawDebug()
    end
    rglPopMatrix()
  end
  
  if drawAmbientDebug then
    rglPushMatrix()
    rglWorldIdentity()   
    vglUnbindTexture()        
    for i=1,AIMAP.NumAmbientVehicles,1 do
      local amb = AIMAP:Vehicle(i-1)
      amb:DrawId()
      amb:DrawBBox(false)
    end
    rglPopMatrix()
  end
  
  if drawOpponentDebug then
    rglPushMatrix()
    rglWorldIdentity()   
    vglUnbindTexture()        
    for i=1,AIMAP.NumOpponents,1 do
      local opp = AIMAP:Opponent(i-1)
      --opp:DrawId()
      opp:DrawRouteThroughTraffic()
    end
    rglPopMatrix()
  end
  
  if showObstacles then
    if currentComponentType == AICOMPTYPE_ROAD then
      drawObstaclesOnPath(AIMAP:Path(currentComponentId))
    elseif currentComponentType == AICOMPTYPE_INTERSECTION then
      drawObstaclesInIntersection(AIMAP:Intersection(currentComponentId))
    end
  end
end

local function drawMenuBar()
  if imgui.BeginMenu("Debug") then
     if imgui.BeginMenu("AI") then
       if imgui.MenuItem("Enable", nil, enabled) then
          enabled = not enabled
       end
       imgui.Separator()
       
       if imgui.MenuItem("Show Component Info", nil, showComponentInfo) then
          showComponentInfo = not showComponentInfo
       end
       if imgui.MenuItem("Draw Paths", nil, drawPaths) then
          drawPaths = not drawPaths
       end
       if imgui.MenuItem("Draw Intersections", nil, drawIntersections) then
          drawIntersections = not drawIntersections
       end
       if imgui.MenuItem("Draw Pedestrian Debug", nil, drawPedDebug) then
          drawPedDebug = not drawPedDebug
       end
       if imgui.MenuItem("Draw Traffic Debug", nil, drawAmbientDebug) then
          drawAmbientDebug = not drawAmbientDebug
       end
       if imgui.MenuItem("Draw Opponent Debug", nil, drawOpponentDebug) then
          drawOpponentDebug = not drawOpponentDebug
       end
       if imgui.MenuItem("Show Obstacles", nil, showObstacles) then
          showObstacles = not showObstacles
       end
       imgui.EndMenu()
     end
    imgui.EndMenu()
  end
end

M.drawMenuBar = drawMenuBar
M.onStateBegin = onStateBegin
M.onUpdate = onUpdate
M.onCull = onCull

return M