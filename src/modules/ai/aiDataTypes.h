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

        LPCSTR GetName() {
            return Name;
        }

        static void BindLua(LuaState L) {
            LuaBinding(L).beginClass<aiAmbientTypeData>("aiAmbientTypeData")
                .addPropertyReadOnly("Name", &GetName)
                .addVariable("SpawnChance", &aiAmbientTypeData::SpawnChance, false)
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
                .addVariable("RoadId", &aiExceptionData::RoadId, false)
                .addVariable("Density", &aiExceptionData::Density, false)
                .addVariable("SpeedLimit", &aiExceptionData::SpeedLimit, false)
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

        LPCSTR GetBasename() {
            return Basename;
        }

        LPCSTR GetWaypointFileName() {
            return WaypointFileName;
        }

        static void BindLua(LuaState L) {
            LuaBinding(L).beginClass<HookmanData>("HookmanData")
                .addPropertyReadOnly("Basename", &GetBasename)
                .addPropertyReadOnly("WaypointFileName", &GetWaypointFileName)
                .addVariable("HideoutPosition", &HookmanData::HideoutPosition, false)
                .endClass();
        }
    };

    struct OpponentData
    {
        char Basename[32];
        char WaypointFileName[32];
        float MaxThrottle;
        float TurnBrakingThreshold;
        float TurnRadius;
        float TurnSpeedMultiplier;
        char UnusedFlag;
        char AvoidTraffic;
        char AvoidProps;
        char AvoidPlayers;
        char AvoidOpponents;
        char BadPathfinding;

        LPCSTR GetBasename() {
            return Basename;
        }

        LPCSTR GetWaypointFileName() {
            return WaypointFileName;
        }

        static void BindLua(LuaState L) {
            LuaBinding(L).beginClass<OpponentData>("OpponentData")
                .addPropertyReadOnly("Basename", &GetBasename)
                .addPropertyReadOnly("WaypointFileName", &GetWaypointFileName)
                .addVariable("MaxThrottle", &OpponentData::MaxThrottle, false)
                .addVariable("TurnBrakingThreshold", &OpponentData::TurnBrakingThreshold, false)
                .addVariable("TurnRadius", &OpponentData::TurnRadius, false)
                .addVariable("TurnSpeedMultiplier", &OpponentData::TurnSpeedMultiplier, false)
                .addVariable("AvoidTraffic", &OpponentData::AvoidTraffic, false)
                .addVariable("AvoidProps", &OpponentData::AvoidProps, false)
                .addVariable("AvoidPlayers", &OpponentData::AvoidPlayers, false)
                .addVariable("AvoidOpponents", &OpponentData::AvoidOpponents, false)
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

        LPCSTR GetBasename() {
            return Basename;
        }

        static void BindLua(LuaState L) {
            LuaBinding(L).beginClass<PoliceData>("PoliceData")
                .addPropertyReadOnly("Basename", &GetBasename)
                .addVariable("Position", &PoliceData::Position, false)
                .addVariable("Rotation", &PoliceData::Rotation, false)
                .addVariable("ChaseDistance", &PoliceData::ChaseDistance, false)
                .addVariable("Flags", &PoliceData::Flags, false)
                .endClass();
        }
    };
}