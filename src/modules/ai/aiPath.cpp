#include "aiPath.h"
#include "..\rgl.h"

namespace MM2 
{
    std::tuple<int, float> aiPath::luaIsPosOnRoad(Vector3 const& pos, float margin) const
    {
        float outVal = 0.f;
        int posOnRoadVal = this->IsPosOnRoad(pos, margin, &outVal);
        return std::tuple<int, float>(posOnRoadVal, outVal);
    }

    Vector3 aiPath::centerPositionLua(float distanceAlongPath)
    {
        Vector3 outPos = Vector3::ORIGIN;
        this->CenterPosition(distanceAlongPath, outPos);
        return outPos;
    }

    float aiPath::GetBaseSpeedLimit() const
    {
        return _baseSpeedLimit.get(this);
    }

    int aiPath::GetId() const
    {
        return _id.get(this);
    }

    int aiPath::GetFlags() const
    {
        return _flags.get(this);
    }

    aiIntersection* aiPath::GetIntersection(int num) const
    {
        if (num == 0)
            return _intersectionA.get(this);
        else
            return _intersectionB.get(this);
    }

    int aiPath::GetSidewalkCount(int side) const
    {
        if (side == 0)
            return _lNumSidewalks.get(this);
        else
            return _rNumSidewalks.get(this);
    }

    int aiPath::GetLaneCount(int side) const
    {
        if (side == 0)
            return _lNumLanes.get(this);
        else
            return _rNumLanes.get(this);
    }

    float aiPath::GetLaneDistances(int lane, int side) const
    {
        if (side == 0)
            return _lLaneDistances.get(this)[lane];
        else
            return _rLaneDistances.get(this)[lane];
    }

    Vector3 aiPath::GetCableCarVertex(int section, int side) const
    {
        if (this->HasCableCarLine(side)) 
        {
            if (side == 0)
                return _lCableCarVertices.get(this)[section];
            else 
                return _rCableCarVertices.get(this)[section];
        }
        return Vector3::ORIGIN;
    }

    Vector3 aiPath::GetSubwayVertex(int section, int side) const
    {
        if (this->HasSubwayLine(side))
        {
            if (side == 0)
                return _lSubwayVertices.get(this)[section];
            else
                return _rSubwayVertices.get(this)[section];
        }
        return Vector3::ORIGIN;
    }

    Vector3 aiPath::GetLaneVertex(int section, int lane, int side) const
    {
        if (side == 0)
            return _lLaneVertices.get(this)[section + (lane * this->NumVerts())];
        else
            return _rLaneVertices.get(this)[section + (lane * this->NumVerts())];
    }

    Vector3 aiPath::GetSidewalkVertex(int section, int lane, int side) const
    {
        if (side == 0)
            return _lLaneVertices.get(this)[section + ((lane + GetLaneCount(side)) * this->NumVerts())];
        else
            return _rLaneVertices.get(this)[section + ((lane + GetLaneCount(side)) * this->NumVerts())];
    }

    Vector3 aiPath::GetCenterVertex(int section) const
    {
        return _sectionVerts.get(this)[section];
    }

    Vector3 aiPath::GetSideDirection(int section) const
    {
        return _sectionOriX.get(this)[section];
    }

    Vector3 aiPath::GetUpDirection(int section) const
    {
        return _sectionOriY.get(this)[section];
    }

    Vector3 aiPath::GetForwardDirection(int section) const
    {
        return _sectionOriZ.get(this)[section];
    }

    Vector3 aiPath::GetTangent(int section) const
    {
        return _tangent.get(this)[section];
    }

    Vector3 aiPath::GetLeftBoundary(int section) const
    {
        return _lBoundaryVertices.get(this)[this->NumVerts() + section];
    }

    Vector3 aiPath::GetRightBoundary(int section) const
    {
        return _rBoundaryVertices.get(this)[this->NumVerts() + section];
    }

    float aiPath::GetWidth() const
    {
        return _halfWidth.get(this) * 2.0f;
    }

    int aiPath::GetRoomCount() const
    {
        return _numRooms.get(this);
    }

    int aiPath::GetRoomId(int index) const
    {
        return _rooms.get(this)[index];
    }

    aiPedestrian* aiPath::GetPedestrians()
    {
        return _pedestrians.get(this);
    }

    void aiPath::RemovePedestrian(aiPedestrian* ped)
    {
        hook::Thunk<0x549910>::Call<void>(this, ped);
    }

    int aiPath::GetBangers(lua_State* L, int section, int side)
    {
        if (section >= 0 && section < this->NumVerts()) 
        {
            aiObstacle* obstacle = (side > 0) ? this->_rBangers.get(this)[section]
                                              : this->_lBangers.get(this)[section];
            return CppFunctor::make<aiObstacleLuaIterator>(L, obstacle);
        }
        return 0;
    }

