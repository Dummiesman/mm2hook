local M = {}

M.info = {
  name = "Physics Debug",
  author = "Dummiesman",
  context = {'game'}
}

local enabled = false
local drawBSpheres= false
local drawBBoxes = false
local drawBounds = false
local drawForces = false
local drawStats = false
local drawHighQualitySDL = true

local physSource = 1

local winOpen = false
local drawWindow

local FaceColor = Vector4(0.8, 0.8, 0.8, 0.8)
local FaceColorSDL = Vector4(0.4, 0.8, 0.4, 0.8)
local NormalsColor = Vector4(0.0, 0.0, 1.0, 1.0)
local EdgeColor = Vector4(0.0, 0.0, 0.0, 1.0)
local BBoxColor = Vector4(1.0, 0.0, 1.0, 1.0)
local FSOuterColor = Vector4(0.0, 0.0, 1.0, 1.0)
local FSInnerColor = Vector4(1.0, 0.0, 0.0, 1.0)
local AppliedForcesColor = Vector4(1.0, 0.0, 1.0, 1.0)
local NormalsScale = 0.25
local BatchSize = 32

local sdlCollection = sdlPolyCollection(512)

local PHYS_SOURCE_MOVERS = 1
local PHYS_SOURCE_ROOM = 2

local function faceCenter(a, b, c, d)
  if d then 
    return (a + b + c + d) / 4.0
  else
    return (a + b + c) / 3.0
  end
end

local function scaleFace(a, b, c, d, scale)
  local center = faceCenter(a, b, c, d)
  
end

local function drawWireBox(size)
  local halfX = size.x / 2.0
  local halfY = size.y / 2.0 
  local halfZ = size.z / 2.0
  
  vglBegin(DRAWMODE_LINELIST, 0)
  
  
  vglVertex(halfX * -1.0, halfY * -1.0, halfZ * -1.0)
  vglVertex(halfX, halfY * -1.0, halfZ * -1.0)
  vglVertex(halfX, halfY * -1.0, halfZ * -1.0)
  vglVertex(halfX, halfY, halfZ * -1.0)
  vglVertex(halfX, halfY, halfZ * -1.0)
  vglVertex(halfX * -1.0, halfY, halfZ * -1.0)
  vglVertex(halfX * -1.0, halfY, halfZ * -1.0)
  vglVertex(halfX * -1.0, halfY * -1.0, halfZ * -1.0)
  vglVertex(halfX * -1.0, halfY * -1.0, halfZ * -1.0)
  vglVertex(halfX * -1.0, halfY * -1.0, halfZ)
  vglVertex(halfX, halfY * -1.0, halfZ * -1.0)
  vglVertex(halfX, halfY * -1.0, halfZ)
  vglVertex(halfX, halfY, halfZ * -1.0)
  vglVertex(halfX, halfY, halfZ)
  vglVertex(halfX * -1.0, halfY, halfZ * -1.0)
  vglVertex(halfX * -1.0, halfY, halfZ)
  vglVertex(halfX * -1.0, halfY * -1.0, halfZ)
  vglVertex(halfX, halfY * -1.0, halfZ)
  vglVertex(halfX, halfY * -1.0, halfZ)
  vglVertex(halfX, halfY, halfZ)
  vglVertex(halfX, halfY, halfZ)
  vglVertex(halfX * -1.0, halfY, halfZ)
  vglVertex(halfX * -1.0, halfY, halfZ)
  vglVertex(halfX * -1.0, halfY * -1.0, halfZ)
  
  vglEnd()
end

