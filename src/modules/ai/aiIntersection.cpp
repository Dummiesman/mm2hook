#include "aiIntersection.h"


namespace MM2
{
    AGE_API int aiIntersection::NumSources() const 
    {
        return hook::Thunk<0x549D60>::Call<int>(this); 
    }

    AGE_API int aiIntersection::NumSinks() const
    {
        return hook::Thunk<0x549D90>::Call<int>(this); 
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
            .addFunction("GetPath", &GetPath)
            .addPropertyReadOnly("ID", &GetId)
            .addPropertyReadOnly("RoomId", &GetRoomId)
            .addPropertyReadOnly("Center", &GetCenter)
            .addPropertyReadOnly("NumPaths", &GetPathCount)
            .addPropertyReadOnly("NumSinks", &NumSinks)
            .addPropertyReadOnly("NumSources", &NumSources)
            .endClass();
    }
}