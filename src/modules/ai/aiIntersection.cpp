#include "aiIntersection.h"
#include "..\rgl.h"

namespace MM2
{
    int aiIntersection::GetBangers(lua_State* L)
    {
        return CppFunctor::make<aiObstacleLuaIterator>(L, this->bangers);
    }

    int aiIntersection::GetVehicles(lua_State* L)
    {
        return CppFunctor::make<aiObstacleLuaIterator>(L, this->vehicles);
    }

    AGE_API int aiIntersection::NumSources() const
    {
        return hook::Thunk<0x549D60>::Call<int>(this); 
    }

    AGE_API int aiIntersection::NumSinks() const
    {
        return hook::Thunk<0x549D90>::Call<int>(this); 
    }

    void aiIntersection::DrawId() const
    {
        Vector3 drawPos = Vector3(this->center.X, this->center.Y + 3.0f, this->center.Z);
        vglDrawLabelf(drawPos, "%d", this->GetId());
    }

    void aiIntersection::DrawPaths() const
    {
        for (int i = 0; i < this->GetPathCount(); i++)
        {
            this->GetPath(i)->Draw();
        }
    }

    int aiIntersection::GetPathCount() const 
    {
        return pathCount;
    }

    aiPath* aiIntersection::GetPath(int id) const 
    {
        if (id >= pathCount)
            return nullptr;
        return paths[id];
    }

    int aiIntersection::GetId() const 
    {
        return id;
    }

    int aiIntersection::GetRoomId() const 
    {
        return roomId;
    }

    Vector3 aiIntersection::GetCenter() const 
    {
        return center;
    }

    void aiIntersection::BindLua(LuaState L) 
    {
        LuaBinding(L).beginClass<aiIntersection>("aiIntersection")
            .addPropertyReadOnly("ID", &GetId)
            .addPropertyReadOnly("RoomId", &GetRoomId)
            .addPropertyReadOnly("Center", &GetCenter)
            .addPropertyReadOnly("NumPaths", &GetPathCount)
            .addPropertyReadOnly("NumSinks", &NumSinks)
            .addPropertyReadOnly("NumSources", &NumSources)
            .addFunction("DrawId", &DrawId)
            .addFunction("DrawPaths", &DrawPaths)
            .addFunction("GetPath", &GetPath)
            .addFunction("GetBangers", &GetBangers)
            .addFunction("GetVehicles", &GetVehicles)
            .endClass();
    }
}