local function drawBoundGeometry(bound, matrix)
  rglPushMatrix()
  rglWorldMatrix(matrix)
 
  local restoreBlend = rglIsEnabled(RGL_BLEND)
  rglEnableDisable(RGL_BLEND, true)
  
  rglSetColor(FaceColor)
  
  for i=1,bound.NumPolygons,BatchSize do
    vglBegin(DRAWMODE_TRIANGLELIST, 0)
    for j=i,math.min(i+BatchSize, bound.NumPolygons),1 do
      local poly = bound:GetPolygon(j-1)
      local v0 = bound:GetVertex(poly:GetIndex(0))
      local v1 = bound:GetVertex(poly:GetIndex(1))
      local v2 = bound:GetVertex(poly:GetIndex(2))
      local v3 = bound:GetVertex(poly:GetIndex(3))
      
      if poly.IsQuad then
        vglVertex3f(v0)
        vglVertex3f(v1)
        vglVertex3f(v2)
        
        vglVertex3f(v2)
        vglVertex3f(v3)
        vglVertex3f(v0)
      else
        vglVertex3f(v0)
        vglVertex3f(v1)
        vglVertex3f(v2)
      end
    end
    vglEnd()
  end
 
  rglEnableDisable(RGL_BLEND, restoreBlend)
  
  -- draw normals
  rglSetColor(NormalsColor)
  
  for i=1,bound.NumPolygons,BatchSize do
    vglBegin(DRAWMODE_LINELIST, 0)
    for j=i,math.min(i+BatchSize, bound.NumPolygons),1 do
      local poly = bound:GetPolygon(j-1)
      local v0 = bound:GetVertex(poly:GetIndex(0))
      local v1 = bound:GetVertex(poly:GetIndex(1))
      local v2 = bound:GetVertex(poly:GetIndex(2))
      local v3 = bound:GetVertex(poly:GetIndex(3))
      
      local center
      if poly.IsQuad then
        center = (v0 + v1 + v2 + v3) / 4.0
      else
        center = (v0 + v1 + v2) / 3.0
      end
      
      vglVertex3f(center)
      vglVertex3f(center + (poly.Normal * NormalsScale))
    end
    vglEnd()
  end

  vglEnd()
  
  -- draw edges
  rglSetColor(EdgeColor)
  for i=1,bound.NumEdges,BatchSize do
    vglBegin(DRAWMODE_LINELIST, 0)
    for j=i,math.min(i+BatchSize, bound.NumEdges),1 do
      local edge = bound:GetEdge(j-1)
      
      local v0 = bound:GetVertex(edge:GetIndex(0))
      local v1 = bound:GetVertex(edge:GetIndex(1))
      
      vglVertex3f(v0)
      vglVertex3f(v1)
    end
    vglEnd()
  end
  
  rglPopMatrix()
end

local function lerp(a, b, t)
	return a + (b - a) * t
end

local function drawBoundTerrain(bound, matrix)
  rglPushMatrix()
  rglWorldMatrix(matrix)
  
  -- bounds
  rglSetColor(Vector4(0.5, 0.5, 0.5, 1.0))
  vglBegin(DRAWMODE_LINELIST, 24)
  
  -- minx -> maxx
  vglVertex3f(Vector3(bound.Min.x, bound.Min.y, bound.Min.z))
  vglVertex3f(Vector3(bound.Max.x, bound.Min.y, bound.Min.z))
  
  vglVertex3f(Vector3(bound.Min.x, bound.Max.y, bound.Min.z))
  vglVertex3f(Vector3(bound.Max.x, bound.Max.y, bound.Min.z))
  
  vglVertex3f(Vector3(bound.Min.x, bound.Min.y, bound.Max.z))
  vglVertex3f(Vector3(bound.Max.x, bound.Min.y, bound.Max.z))
  
  vglVertex3f(Vector3(bound.Min.x, bound.Max.y, bound.Max.z))
  vglVertex3f(Vector3(bound.Max.x, bound.Max.y, bound.Max.z))
  
  -- minz -> maxz
  vglVertex3f(Vector3(bound.Min.x, bound.Min.y, bound.Min.z))
  vglVertex3f(Vector3(bound.Min.x, bound.Min.y, bound.Max.z))
  
  vglVertex3f(Vector3(bound.Max.x, bound.Max.y, bound.Min.z))
  vglVertex3f(Vector3(bound.Max.x, bound.Max.y, bound.Max.z))
  
  vglVertex3f(Vector3(bound.Min.x, bound.Min.y, bound.Min.z))
  vglVertex3f(Vector3(bound.Min.x, bound.Min.y, bound.Max.z))
  
  vglVertex3f(Vector3(bound.Max.x, bound.Max.y, bound.Min.z))
  vglVertex3f(Vector3(bound.Max.x, bound.Max.y, bound.Max.z))
  
  -- verticals
  vglVertex3f(Vector3(bound.Max.x, bound.Max.y, bound.Min.z))
  vglVertex3f(Vector3(bound.Max.x, bound.Min.y, bound.Min.z))
  
  vglVertex3f(Vector3(bound.Max.x, bound.Max.y, bound.Max.z))
  vglVertex3f(Vector3(bound.Max.x, bound.Min.y, bound.Max.z))
  
  vglVertex3f(Vector3(bound.Min.x, bound.Max.y, bound.Min.z))
  vglVertex3f(Vector3(bound.Min.x, bound.Min.y, bound.Min.z))
  
  vglVertex3f(Vector3(bound.Min.x, bound.Max.y, bound.Max.z))
  vglVertex3f(Vector3(bound.Min.x, bound.Min.y, bound.Max.z))
  
  vglEnd()
  
  --grid
  rglSetColor(Vector4(0.4, 0.4, 0.4, 1.0))
  
  vglBegin(DRAWMODE_LINELIST, bound.NumWidthSections * 2)
  for i=1,bound.NumWidthSections,1 do
    
    local v0 = Vector3(lerp(bound.Min.x, bound.Max.x, i / bound.NumWidthSections), 0, bound.Min.z)
    local v1 = Vector3(v0.x, 0, bound.Max.z)
    vglVertex3f(v0)
    vglVertex3f(v1)
  end
  vglEnd()
  
  vglBegin(DRAWMODE_LINELIST, bound.NumDepthSections * 2)
  for i=1,bound.NumDepthSections,1 do
    
    local v0 = Vector3(bound.Min.x, 0, lerp(bound.Min.z, bound.Max.z, i / bound.NumDepthSections))
    local v1 = Vector3(bound.Max.x, 0, v0.z)
    vglVertex3f(v0)
    vglVertex3f(v1)
  end
  vglEnd()
  
  rglPopMatrix()
