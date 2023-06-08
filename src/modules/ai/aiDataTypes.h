#pragma once
#include <mm2_common.h>

namespace MM2
{
    // Forward declarations
    struct aiAmbientTypeData;
    struct aiExceptionData;
    struct HookmanData;
    struct OpponentData;
    struct PoliceData;

    // External declarations


    // Class definitions
    struct aiAmbientTypeData
    {
        char Name[32];
        float SpawnChance;
        int dword_24;

        static void BindLua(LuaState L) {
            LuaBinding(L).beginClass<aiAmbientTypeData>("aiAmbientTypeData")
                .endClass();
        }
    };

    struct aiExceptionData
    {
        int RoadId;
        float Density;
        float SpeedLimit;

        static void BindLua(LuaState L) {
            LuaBinding(L).beginClass<aiExceptionData>("aiExceptionData")
                .endClass();
        }
    };

    struct HookmanData
    {
        char Basename[32];
        char WaypointFileName[32];
        Vector3 HideoutPosition;
        float float_4c;
        char dword_50;

        static void BindLua(LuaState L) {
            LuaBinding(L).beginClass<HookmanData>("HookmanData")
                .endClass();
        }
    };

    struct OpponentData
    {
        char Basename[32];
        char WaypointFileName[32];
        float SpeedMultiplier;
        float CornerCautionThreshold;
        float CornerCaution;
        float BrakingBias;
        char UnusedFlag;
        char AvoidTraffic;
        char AvoidProps;
        char AvoidPlayers;
        char AvoidOpponents;
        char BadPathfinding;

        static void BindLua(LuaState L) {
            LuaBinding(L).beginClass<OpponentData>("OpponentData")
                .endClass();
        }
    };

    struct PoliceData
    {
        char Basename[32];
        Vector3 Position;
        float Rotation;
        float ChaseDistance;
        float OppChaseChance;
        int Unk1;
        int Flags;

        static void BindLua(LuaState L) {
            LuaBinding(L).beginClass<PoliceData>("PoliceData")
                .endClass();
        }
    };
}