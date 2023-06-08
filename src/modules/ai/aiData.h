#pragma once
#include <mm2_common.h>
#include "aiDataTypes.h"

namespace MM2
{
    // Forward declarations
    class aiRaceData;

    // External declarations


    // Class definitions
    class aiRaceData {
    private:
        char RaceFileName[16];
        char RaceDirectory[128];
        bool AmbientLaneChanges;
        float CopChaseDistance;
        ushort ExceptionCount;
        ushort PoliceCount;
        ushort OpponentCount;
        ushort HookmanCount;
        ushort AmbientTypeCount;
        aiAmbientTypeData* AmbientTypeData;
        aiExceptionData* ExceptionData;
        PoliceData* PoliceDatas;
        OpponentData* OpponentDatas;
        HookmanData* HookmanDatas;
        char** GoodWeatherPedNames;
        char** BadWeatherPedNames;
        int NumPedTypes;
    public:
        aiRaceData(void)                                    DONOTCALL;
        aiRaceData(const aiRaceData &&)                     DONOTCALL;
        virtual ~aiRaceData(void) DONOTCALL;

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

        int GetOpponentCount() const
        {
            return this->OpponentCount;
        }

        int GetHookmanCount() const
        {
            return this->HookmanCount;
        }

        int GetPoliceCount() const
        {
            return this->PoliceCount;
        }

        OpponentData* GetOpponentData(int index)
        {
            if (index < 0 || index >= this->GetOpponentCount())
                return nullptr;
            return &this->OpponentDatas[index];
        }

        HookmanData* GetHookmanData(int index)
        {
            if (index < 0 || index >= this->GetHookmanCount())
                return nullptr;
            return &this->HookmanDatas[index];
        }

        PoliceData* GetPoliceData(int index)
        {
            if (index < 0 || index >= this->GetPoliceCount())
                return nullptr;
            return &this->PoliceDatas[index];
        }


        //lua
        static void BindLua(LuaState L) {
            LuaBinding(L).beginClass<aiRaceData>("aiRaceData")
                .addPropertyReadOnly("NumOpponents", &GetOpponentCount)
                .addPropertyReadOnly("NumPolice", &GetPoliceCount)
                .addPropertyReadOnly("NumHookmen", &GetHookmanCount)
                .addVariable("CopChaseDistance", &aiRaceData::CopChaseDistance)
                .addVariable("AmbientLaneChanges", &aiRaceData::AmbientLaneChanges)
                .addFunction("GetOpponent", &GetOpponentData)
                .addFunction("GetHookman", &GetHookmanData)
                .addFunction("GetPolice", &GetPoliceData)
                .addPropertyReadOnly("NumPedTypes", &GetNumPedTypes)
                .addFunction("GetGoodWeatherPedName", &GetGoodWeatherPedName)
                .addFunction("GetBadWeatherPedName", &GetBadWeatherPedName)
                .endClass();
        }
    };

    ASSERT_SIZEOF(aiRaceData, 0xC8);
}