end

local function drawBoundSphere(bound, matrix)
  rglPushMatrix()
  rglWorldMatrix(matrix)
  
  rglSetColor(FaceColor)
  rglDrawSphere(bound.Radius, 8)
  
  rglPopMatrix()
end

local function drawForceSphere(bound, matrix)
  rglPushMatrix()
  rglWorldMatrix(matrix)
  
  rglSetColor(FSInnerColor)
  rglDrawSphere(bound.Radius, 8)
  
  rglSetColor(FSOuterColor)
  rglDrawSphere(bound.MaxRadius, 8)
  
  rglPopMatrix()
end

local function drawBoundHotDog(bound, matrix)
  rglPushMatrix()
  
  rglSetColor(FaceColor)
  rglDrawEllipsoid(Vector3(bound.Radius, bound.Height, bound.Radius), matrix, 8)
  
  rglPopMatrix()
end

local function drawBoundingSphere(instance, matrix)
  rglPushMatrix()
  rglWorldMatrix(matrix)
  
  rglSetColor(BBoxColor)
  rglDrawSphere(instance:GetRadius(), 8)
  
  rglPopMatrix()
end

local function drawBoundingBox(bound, matrix)
  if bound == nil then return end 
  
  rglPushMatrix()
  rglSetColor(BBoxColor)

  local drawMtx = matrix
  
  drawMtx:Normalize()
  drawMtx:SetRow(3, drawMtx:Transform(bound.Offset))

  local size = (bound.Max - bound.Min)
  
  rglWorldMatrix(drawMtx)
  rglSetColor(BBoxColor)
  drawWireBox(size)
  
  rglPopMatrix()
end

