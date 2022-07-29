#pragma once
#include <mm2_common.h>

namespace MM2
{
    // Forward declarations
    class aiCityData;
    struct aiAmbientTypeData;

    // External declarations


    // Class definitions
    struct aiAmbientTypeData
    {
        char Name[32];
        float SpawnChance;
        int dword_24;
    };

    class aiCityData {
    private:
        bool AmbientsDriveOnleft;
        float SpeedLimit;
        int PedPoolSize;
        short NumAmbientTypes;
        aiAmbientTypeData* AmbientTypeData;
        char* SubwayName;
        int SubwayCount;
        char* TrafLitNameSingle;
        char* TrafLitNameDual;
        char** GoodWeatherPedNames;
        char** BadWeatherPedNames;
        int NumPedTypes;
    public:
        aiCityData(void)                                    DONOTCALL;
        aiCityData(const aiCityData &&)                     DONOTCALL;

        virtual ~aiCityData(void)                           DONOTCALL;

        int GetNumPedTypes() const {
            return this->NumPedTypes;
        }

        LPCSTR GetGoodWeatherPedName(int num) const {
            if (num < 0 || num >= NumPedTypes)
                return nullptr;
            return this->GoodWeatherPedNames[num];
        }

        LPCSTR GetBadWeatherPedName(int num) const {
            if (num < 0 || num >= NumPedTypes)
                return nullptr;
            return this->BadWeatherPedNames[num];
        }

        char** GetGoodWeatherPedNames() const {
            return this->GoodWeatherPedNames;
        }

        char** GetBadWeatherPedNames() const {
            return this->BadWeatherPedNames;
        }

        static void BindLua(LuaState L) {
            LuaBinding(L).beginClass<aiCityData>("aiCityData")
                .addVariable("AmbientsDriveOnleft", &aiCityData::AmbientsDriveOnleft, false)
                .addVariable("SpeedLimit", &aiCityData::SpeedLimit, false)
                .addVariable("PedPoolSize", &aiCityData::PedPoolSize, false)
                .addVariable("NumPedTypes", &aiCityData::NumPedTypes, false)
                .addFunction("GetGoodWeatherPedName", &GetGoodWeatherPedName)
                .addFunction("GetBadWeatherPedName", &GetBadWeatherPedName)
                .endClass();
        }
    };

    ASSERT_SIZEOF(aiCityData, 0x34);
}