    int aiPath::GetVehicles(lua_State* L, int section, int side)
    {
        if (section >= 0 && section < this->NumVerts())
        {
            aiObstacle* obstacle = (side > 0) ? this->_rVehicles.get(this)[section]
                                              : this->_lVehicles.get(this)[section];
            return CppFunctor::make<aiObstacleLuaIterator>(L, obstacle);
        }
        return 0;
    }

    int aiPath::GetPedestrians(lua_State* L, int section, int side)
    {
        if (section >= 0 && section < this->NumVerts())
        {
            aiObstacle* obstacle = (side > 0) ? this->_rPedestrians.get(this)[section]
                                              : this->_lPedestrians.get(this)[section];
            return CppFunctor::make<aiObstacleLuaIterator>(L, obstacle);
        }
        return 0;
    }

    AGE_API void aiPath::AddAmbVehicle(aiVehicleSpline* spline, int lane, float dist, int side)
    {
        hook::Thunk<0x5491C0>::Call<void>(this, spline, lane, dist, side);
    }

    AGE_API float aiPath::CenterDist(Vector3 const& pos) const
    {
        return hook::Thunk<0x548850>::Call<float>(this, &pos);
    }

    AGE_API float aiPath::CenterLength(int startIdx, int endIdx)  const
    {
        return hook::Thunk<0x547340>::Call<float>(this, startIdx, endIdx); 
    }

    AGE_API void aiPath::CenterPosition(float distanceAlongPath, Vector3& outPos)
    {
        hook::Thunk<0x547670>::Call<void>(this, distanceAlongPath, &outPos);
    }

    AGE_API int aiPath::CenterIndex(float distance) const
    {
        return hook::Thunk<0x5475D0>::Call<int>(this, distance);
    }

    AGE_API void aiPath::ClearAmbients()
    {
        hook::Thunk<0x547C00>::Call<void>(this); 
    }

    AGE_API void aiPath::ClearPeds() 
    {
        hook::Thunk<0x547BD0>::Call<void>(this);
    }
    
    AGE_API bool aiPath::HasCableCarLine(int roadSide) const 
    {
        return hook::Thunk<0x549980>::Call<bool>(this, roadSide);
    }
  
    AGE_API bool aiPath::HasSubwayLine(int roadSide) const
    {
        return hook::Thunk<0x5499B0>::Call<bool>(this, roadSide);
    }

    AGE_API float aiPath::GetHeading(float distanceWithinSection, int section, int roadSide) const
    {
        return hook::Thunk<0x5473A0>::Call<float>(this, distanceWithinSection, section, roadSide);
    }
  
    AGE_API int aiPath::IsPosOnRoad(Vector3 const& pos, float margin, float* outDistanceFromCenter) const
    {
        return hook::Thunk<0x548370>::Call<int>(this, &pos, margin, outDistanceFromCenter);
    }

    AGE_API int aiPath::NumVehiclesAfterDist(int lane, float distanceAlongPath, int roadSide) const
    {
        return hook::Thunk<0x548DE0>::Call<int>(this, lane, distanceAlongPath, roadSide);
    }

    AGE_API int aiPath::NumVerts() const 
    {
        return _numSections.get(this); 
    }
    
    AGE_API int aiPath::Lane(Vector3 const& pos, int roadSide) const 
    {
        return hook::Thunk<0x547900>::Call<int>(this, &pos, roadSide); 
    }

    AGE_API int aiPath::Index(Vector3 const& pos) const
    {
        return hook::Thunk<0x547820>::Call<int>(this, &pos);
    }

    AGE_API void aiPath::UpdatePedestrians() 
    {
        hook::Thunk<0x544150>::Call<void>(this); 
    }

    AGE_API int aiPath::RoadVertice(Vector3 const& position, int side) const
    {
        return hook::Thunk<0x5485E0>::Call<int>(this, &position, side);
    }

    AGE_API float aiPath::SubSectionLength(int sectionBegin, int sectionEnd, int side)
    {
        return hook::Thunk<0x547360>::Call<float>(this, sectionBegin, sectionEnd, side);
    }

    AGE_API bool aiPath::Direction(Matrix34 const& matrix) const
    {
        return hook::Thunk<0x548320>::Call<bool>(this, &matrix);
    }

    AGE_API bool aiPath::IsOneWay() const
    {
        int lc0 = GetLaneCount(0);
        int lc1 = GetLaneCount(1);
        return (lc0 == 0 && lc1 != 0) || (lc1 == 0 && lc0 != 0);
    }

