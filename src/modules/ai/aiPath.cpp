#include "aiPath.h"


namespace MM2 
{
    std::tuple<int, float> aiPath::luaIsPosOnRoad(Vector3 const& pos, float margin) const
    {
        float outVal = 0.f;
        int posOnRoadVal = this->IsPosOnRoad(pos, margin, &outVal);
        return std::tuple<int, float>(posOnRoadVal, outVal);
    }

    float aiPath::GetBaseSpeedLimit() const
    {
        return _baseSpeedLimit.get(this);
    }

    int aiPath::GetId() const
    {
        return _id.get(this);
    }

    aiIntersection* aiPath::GetIntersection(int num) const
    {
        if (num == 0)
            return _intersectionA.get(this);
        else if (num == 1)
            return _intersectionB.get(this);
        else
            return nullptr;
    }

    int aiPath::GetLaneCount(int side) const
    {
        if (side == 0)
            return _lNumLanes.get(this);
        else
            return _rNumLanes.get(this);
    }

    Vector3 aiPath::GetLaneVertex(int section, int lane, int side) const
    {
        if (side == 0)
            return _lLaneVertices.get(this)[section + (lane * this->NumVerts())];
        else
            return _rLaneVertices.get(this)[section + (lane * this->NumVerts())];
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

    float aiPath::GetWidth() const
    {
        return _halfWidth.get(this) * 2.0f;
    }

    AGE_API float aiPath::CenterLength(int startIdx, int endIdx)  const 
    {
        return hook::Thunk<0x547340>::Call<float>(this, startIdx, endIdx); 
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
  
    AGE_API int aiPath::IsPosOnRoad(Vector3 const& pos, float margin, float* outDistanceFromCenter) const
    {
        return hook::Thunk<0x548370>::Call<int>(this, &pos, margin, outDistanceFromCenter);
    }

    AGE_API int aiPath::NumVerts() const 
    {
        return _numSections.get(this); 
    }
    
    AGE_API int aiPath::Lane(Vector3& pos, int roadSide) const 
    {
        return hook::Thunk<0x547900>::Call<int>(this, &pos, roadSide); 
    }

    AGE_API void aiPath::UpdatePedestrians() 
    {
        hook::Thunk<0x544150>::Call<void>(this); 
    }

    void aiPath::BindLua(LuaState L) {
        LuaBinding(L).beginClass<aiPath>("aiPath")
            .addPropertyReadOnly("ID", &GetId)
            .addPropertyReadOnly("NumVerts", &NumVerts)
            .addPropertyReadOnly("Width", &GetWidth)
            .addFunction("GetIntersection", &GetIntersection)
            .addFunction("CenterLength", &CenterLength)
            .addFunction("ClearAmbients", &ClearAmbients)
            .addFunction("ClearPeds", &ClearPeds)
            .addFunction("HasCableCarLine", &HasCableCarLine)
            .addFunction("HasSubwayLine", &HasSubwayLine)
            .addFunction("IsPosOnRoad", &luaIsPosOnRoad)
            .addFunction("Lane", &Lane)
            .addFunction("UpdatePedestrians", &UpdatePedestrians)
            .addFunction("GetLaneCount", &GetLaneCount)
            .addFunction("GetLaneVertex", &GetLaneVertex)
            .addFunction("GetCenterVertex", &GetCenterVertex)
            .addFunction("GetSideDirection", &GetSideDirection)
            .addFunction("GetUpDirection", &GetUpDirection)
            .addFunction("GetForwardDirection", &GetForwardDirection)
            .endClass();
    }
}