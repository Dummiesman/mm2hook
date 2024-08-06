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

        LPCSTR GetBasename() const
        {
            return Basename;
        }

        LPCSTR GetWaypointFileName() const
        {
            return WaypointFileName;
        }

        void SetBasename(LPCSTR basename)
        {
            strcpy_s(Basename, basename);
        }

        void SetWaypointFileName(LPCSTR name)
        {
            strcpy_s(WaypointFileName, name);
        }

        static void BindLua(LuaState L) {
            LuaBinding(L).beginClass<HookmanData>("HookmanData")
                .addProperty("Basename", &GetBasename, &SetBasename)
                .addProperty("WaypointFileName", &GetWaypointFileName, &SetWaypointFileName)
                .addVariable("HideoutPosition", &HookmanData::HideoutPosition)
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

        LPCSTR GetBasename() const 
        {
            return Basename;
        }

        LPCSTR GetWaypointFileName() const 
        {
            return WaypointFileName;
        }

        void SetBasename(LPCSTR basename)
        {
            strcpy_s(Basename, basename);
        }

        void SetWaypointFileName(LPCSTR name)
        {
            strcpy_s(WaypointFileName, name);
        }

        static void BindLua(LuaState L) {
            LuaBinding(L).beginClass<OpponentData>("OpponentData")
                .addProperty("Basename", &GetBasename, &SetBasename)
                .addProperty("WaypointFileName", &GetWaypointFileName, &SetWaypointFileName)
                .addVariable("MaxThrottle", &OpponentData::MaxThrottle)
                .addVariable("TurnBrakingThreshold", &OpponentData::TurnBrakingThreshold)
                .addVariable("TurnRadius", &OpponentData::TurnRadius)
                .addVariable("TurnSpeedMultiplier", &OpponentData::TurnSpeedMultiplier)
                .addVariable("AvoidTraffic", &OpponentData::AvoidTraffic)
                .addVariable("AvoidProps", &OpponentData::AvoidProps)
                .addVariable("AvoidPlayers", &OpponentData::AvoidPlayers)
                .addVariable("AvoidOpponents", &OpponentData::AvoidOpponents)
                .endClass();
        }
    };

    struct PoliceData
    {
        char Basename[32];
        Vector3 Position;
        float Rotation;
        float OppDetectRange;
        float OppChaseChance;
        int Unk1;
        int Flags;

        LPCSTR GetBasename() const {
            return Basename;
        }

        void SetBasename(LPCSTR basename)
        {
            strcpy_s(Basename, basename);
        }

        static void BindLua(LuaState L) {
            LuaBinding(L).beginClass<PoliceData>("PoliceData")
                .addProperty("Basename", &GetBasename, &SetBasename)
                .addVariable("Position", &PoliceData::Position)
                .addVariable("Rotation", &PoliceData::Rotation)
                .addVariable("OpponentDetectionRange", &PoliceData::OppDetectRange)
                .addVariable("OpponentChaseChance", &PoliceData::OppDetectRange)
                .addVariable("Flags", &PoliceData::Flags)
                .endClass();
        }
    };
}