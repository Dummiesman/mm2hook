#pragma once
#include <mm2_common.h>

namespace MM2
{
    // Forward declarations
    class mmRaceData;

    // External declarations


    // Class definitions
    class mmRaceData {
    private:
        char _buf[0x4B4];
    private:
        bool loadLua(const char* filename, const char* locale) {
            return Load(filename, locale) == TRUE;
        }
    public:
        AGE_API mmRaceData(void) {
            hook::Thunk<0x529B80>::Call<void>(this);
        }

        AGE_API ~mmRaceData(void) {
            hook::Thunk<0x529B90>::Call<void>(this);
        }

        // mmRaceData members
        BOOL Load(const char* filename, const char* locale)             { return hook::Thunk<0x529BA0>::Call<BOOL>(this, filename, locale); }
        int GetWeather(int raceId, dgSkillLevel skillLevel)             { return hook::Thunk<0x529FF0>::Call<int>(this, raceId, skillLevel); }
        int GetTimeOfDay(int raceId, dgSkillLevel skillLevel)           { return hook::Thunk<0x529FC0>::Call<int>(this, raceId, skillLevel); }
        float GetTimeLimit(int raceId, dgSkillLevel skillLevel)         { return hook::Thunk<0x529F30>::Call<float>(this, raceId, skillLevel); }
        char* GetRaceName(int raceId)                                   { return hook::Thunk<0x529EC0>::Call<char*>(this, raceId); }
        float GetPedDensity(int raceId, dgSkillLevel skillLevel)        { return hook::Thunk<0x52A020>::Call<float>(this, raceId, skillLevel); }
        int GetNumRaces()                                               { return hook::Thunk<0x529EB0>::Call<int>(this); }
        int GetNumOpponents(int raceId, dgSkillLevel skillLevel)        { return hook::Thunk<0x529F60>::Call<int>(this, raceId, skillLevel); }
        int GetNumLaps(int raceId, dgSkillLevel skillLevel)             { return hook::Thunk<0x529F00>::Call<int>(this, raceId, skillLevel); }
        int GetNumCops(int raceId, dgSkillLevel skillLevel)             { return hook::Thunk<0x529F90>::Call<int>(this, raceId, skillLevel); }
        float GetDifficulty(int raceId, dgSkillLevel skillLevel)        { return hook::Thunk<0x52A080>::Call<float>(this, raceId, skillLevel); }
        int GetCarType(int raceId, dgSkillLevel skillLevel)             { return hook::Thunk<0x529ED0>::Call<int>(this, raceId, skillLevel); }
        float GetAmbientDensity(int raceId, dgSkillLevel skillLevel)    { return hook::Thunk<0x52A050>::Call<float>(this, raceId, skillLevel); }

        //lua
        static void BindLua(LuaState L) {
            LuaBinding(L).beginClass<mmRaceData>("mmRaceData")
                .addConstructor(LUA_ARGS())
                .addPropertyReadOnly("NumRaces", &GetNumRaces)
                .addFunction("Load", &loadLua)
                .addFunction("GetWeather", &GetWeather)
                .addFunction("GetTimeOfDay", &GetTimeOfDay)
                .addFunction("GetTimeLimit", &GetTimeLimit)
                .addFunction("GetRaceName", &GetRaceName)
                .addFunction("GetPedDensity", &GetPedDensity)
                .addFunction("GetNumOpponents", &GetNumOpponents)
                .addFunction("GetNumLaps", &GetNumLaps)
                .addFunction("GetNumCops", &GetNumCops)
                .addFunction("GetDifficulty", &GetDifficulty)
                .addFunction("GetCarType", &GetCarType)
                .addFunction("GetAmbientDensity", &GetAmbientDensity)
                .endClass();
        }
    };
    ASSERT_SIZEOF(mmRaceData, 0x4B4);
}