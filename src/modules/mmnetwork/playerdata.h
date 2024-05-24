#pragma once
#include <mm2_common.h>

namespace MM2
{
    // Forward declarations
    struct mmNetworkPlayerData;

    // External declarations


    // Class definitions
    struct mmNetworkPlayerData
    {
    private:
        bool isLoadedLua() const
        {
            return Loaded == TRUE;
        }
        bool isHostLua() const
        {
            return IsHost == TRUE;
        }
        const char* getVehicleNameLua()
        {
            return VehicleName;
        }
        const char* getCityNameLua()
        {
            return CityName;
        }
    public:
        char VehicleName[80];
        int VehiclePaintjob;
        int CnRTeam;
        BOOL Loaded;  // Set when finished loading
        BOOL IsHost;
        float PhysicsRealism;
        char CityName[80];
        short EnableMultiplayerCheatCheck;
        bool IsModemDialin;
        bool BeganLoading; // Set to true when the loading screen shows (and after completion)
        int TuningCRC;

        //lua
        static void BindLua(LuaState L) {
            LuaBinding(L).beginClass<mmNetworkPlayerData>("mmNetworkPlayerData")
                .addPropertyReadOnly("IsHost", &isHostLua)
                .addPropertyReadOnly("CityName", &getCityNameLua)
                .addPropertyReadOnly("VehicleName", &getVehicleNameLua)
                .addVariable("VehiclePaintjob", &mmNetworkPlayerData::VehiclePaintjob, false)
                .addVariable("CnRTeam", &mmNetworkPlayerData::CnRTeam)
                .addProperty("Loaded", &isLoadedLua)
                .addVariable("IsModemDialin", &mmNetworkPlayerData::IsModemDialin, false)
                .addVariable("TuningCRC", &mmNetworkPlayerData::TuningCRC, false)
                .addVariable("BeganLoading", &mmNetworkPlayerData::BeganLoading, false)
                .endClass();
        }
    };
    ASSERT_SIZEOF(mmNetworkPlayerData, 0xBC);
}