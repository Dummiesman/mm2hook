#pragma once
#include <modules\city.h>

namespace MM2
{
    // Forward declarations
    class lvlAiMap;
    class lvlAiRoad;

    // External declarations
    extern class Stream;

    // Class definitions

    class lvlAiMap {
    private:
        static std::tuple<Vector3, Vector3> getStopLightPosLua(int a1, bool a2) {
            Vector3 lightA, lightB;
            lvlAiMap::GetStopLightPos(a1, lightA, lightB, a2);
            return std::make_tuple(lightA, lightB);
        }
    public:
        AGE_API static void SetRoad(lvlSDL const & sdl, int roadIndex, bool bevelMode) { hook::StaticThunk<0x45D700>::Call<void>(&sdl, roadIndex, bevelMode); }
        AGE_API static void SetRoad(lvlSDL const * sdl, int roadIndex, bool bevelMode) { hook::StaticThunk<0x45D720>::Call<void>(sdl, roadIndex, bevelMode); }
        AGE_API static lvlAiRoad* GetRoad(int a1) { return hook::StaticThunk<0x45D840>::Call<lvlAiRoad*>(a1); }
        AGE_API static int GetRoom(int a1) { return hook::StaticThunk<0x45DA00>::Call<int>(a1); }
        AGE_API static int GetNumRoads() { return hook::StaticThunk<0x45D860>::Call<int>(); }
        AGE_API static int GetNumRooms() { return hook::StaticThunk<0x45DA58>::Call<int>(); }
        AGE_API static int GetNumSubwayLanes(int roadSide) { return hook::StaticThunk<0x45DAB0>::Call<int>(roadSide); }
        AGE_API static int GetNumLanes(int roadSide) { return hook::StaticThunk<0x45DA70>::Call<int>(roadSide); }
        AGE_API static int GetIntersectionType(int roadSide) { return hook::StaticThunk<0x45E930>::Call<int>(roadSide); }
        AGE_API static int GetStopLightType(int roadSide) { return hook::StaticThunk<0x45ECC0>::Call<int>(roadSide); }
        AGE_API static char* GetStopLightName(int roadSide) { return hook::StaticThunk<0x45EC80>::Call<char*>(roadSide); }
        AGE_API static int GetRoadIntersection(int roadSide) { return hook::StaticThunk<0x45E910>::Call<int>(roadSide); }
        AGE_API static Vector3 GetVertex(int a1, int a2, int a3) { return hook::StaticThunk<0x45E2E0>::Call<Vector3>(a1, a2, a3); }
        AGE_API static Vector3 GetSubwayVertex(int a1, int a2, int a3) { return hook::StaticThunk<0x45E2A0>::Call<Vector3>(a1, a2, a3); }
        AGE_API static int GetNumVertexs() { return hook::StaticThunk<0x45DA60>::Call<int>(); }
        AGE_API static void GetStopLightPos(int a1, Vector3& a2, Vector3& a3, bool a4) { hook::StaticThunk<0x45E980>::Call<void>(a1, &a2, &a3, a4); }

        static void BindLua(LuaState L) {
            LuaBinding(L).beginClass<lvlAiMap>("lvlAiMap")
                //.addStaticFunction("SetRoad", static_cast<void(lvlAiMap::*)(lvlSDL const &, int, bool)>(&lvlAiMap::SetRoad))
                .addStaticFunction("GetRoad", &GetRoad)
                .addStaticFunction("GetRoom", &GetRoom)
                .addStaticFunction("GetNumRoads", &GetNumRoads)
                .addStaticFunction("GetNumRooms", &GetNumRooms)
                .addStaticFunction("GetIntersectionType", &GetIntersectionType)
                .addStaticFunction("GetStopLightType", &GetStopLightType)
                .addStaticFunction("GetStopLightName", &GetStopLightName)
                .addStaticFunction("GetStopLightPos", &getStopLightPosLua)
                .addStaticFunction("GetVertex", &GetVertex)
                .addStaticFunction("GetSubwayVertex", &GetSubwayVertex)   
                .addStaticFunction("Draw", &Draw)
            .endClass();
        }
    };

    class lvlAiRoad {
    public:
        AGE_API void LoadBinary(Stream *a1) { hook::Thunk<0x45D870>::Call<void>(this, a1); }

        static void BindLua(LuaState L) {
            LuaBinding(L).beginClass<lvlAiRoad>("lvlAiRoad")
                .addFunction("LoadBinary", &LoadBinary)
            .endClass();
        }
    };

    // Lua initialization

}