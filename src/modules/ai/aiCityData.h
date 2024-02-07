#pragma once
#include <mm2_common.h>
#include "aiDataTypes.h"

namespace MM2
{
    // Forward declarations
    class aiCityData;

    // External declarations


    // Class definitions
    class aiCityData {
    private:
        bool AmbientsDriveOnLeft;
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

        int GetNumAmbientTypes() const
        {
            return this->NumAmbientTypes;
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

        aiAmbientTypeData* GetAmbientTypeData(int index)
        {
            if (index < 0 || index >= this->GetNumAmbientTypes())
                return nullptr;
            return &this->AmbientTypeData[index];
        }

        bool AreAmbientsDriveOnLeft()
        {
            return this->AmbientsDriveOnLeft == true;
        }

        static void BindLua(LuaState L) {
            LuaBinding(L).beginClass<aiCityData>("aiCityData")
                .addVariable("AmbientsDriveOnLeft", &aiCityData::AmbientsDriveOnLeft, false)
                .addVariable("SpeedLimit", &aiCityData::SpeedLimit, false)
                .addVariable("PedPoolSize", &aiCityData::PedPoolSize, false)
                .addPropertyReadOnly("NumPedTypes", &GetNumPedTypes)
                .addPropertyReadOnly("NumAmbientTypes", &GetNumAmbientTypes)
                .addFunction("GetAmbientType", &GetAmbientTypeData)
                .addFunction("GetGoodWeatherPedName", &GetGoodWeatherPedName)
                .addFunction("GetBadWeatherPedName", &GetBadWeatherPedName)
                .endClass();
        }
    };

    ASSERT_SIZEOF(aiCityData, 0x34);
}