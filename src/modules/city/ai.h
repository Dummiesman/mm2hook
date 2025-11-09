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
        AGE_API static lvlAiRoad* GetRoad(int index)                                   { return hook::StaticThunk<0x45D840>::Call<lvlAiRoad*>(index); }
        AGE_API static int GetNumRoads()                                               { return hook::StaticThunk<0x45D860>::Call<int>(); }
        AGE_API static void Delete()                                                   { hook::StaticThunk<0x45D9C0>::Call<void>(); }
        AGE_API static int GetRoom(int index)                                          { return hook::StaticThunk<0x45DA00>::Call<int>(index); }
        AGE_API static bool GetRoomChop(int index)                                     { return hook::StaticThunk<0x45DA20>::Call<bool>(index); }
        AGE_API static int GetNumRooms()                                               { return hook::StaticThunk<0x45DA58>::Call<int>(); }
        AGE_API static int GetNumVertexs()                                             { return hook::StaticThunk<0x45DA60>::Call<int>(); }
        AGE_API static int GetNumLanes(int roadSide)                                   { return hook::StaticThunk<0x45DA70>::Call<int>(roadSide); }
        AGE_API static int GetNumCablecarLanes(int roadSide)                           { return hook::StaticThunk<0x45DAA0>::Call<int>(roadSide); }
        AGE_API static int GetNumSubwayLanes(int roadSide)                             { return hook::StaticThunk<0x45DAB0>::Call<int>(roadSide); }
        AGE_API static int GetNumSidewalks(int roadSide)                               { return hook::StaticThunk<0x45DAE0>::Call<int>(roadSide); }
        AGE_API static Vector3 GetNormal(int row)                                      { return hook::StaticThunk<0x45DAF0>::Call<Vector3>(row); }
        AGE_API static Vector3 GetSidewalkVertex(int a1, int a2, int a3)               { return hook::StaticThunk<0x45DC30>::Call<Vector3>(a1, a2, a3); }
        AGE_API static Vector3 GetSidewalkVertexMulti(int a1, int a2, int a3)          { return hook::StaticThunk<0x45DF60>::Call<Vector3>(a1, a2, a3); }
        AGE_API static Vector3 GetSidewalkVertexSingle(int a1, int a2, int a3)         { return hook::StaticThunk<0x45E130>::Call<Vector3>(a1, a2, a3); }
        AGE_API static Vector3 GetCablecarVertex(int a1, int a2, int a3)               { return hook::StaticThunk<0x45E260>::Call<Vector3>(a1, a2, a3); }
        AGE_API static Vector3 GetSubwayVertex(int a1, int a2, int a3)                 { return hook::StaticThunk<0x45E2A0>::Call<Vector3>(a1, a2, a3); }
        AGE_API static Vector3 GetVertex(int a1, int a2, int a3)                       { return hook::StaticThunk<0x45E2E0>::Call<Vector3>(a1, a2, a3); }
        AGE_API static Vector3 GetVertexMulti(int a1, int a2, int a3)                  { return hook::StaticThunk<0x45E5F0>::Call<Vector3>(a1, a2, a3); }
        AGE_API static Vector3 GetVertexSingle(int a1, int a2, int a3)                 { return hook::StaticThunk<0x45E710>::Call<Vector3>(a1, a2, a3); }
        AGE_API static Vector3 GetVertexSingleCenter(int a1, int a2, int a3)           { return hook::StaticThunk<0x45E7F0>::Call<Vector3>(a1, a2, a3); }
        AGE_API static BOOL IsBlocked()                                                { return hook::StaticThunk<0x45E8C0>::Call<BOOL>(); }
        AGE_API static BOOL IsPedBlocked()                                             { return hook::StaticThunk<0x45E8D0>::Call<BOOL>(); }
        AGE_API static BOOL IsDivided()                                                { return hook::StaticThunk<0x45E8E0>::Call<BOOL>(); }
        AGE_API static BOOL IsAlley()                                                  { return hook::StaticThunk<0x45E8F0>::Call<BOOL>(); }
        AGE_API static BOOL IsFreeway()                                                { return hook::StaticThunk<0x45E900>::Call<BOOL>(); }
        AGE_API static int GetRoadIntersection(int roadSide)                           { return hook::StaticThunk<0x45E910>::Call<int>(roadSide); }
        AGE_API static int GetIntersectionType(int roadSide)                           { return hook::StaticThunk<0x45E930>::Call<int>(roadSide); }
        AGE_API static void GetStopLightPos(int a1, Vector3& a2, Vector3& a3, bool a4) { hook::StaticThunk<0x45E980>::Call<void>(a1, &a2, &a3, a4); }
        AGE_API static char* GetStopLightName(int roadSide)                            { return hook::StaticThunk<0x45EC80>::Call<char*>(roadSide); }
        AGE_API static int GetStopLightType(int roadSide)                              { return hook::StaticThunk<0x45ECC0>::Call<int>(roadSide); }

        AGE_API static void Draw(lvlSDL const & sdl) {
            for (int i = 0; i < GetNumRoads(); i++) {
                SetRoad(sdl, i, true);
                int numVertices = GetNumVertexs();

                for (int s = 0; s < 2; s++) {
                    int numLanes = GetNumLanes(s);
                    int intersectionType = GetIntersectionType(s);
                    if (numLanes == 0) continue;

                    vglBegin(gfxDrawMode::DRAWMODE_LINESTRIP, 0);
                    vglCurrentColor.set(0xFFFFFFFF);

                    Vector3 vertex = GetVertex(numVertices - 1, 0, s);
                    vglVertex3f(vertex.X, vertex.Y + 0.15f, vertex.Z);

                    Vector3 lightA, lightB;
                    GetStopLightPos(1 - s, lightA, lightB, true);
                    if (GetIntersectionType(1 - s) == 1)
                    {
                        vglCurrentColor.set(0xFFFF0000);
                    }
                    else if (GetIntersectionType(1 - s))
                    {
                        vglCurrentColor.set(0xFF0000FF);
                    }
                    else
                    {
                        vglCurrentColor.set(0xFF00FF00);
                    }
                    vglVertex3f(lightA.X, lightA.Y + 0.15f, lightA.Z);
                    vglVertex3f(lightB.X, lightB.Y + 0.15f, lightB.Z);
                    vglEnd();

                    // draw light triangle
                    Vector3 lightDir = (lightA - lightB);
                    lightDir.Normalize();

                    Vector3 midpoint = lightDir;
                    midpoint.Scale(5.0f);
                    midpoint += lightB;

                    Vector3 endpoint = lightDir;
                    endpoint.Cross(Vector3::YAXIS);
                    endpoint.Scale(2.5f);
                    endpoint += lightA;

                    vglBegin(gfxDrawMode::DRAWMODE_TRIANGLELIST, 0);
                    vglVertex3f(endpoint.X, endpoint.Y + 0.15f, endpoint.Z);
                    vglVertex3f(midpoint.X, midpoint.Y + 0.15f, midpoint.Z);
                    vglVertex3f(lightA.X, lightA.Y + 0.15f, lightA.Z);
                    vglEnd();
                }

                for (int s = 0; s < 2; s++) {
                    int numSubwayLanes = GetNumSubwayLanes(s);
                    if (numSubwayLanes == 0) continue;

                    for (int j = 0; j < numSubwayLanes; j++) {
                        vglBegin(gfxDrawMode::DRAWMODE_LINESTRIP, 0);

                        for (int k = 0; k < numVertices; k++) {
                            float percentage = (float)k / (float)(numVertices - 1);
                            int colorAmount = (int)(percentage * 255.0f);
                            int invColorAmount = 255 - colorAmount;

                            vglCurrentColor.set(invColorAmount | ((colorAmount | 0xFFFFFF00) << 16));

                            Vector3 vertex = GetSubwayVertex(k, j, s);
                            vglVertex3f(vertex.X, vertex.Y + 0.15f, vertex.Z);
                        }

                        vglEnd();
                    }
                }
            }
        }

        static void BindLua(LuaState L) {
            LuaBinding(L).beginClass<lvlAiMap>("lvlAiMap")
                .addStaticProperty("NumRoads", &GetNumRoads)
                .addStaticProperty("NumRooms", &GetNumRooms)
                .addStaticProperty("IsFreeway", &IsFreeway)
                .addStaticProperty("IsBlocked", &IsBlocked)
                .addStaticProperty("IsPedBlocked", &IsPedBlocked)
                .addStaticProperty("IsDivided", &IsDivided)
                .addStaticProperty("IsAlley", &IsAlley)
                .addStaticFunction("GetNumSidewalks", &GetNumSidewalks)
                .addStaticFunction("GetNumSubwayLanes", &GetNumSubwayLanes)
                .addStaticFunction("GetNumCablecarLanes", &GetNumCablecarLanes)
                .addStaticFunction("GetNumLanes", &GetNumLanes)
                .addStaticFunction("GetVertex", &GetVertex)
                .addStaticFunction("GetCablecarVertex", &GetCablecarVertex)
                .addStaticFunction("GetSubwayVertex", &GetSubwayVertex)
                .addStaticFunction("GetVertexSingle", &GetVertexSingle)
                .addStaticFunction("GetVertexSingleCenter", &GetVertexSingleCenter)
                .addStaticFunction("GetVertexMulti", &GetVertexMulti)
                .addStaticFunction("GetSidewalkVertex", &GetSidewalkVertex)
                .addStaticFunction("GetSidewalkVertexSingle", &GetSidewalkVertexSingle)
                .addStaticFunction("GetSidewalkVertexMulti", &GetSidewalkVertexMulti)
                .addStaticFunction("SetRoad", [](lvlSDL const& sdl, int roadIndex, bool bevelMode) -> void { SetRoad(sdl, roadIndex, bevelMode); })
                .addStaticFunction("GetRoad", &GetRoad)
                .addStaticFunction("GetRoom", &GetRoom)
                .addStaticFunction("GetIntersectionType", &GetIntersectionType)
                .addStaticFunction("GetStopLightType", &GetStopLightType)
                .addStaticFunction("GetStopLightName", &GetStopLightName)
                .addStaticFunction("GetStopLightPos", &getStopLightPosLua)
                .addStaticFunction("Draw", &Draw)

                .addStaticFunction("DrawStatic", []() -> void
                {
                    //uhh, no idea about inheritance so we have this for now
                    auto sdl = cityLevel::GetSDL();
                    Draw(*sdl);
                })
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