local function drawBound(bound, matrix)
  if bound == nil then return end 
  
  local flag = rglEnableDisable(RGL_LIGHTING, false)
  if bound.Type == BOUNDTYPE_GEOMETRY 
  or bound.Type == BOUNDTYPE_BOX then
    drawBoundGeometry(bound, matrix)
  elseif bound.Type == BOUNDTYPE_SPHERE then
    drawBoundSphere(bound, matrix)
  elseif bound.Type == BOUNDTYPE_FORCESPHERE then
    drawForceSphere(bound, matrix)
  elseif bound.Type == BOUNDTYPE_HOTDOG then
    drawBoundHotDog(bound, matrix)
  elseif bound.Type == BOUNDTYPE_TERRAIN then
    local identity = Matrix34()
    identity:Identity()
    
    drawBoundGeometry(bound, matrix)
    drawBoundTerrain(bound, identity)
  elseif bound.Type == BOUNDTYPE_TERRAINLOCAL then
    drawBoundGeometry(bound, matrix)
    drawBoundTerrain(bound, matrix)
  else
    --print("Unsupported bound draw type")
    --print(bound.Type)
  end
  
  rglEnableDisable(RGL_LIGHTING, flag)
end

local function drawAppliedForces(entity)
  if not entity then return end
  local ics = entity:GetICS()

  local invMass = 1.0 / ics.Mass
  local massTimesGrav = dgPhysManager.Gravity * ics.Mass

  rglPushMatrix()
  vglUnbindTexture()
  rglWorldIdentity()
  
  rglSetColor(AppliedForcesColor)
  vglBegin(DRAWMODE_LINELIST, 2)
  vglVertex3f(ics:GetPosition())
  vglVertex3f(ics:GetPosition() + (Vector3.YAXIS * invMass * massTimesGrav))
  vglEnd()
  
  rglPopMatrix()
end

-- temp workaround
local function wrapvec(vec)
  return Vector3(vec.x, vec.y + 0.01, vec.z)
end

local function drawSdl(page, sdlCollection)
  rglPushMatrix()
  rglWorldIdentity()
  
  local restoreBlend = rglIsEnabled(RGL_BLEND)
  rglEnableDisable(RGL_BLEND, true)
  
  for i=1,sdlCollection.Used,BatchSize do
    vglBegin(DRAWMODE_TRIANGLELIST, 0)
    for j=i,math.min(i+BatchSize, sdlCollection.Used),1 do
      local poly = sdlCollection:GetPolygon(j-1)
      local v0 = wrapvec(page:GetCodedVertex(poly:GetIndex(0)))
      local v1 = wrapvec(page:GetCodedVertex(poly:GetIndex(1)))
      local v2 = wrapvec(page:GetCodedVertex(poly:GetIndex(2)))
      local v3 = wrapvec(page:GetCodedVertex(poly:GetIndex(3)))
      
      local colorScale = math.max(0.5, poly.Normal.y)
      rglSetColor(Vector4(FaceColorSDL.x * colorScale, FaceColorSDL.y * colorScale, FaceColorSDL.z * colorScale, FaceColorSDL.w))
      
      if poly.IsQuad then
        vglVertex3f(v0)
        vglVertex3f(v1)
        vglVertex3f(v2)
        
        vglVertex3f(v2)
        vglVertex3f(v3)
        vglVertex3f(v0)
      else
        vglVertex3f(v0)
        vglVertex3f(v1)
        vglVertex3f(v2)
      end
    end
    vglEnd()
  end
  rglEnableDisable(RGL_BLEND, restoreBlend)
  
  -- draw normals
  rglSetColor(NormalsColor)
  
  for i=1,sdlCollection.Used,BatchSize do
    vglBegin(DRAWMODE_LINELIST, 0)
    for j=i,math.min(i+BatchSize, sdlCollection.Used),1 do
      local poly = sdlCollection:GetPolygon(j-1)
      local v0 = wrapvec(page:GetCodedVertex(poly:GetIndex(0)))
      local v1 = wrapvec(page:GetCodedVertex(poly:GetIndex(1)))
      local v2 = wrapvec(page:GetCodedVertex(poly:GetIndex(2)))
      local v3 = wrapvec(page:GetCodedVertex(poly:GetIndex(3)))
      
      local center
      if poly.IsQuad then
        center = (v0 + v1 + v2 + v3) / 4.0
      else
        center = (v0 + v1 + v2) / 3.0
      end
      
      vglVertex3f(center)
      vglVertex3f(center + (poly.Normal * NormalsScale))
    end
    vglEnd()
  end

  rglPopMatrix()
end