    void aiPath::Draw() const
    {
        Vector3 drawOffset = Vector3(0.0f, 0.5f, 0.0f);

        for (int side = 0; side <= 1; side++) 
        {
            for (int j = 0; j < this->GetLaneCount(side) + this->GetSidewalkCount(side); j++)
            {
                vglBegin(gfxDrawMode::DRAWMODE_LINELIST, this->NumVerts() * 2);
                for (int i = 0; i < this->NumVerts() - 1; i++)
                {
                    if (i == 0 || i == this->NumVerts() - 2)
                    {
                        vglCurrentColor = mkfrgba(1.0f, 0.0f, 0.0f, 1.0f);
                    }
                    else if (i % 2)
                    {
                        vglCurrentColor = mkfrgba(0.0f, 0.0f, 1.0f, 1.0f);
                    }
                    else
                    {
                        vglCurrentColor = mkfrgba(0.0f, 1.0f, 0.0f, 1.0f);
                    }

                    vglVertex3f(this->GetLaneVertex(i, j, side) + drawOffset);
                    vglVertex3f(this->GetLaneVertex(i + 1, j, side) + drawOffset);
                }
                vglEnd();
            }
        }

        vglBegin(gfxDrawMode::DRAWMODE_LINELIST, this->NumVerts() * 2);
        for (int i = 0; i < this->NumVerts() - 1; i++)
        {
            if (i == 0 || i == this->NumVerts() - 2)
            {
                vglCurrentColor = mkfrgba(1.0f, 0.0f, 0.0f, 1.0f);
            }
            else if (i % 2)
            {
                vglCurrentColor = mkfrgba(0.0f, 0.0f, 1.0f, 1.0f);
            }
            else
            {
                vglCurrentColor = mkfrgba(0.0f, 1.0f, 0.0f, 1.0f);
            }

            vglVertex3f(this->GetCenterVertex(i) + drawOffset);
            vglVertex3f(this->GetCenterVertex(i + 1) + drawOffset);
        }
        vglEnd();
    }

    void aiPath::DrawNormals() const
    {
        vglBegin(gfxDrawMode::DRAWMODE_LINELIST, this->NumVerts() * 2);
        vglCurrentColor = mkfrgba(1.0f, 1.0f, 0.0f, 1.0f);

        for (int i = 0; i < this->NumVerts(); i++)
        {
            Vector3 centerPos = this->GetCenterVertex(i);
            Vector3 upDirection = this->GetUpDirection(i);
            vglVertex3f(centerPos);
            vglVertex3f(centerPos + (upDirection * 5.0f));
        }

        vglEnd();
    }

    void aiPath::DrawId() const
    {
        Vector3 drawPos = this->GetCenterVertex(1);
        drawPos.Y += 3.0f;
        vglDrawLabelf(drawPos, "%d", this->GetId());
    }

    void aiPath::BindLua(LuaState L) {
        LuaBinding(L).beginClass<aiPath>("aiPath")
            .addPropertyReadOnly("ID", &GetId)
            .addPropertyReadOnly("NumVerts", &NumVerts)
            .addPropertyReadOnly("NumRooms", &GetRoomCount)
            .addPropertyReadOnly("Width", &GetWidth)
            .addPropertyReadOnly("Flags", &GetFlags)
            .addFunction("Draw", &Draw)
            .addFunction("DrawNormals", &DrawNormals)
            .addFunction("DrawId", &DrawId)
            .addFunction("GetIntersection", &GetIntersection)
            .addFunction("CenterDist", &CenterDist)
            .addFunction("CenterLength", &CenterLength)
            .addFunction("CenterIndex", &CenterIndex)
            .addFunction("CenterPosition", &centerPositionLua)
            .addFunction("ClearAmbients", &ClearAmbients)
            .addFunction("ClearPeds", &ClearPeds)
            .addFunction("HasCableCarLine", &HasCableCarLine)
            .addFunction("HasSubwayLine", &HasSubwayLine)
            .addFunction("GetHeading", &GetHeading)
            .addFunction("IsPosOnRoad", &luaIsPosOnRoad)
            .addFunction("NumVehiclesAfterDist", &NumVehiclesAfterDist)
            .addFunction("Lane", &Lane)
            .addFunction("Index", &Index)
            .addFunction("RoadVertice", &RoadVertice)
            .addFunction("SubSectionLength", &SubSectionLength)
            .addFunction("Direction", &Direction)
            .addFunction("GetSidewalkCount", &GetSidewalkCount)
            .addFunction("GetLaneCount", &GetLaneCount)
            .addFunction("GetLaneVertex", &GetLaneVertex)
            .addFunction("GetSidewalkVertex", &GetSidewalkVertex)
            .addFunction("GetCableCarVertex", &GetCableCarVertex)
            .addFunction("GetSubwayVertex", &GetSubwayVertex)
            .addFunction("GetCenterVertex", &GetCenterVertex)
            .addFunction("GetSideDirection", &GetSideDirection)
            .addFunction("GetUpDirection", &GetUpDirection)
            .addFunction("GetForwardDirection", &GetForwardDirection)
            .addFunction("GetTangent", &GetTangent)
            .addFunction("GetLeftBoundary", &GetLeftBoundary)
            .addFunction("GetRightBoundary", &GetRightBoundary)
            .addFunction("GetRoomID", &GetRoomId)
            .addFunction("GetVehicles", &GetVehicles)
            .addFunction("GetBangers", &GetBangers)
            .endClass();
    }
}