#pragma once
#include <modules\city\state.h>
#include <modules\mmcityinfo\playerdata.h>

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
    class NetStartArray;
    class mmStatePack;

    // External declarations

    // Class definitions
    class NetStartArray {
        ulong Slots[10];
    public:
        AGE_API void Clear(void)                            { return hook::Thunk<0x5235C0>::Call<void>(this); }
        AGE_API int GetIndex(ulong playerId)                { return hook::Thunk<0x5235D0>::Call<int>(this, playerId); }
        AGE_API void ClearIndex(ulong playerId)             { return hook::Thunk<0x5235F0>::Call<void>(this, playerId); }
        AGE_API int AssignOpenIndex(ulong playerId)         { return hook::Thunk<0x523610>::Call<int>(this, playerId); }
        AGE_API void Init(ulong *playerIds)                 { return hook::Thunk<0x523650>::Call<void>(this, playerIds); }
    };
    ASSERT_SIZEOF(NetStartArray, 0x28);

    class mmStatePack : public dgStatePack {
    private:
        //lua helper for portals
        bool getUsePortals() {
            return this->UsePortals == TRUE;
        }

        void setUsePortals(bool usePortals) {
            this->UsePortals = (usePortals) ? TRUE : FALSE;
        }

        LPCSTR getNetName() {
            return (LPCSTR)NetName;
        }

        void setNetName(LPCSTR name) {
            strncpy_s(NetName, name, sizeof(NetName));
        }

        LPCSTR getVehicleName() {
            return (LPCSTR)VehicleName;
        }

        void setVehicleName(LPCSTR name) {
            strncpy_s(VehicleName, name, sizeof(VehicleName));
        }

        LPCSTR getCityName() {
            return (LPCSTR)CityName;
        }

        void setCityName(LPCSTR name) {
            strncpy_s(CityName, name, sizeof(CityName));
        }

        LPCSTR getCityLocale() const {
            return (LPCSTR)CityLocale;
        }

        void setCityLocale(LPCSTR name) {
            strncpy_s(CityLocale, name, sizeof(CityLocale));
        }
    public:
        char CityName[40];
        char CityLocale[40]; // same as city name, referred to as 'locale'?
        char ReplayName[40];

        int VehicleId;

        int InputDevice;
        int unk_EC; // seems heavily tied into multiplayer, but also has singleplayer uses?

        BOOL CopsChaseAI; // cops will chase after opponents
        int unk_F4; // unused

        BOOL Shutdown; // stop the process

        /*
            Player settings
        */

        float Difficulty; // based on the SkillLevel
        int TransmissionType;
        float PhysicsRealism; // unused :(
        BOOL UseForceFeedback;
        BOOL DisableProfile;

        char VehicleName[80];
        int VehiclePaintjob;

        char NetName[80];

        /*
            Race settings?
        */

        float TimeLimit;

        int NextState; // -1 = ???, 0 = main menu, 1 = race
        BOOL DisableRegen; // educated guess based on a skipped call to mmPlayer::UpdateRegen if true

        /*
            Audio settings
        */

        float SoundFXVolume;
        float AudioBalance;
        float MusicVolume;
        short NumAudioChannels;
        short unk_1CE[10]; // unused -- not sure what this might've been
        char AudioDeviceName[200];
        bool HasMusicCD; // allow use of music player if true

        /*
            Cops 'n Robbers settings
        */

        int CnRMode;
        int CnRLimitType;
        int CnRTeam;
        float CnRTimeLimit; // in minutes
        int CnRPointLimit;
        int CnRGoldMass; // actual mass units

        /*
            Unknown settings (unused)
        */

        int unk_2C4; // default: 0
        int unk_2C8; // default: 0
        char unk_2CC[160];

        /*
            View settings (NAMES NEED CONFIRMATION!)
        */

        byte ViewMode;
        byte MapMode;
        bool UseWideFOV;
        
        bool DashEnabled;
        bool MirrorEnabled;
        bool HUDEnabled;
        bool unk_372;
        bool IconsEnabled;

        bool unk_374;
        bool unk_375;
        bool HudOrient;
        bool HudZoomIn;

        /*
            Graphics settings
        */

        BOOL BestTextureFilter; // MM1 - unused
        BOOL InterlacedRendering; // MM1 - unused
        float FarClip;
        BOOL EnableSky;
        float LightingQuality;
        BOOL UsePortals;
        int ObjectDetail;
        BOOL EnableReflections; // vehicle reflections
        int CloudShadowQuality;

        /*
            Cheat settings?
        */

        BOOL IsCheating; // true if player entered a cheatcode
        float TimeLimitOverride; // overrides TimeLimit when > 0
        BOOL UnlockRewards; // only works on menu loading

        /*
            Multiplayer settings
        */

        bool AllowCheaters; // don't check tuning CRC
        bool ShowMultiplayerResults; // display results at end of multiplayer
        bool IsHost; // unconfirmed
        bool UseDialup;

        /*
            Global settings?
        */

        BOOL UseIME;
        HIMC IMEContext;

        bool InCrashCourse;

        NetStartArray NetStartArray;

        //LUA
        static void BindLua(LuaState L) {
            beginStatePackLua<mmStatePack>(L, "mmStatePack")

                //mmStatePack specifics
                .addStaticProperty("Instance", [] { return (mmStatePack*)0x6B1610; }) //HACK but it should work

                .addProperty("NetName", &getNetName, &setNetName)
                .addProperty("VehicleName", &getVehicleName, &setVehicleName)
                .addProperty("CityName", &getCityName, &setCityName)
                .addProperty("CityLocale", &getCityLocale, &setCityLocale)
                .addVariable("Difficulty", &mmStatePack::Difficulty)
                .addVariable("VehiclePaintjob", &mmStatePack::VehiclePaintjob)
                .addVariable("InCrashCourse", &mmStatePack::InCrashCourse)
                .addVariable("FarClip", &mmStatePack::FarClip)
                .addVariable("TimeLimit", &mmStatePack::TimeLimit)
                .addVariable("TimeLimitOverride", &mmStatePack::TimeLimitOverride)
                .addProperty("UsePortals", &getUsePortals, &setUsePortals)
                .addVariable("NextState", &mmStatePack::NextState)

                .addVariable("InputDevice", &mmStatePack::InputDevice, false)
                .addVariable("ViewMode", &mmStatePack::ViewMode, false)
                .addVariable("MapMode", &mmStatePack::MapMode, false)
                .addVariable("DashEnabled", &mmStatePack::DashEnabled, false)
                .addVariable("HUDEnabled", &mmStatePack::HUDEnabled, false)
                .addVariable("MirrorEnabled", &mmStatePack::MirrorEnabled, false)
                .addVariable("IconsEnabled", &mmStatePack::IconsEnabled, false)

                .addVariable("CnRGoldMass", &mmStatePack::CnRGoldMass)
                .addVariable("CnRLimitType", &mmStatePack::CnRLimitType)
                .addVariable("CnRMode", &mmStatePack::CnRMode)
                .addVariable("CnRPointLimit", &mmStatePack::CnRPointLimit)
                .addVariable("CnRTeam", &mmStatePack::CnRTeam)
                .addVariable("CnRTimeLimit", &mmStatePack::CnRTimeLimit)
                
                .endClass();
        }
    };
    ASSERT_SIZEOF(mmStatePack, 0x3E0);

    declhook(0x6B1610, _TypeProxy<mmStatePack>, MMSTATE);
    declhook(0x6B19F8, _TypeProxy<mmPlayerData>, MMCURRPLAYER);
}