local function onCull()
  if not enabled then return end 
  
  local physmgr = dgPhysManager.Instance
  local level = lvlLevel.Singleton
  
  vglUnbindTexture()
  
  if drawStats then
    -- draw stats
    local statDrawer = { yPos = 8, drawStat = function(self, text) gfxDrawFont(8, self.yPos, text) self.yPos = self.yPos + 8 end }
    local stats = physmgr.Stats
    
    local samples = physmgr.NumSamples
    local strfmt = string.format
    
    statDrawer:drawStat(strfmt("Total Physics Update Time:      %5.2fms", stats.TotalUpdateTime))
    statDrawer:drawStat(strfmt("  Obj Pair Collecting Time:     %5.2fms", stats.ObjPairCollectingTime))
    statDrawer:drawStat(strfmt("  Total Collision Time:         %5.2fms (%5.2fms)", stats.TotalCollisionTime, stats.TotalCollisionTime / samples))
    statDrawer:drawStat(strfmt("    Mover Update:               %5.2fms (%5.2fms)", stats.MoverUpdateTime, stats.MoverUpdateTime / samples))
    statDrawer:drawStat(strfmt("    Gathering Movers:           %5.2fms (%5.2fms)", stats.MoverGatherTime, stats.MoverGatherTime / samples))
    statDrawer:drawStat(strfmt("    Collisions:                 %5.2fms (%5.2fms)", stats.CollisionsTime, stats.CollisionsTime / samples))
    statDrawer:drawStat(strfmt("      Obj/Obj Collision:        %5.2fms (%5.2fms)", stats.ObjToObjCollisionTime, stats.ObjToObjCollisionTime / samples))
    statDrawer:drawStat(strfmt("      Obj/Obj Impact:           %5.2fms (%5.2fms)", stats.ObjToObjImpactTime, stats.ObjToObjImpactTime / samples))
    statDrawer:drawStat(strfmt("        VehCarDamage::Impact(): %5.2fms (%5.2fms)", stats.CarImpactTime, stats.CarImpactTime / samples))
    statDrawer:drawStat(strfmt("      Obj/Terrain Collision:    %5.2fms (%5.2fms)", stats.ObjTerrainCollisionTime, stats.ObjTerrainCollisionTime / samples))
    statDrawer:drawStat(strfmt("      Obj/Terrain Impact:       %5.2fms (%5.2fms)", stats.ObjTerrainImpactTime, stats.ObjTerrainImpactTime / samples))
    statDrawer:drawStat(strfmt("  Post-Collision:             %5.2fms", stats.PostCollisionTime))
    statDrawer:drawStat(strfmt("NumSamples:         %5d", samples))
    statDrawer:drawStat(strfmt("NumMovers:          %5d", physmgr.NumMovers))
    statDrawer:drawStat(strfmt("Mover vs Mover:     %5d", stats.MoverVsMover))
    statDrawer:drawStat(strfmt("Mover vs Collidable:%5d", stats.MoverVsCollidable))
    statDrawer:drawStat(strfmt("Total Probes:       %5d", stats.TotalProbes))
    statDrawer:drawStat(strfmt("Total Probe Time:   %5.2fms", stats.TotalProbeTime))
    statDrawer:drawStat(strfmt("External Probes:    %5d", stats.ExternalProbes))
    statDrawer:drawStat(strfmt("External Probe Time:%5.2fms", stats.ExternalProbeTime))
    statDrawer:drawStat(strfmt("NumActiveRooms:     %5d", physmgr.NumActiveRooms))
    statDrawer:drawStat(strfmt("NumCulledMovers:    %5d", physmgr.NumCulledMovers))
  end
    
  -- gather things
  local instances = {}
  if physSource == PHYS_SOURCE_MOVERS then
    for i=1, physmgr.NumMovers, 1 do
      local mover = physmgr:GetMover(i-1)
      table.insert(instances, mover.Instance)
    end
  elseif physSource == PHYS_SOURCE_ROOM then
    local info = level:GetRoomInfo(Player.Car.Instance.CurrentRoom)
    for instance in info:GetInstances() do
      table.insert(instances, instance)
    end
  end
  
  -- draw things
  vglUnbindTexture()
  
  --if physSource == PHYS_SOURCE_ROOM and drawBounds then
  --  local page = cityLevel.SDL:GetPage(Player.Car.Instance.CurrentRoom)
  --  local pos = Player.Car:GetICS():GetPosition()
  --  sdlCollection:BeginCollect(drawHighQualitySDL and Player.Car.Instance.CurrentRoom or 0) -- provide current room or zero, used to lookup tri bound flag
  --  page:CollectAt(sdlCollection, Vector4(pos.x, pos.y, pos.z, 2.0))
  --  sdlCollection:EndCollect()
  --  drawSdl(page, sdlCollection)
  --end
  
  for _, instance in pairs(instances) do
    if drawBounds then
      drawBound(instance:GetBound(0),instance:GetMatrix())
    end
    if drawBBoxes then
      drawBoundingBox(instance:GetBound(0),instance:GetMatrix())
    end
    if drawBSpheres then
      drawBoundingSphere(instance,instance:GetMatrix())
    end
    if drawForces then
      drawAppliedForces(instance:GetEntity())
    end
  end
