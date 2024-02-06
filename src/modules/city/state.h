#pragma once
#include <mm2_common.h>

/*
    !!! IMPORTANT !!!

    Regarding dgStatePack and mmStatePack:

    There's quite a bit of unused stuff unfortunately.
    Some things are leftover from MM1 and others are just never actually used.

    You should verify what you're doing is actually used by the game.
*/
namespace MM2
{
    // Forward declarations
    struct dgStatePack;

    // Lua bind helper
    template<typename T>
    CppBindClass<T, LuaBinding> beginStatePackLua(LuaState L, const char* name) {
        auto binder = LuaBinding(L).beginClass<T>(name);
        binder
            .addProperty("GameMode", &dgStatePack::getGameMode, &dgStatePack::setGameMode)
            .addVariable("RaceId", &dgStatePack::RaceId)

            .addVariable("TrafficDensity", &dgStatePack::TrafficDensity)
            .addVariable("PedestrianDensity", &dgStatePack::PedestrianDensity)
            .addVariable("CopDensity", &dgStatePack::CopDensity)
            .addVariable("OpponentDensity", &dgStatePack::OpponentDensity)
            .addVariable("NumCTFRacers", &dgStatePack::NumberOfCTFRacers)

            .addVariable("MaxAmbientVehicles", &dgStatePack::MaxAmbientVehicles)

            .addProperty("EnableCableCars", &dgStatePack::getEnableCableCars, &dgStatePack::setEnableCableCars)
            .addProperty("EnableSubways", &dgStatePack::getEnableSubways, &dgStatePack::setEnableSubways)

            .addVariable("NumLaps", &dgStatePack::NumLaps)

            .addVariable("TextureQuality", &dgStatePack::TextureQuality)

            .addVariable("TimeOfDay", &dgStatePack::TimeOfDay)
            .addVariable("WeatherType", &dgStatePack::WeatherType)
            .addProperty("SkillLevel", &dgStatePack::getSkillLevel, &dgStatePack::setSkillLevel)
            .addVariable("AudioFlags", &dgStatePack::AudioFlags, false)

            .addProperty("EnablePedestrians", &dgStatePack::getEnablePedestrians, &dgStatePack::setEnablePedestrians);
        return binder;
    }

    // External declarations


    // Class definitions
    struct dgStatePack {
        //FUNCS
        AGE_API dgStatePack(void) {
            scoped_vtable x(this);
            hook::Thunk<0x443110>::Call<void>(this);
        }

        AGE_API ~dgStatePack(void) {
            scoped_vtable x(this);
            hook::Thunk<0x443180>::Call<void>(this);
        }

        //FIELDS
        static hook::Type<dgStatePack*> Instance;

        dgGameMode GameMode;

        int RaceId;

        float TrafficDensity;
        float PedestrianDensity;
        float CopDensity;
        float OpponentDensity;

        /* ---- MC1 leftovers ---- */
        float unk_18; // unused (default: 0.5)
        float unk_1C; // unused (default: 0.5)
        float unk_20; // unused (default: 0.5)
        bool unk_24;  // unused (default: 1)

        int NumberOfCTFRacers; // default: 0
        /* ----------------------- */

        BOOL DisableAI;

        int unk_30; // unused
        int MaxAmbientVehicles;
        int MaxAmbientPedestrians; // unused -- assumed name (default: 100)
        int unk_3C; // unused

        BOOL EnableCableCars;
        BOOL EnableSubways;

        int NumLaps;

        int TextureQuality;
        int unk_50; // unused (default: 99)

        int TimeOfDay;
        int WeatherType;
        dgSkillLevel SkillLevel;
        int AudioFlags;

        BOOL EnablePedestrians;
        void* unk_68; // related to hookmen

        //lua helpers
        int getSkillLevel() {
            return (int)SkillLevel;
        }

        void setSkillLevel(int level) {
            SkillLevel = (dgSkillLevel)level;
        }

        int getGameMode() {
            return (int)GameMode;
        }

        void setGameMode(int mode) {
            GameMode = (dgGameMode)mode;
        }

        bool getEnableCableCars() {
            return !!EnableCableCars;
        }

        void setEnableCableCars(bool value) {
            EnableCableCars = (value) ? TRUE : FALSE;
        }

        bool getEnableSubways() {
            return !!EnableSubways;
        }

        void setEnableSubways(bool value) {
            EnableSubways = (value) ? TRUE : FALSE;
        }

        bool getEnablePedestrians() {
            return !!EnablePedestrians;
        }

        void setEnablePedestrians(bool value) {
            EnablePedestrians = (value) ? TRUE : FALSE;
        }

        //LUA
        static void BindLua(LuaState L) {
            beginStatePackLua<dgStatePack>(L, "dgStatePack")
                .endClass();
        }
    };
    ASSERT_SIZEOF(dgStatePack, 0x6C);
}