end

local function onRenderUi()
  if not winOpen then return end

  local window_flags = ImGuiWindowFlags_AlwaysAutoResize
  
  --begindraw
  drawWindow, winOpen = imgui.Begin("Physics Debug", true, window_flags)
  if drawWindow then
  
    enabled = imgui.Checkbox("Enable", enabled)
  
    --
    imgui.Text("Object Source: ")
    imgui.SameLine()
    
    imgui.BeginRadioButtonGroup(physSource)
    imgui.RadioButton("Movers")
    imgui.SameLine()
    imgui.RadioButton("Player Room")
    physSource = imgui.EndRadioButtonGroup()
    
    imgui.Dummy(ImVec2(0, 8))
    imgui.Text("Debug Draw")
    
    drawStats = imgui.Checkbox("Draw Stats", drawStats)
    imgui.Tooltip("Draw on screen stats")
    
    drawBounds = imgui.Checkbox("Draw Bounds", drawBounds)
    imgui.Tooltip("Draw bound geometry/shape")
    
    drawHighQualitySDL = imgui.Checkbox("High SDL LOD", drawHighQualitySDL)
    imgui.Tooltip("Draw bound geometry/shape")
    
    drawBBoxes = imgui.Checkbox("Draw Bounding Boxes", drawBBoxes)
    imgui.Tooltip("Draw bounding boxes")
    
    drawBSpheres = imgui.Checkbox("Draw Bounding Spheres", drawBSpheres)
    imgui.Tooltip("Draw instance bounding spheres")
    
    drawForces = imgui.Checkbox("Draw Applied Forces", drawForces)
    imgui.Tooltip("Draw the force that is applied to an object based on mass")
    
    imgui.End()
  end
end

local function drawMenuBar()
  if imgui.BeginMenu("Debug") then
     if imgui.BeginMenu("Physics") then
       if imgui.MenuItem("Enable", nil, enabled) then
          enabled = not enabled
       end
       imgui.Separator()
       
       imgui.Text("Source: ")
       imgui.SameLine()
       imgui.BeginRadioButtonGroup(physSource)
       imgui.RadioButton("Movers")
       imgui.SameLine()
       imgui.RadioButton("Player Room")
       physSource = imgui.EndRadioButtonGroup()
      
       if imgui.MenuItem("Draw Stats", nil, drawStats) then
          drawStats = not drawStats
       end
       if imgui.MenuItem("Draw Bounds", nil, drawBounds) then
          drawBounds = not drawBounds
       end
       if imgui.MenuItem("High SDL LOD", nil, drawHighQualitySDL) then
          drawHighQualitySDL = not drawHighQualitySDL
       end
       if imgui.MenuItem("Draw Bounding Boxes", nil, drawBBoxes) then
          drawBBoxes = not drawBBoxes
       end
       if imgui.MenuItem("Draw Bounding Spheres", nil, drawBSpheres) then
          drawBSpheres = not drawBSpheres
       end
       imgui.EndMenu()
     end
    imgui.EndMenu()
  end
  
  if imgui.MenuItem("Physics Debug", nil, winOpen) then
    winOpen = not winOpen
  end
end

M.drawMenuBar = drawMenuBar
M.onRenderUi = onRenderUi
M.onCull = onCull
return M