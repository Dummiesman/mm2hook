#pragma once
#include "infobase.h"

namespace MM2
{
    // Forward declarations
    class mmPlayerData;

    // External declarations


    // Class definitions
    class mmPlayerData : public mmInfoBase {
    private:
        char Name[40];
        char NetName[40];
        char FileName[40];
        char dword_100[40];
        char SkillLevel;
        float TagID;
        char VehicleName[80];
        int VehiclePaintjob;
        int GameMode;
        int RaceId;
        char char_18C;
        char City[80];
    public:
        AGE_API mmPlayerData(void) {
            scoped_vtable x(this);
            hook::Thunk<0x526FE0>::Call<void>(this);
        }

        virtual AGE_API ~mmPlayerData(void) {
            scoped_vtable x(this);
            hook::Thunk<0x523850>::Call<void>(this);
        }

        // mmPlayerData members
        int GetCheckpointProgress(const char* city, int a3)      { return hook::Thunk<0x527230>::Call<int>(this, city, a3); }
        float GetTagID()                                         { return hook::Thunk<0x527250>::Call<float>(this); }
        void SetTagID(float tag)                                 { hook::Thunk<0x527260>::Call<void>(this, tag); }
        int GetProgress(const char* city, dgGameMode gameMode)   { return hook::Thunk<0x527270>::Call<int>(this, city, gameMode); }
        int GetPassedMask(const char* city, dgGameMode gameMode) { return hook::Thunk<0x5272A0>::Call<int>(this, city, gameMode); }
        int GetNumPassed(const char* city, dgGameMode gameMode)  { return hook::Thunk<0x527370>::Call<int>(this, city, gameMode); }
        int GetTotalPassed(const char* city)                     { return hook::Thunk<0x527440>::Call<int>(this, city); }
        int GetScore(const char* city, dgGameMode gameMode)      { return hook::Thunk<0x527530>::Call<int>(this, city, gameMode); }
        int GetTotalScore(const char* city)                      { return hook::Thunk<0x527710>::Call<int>(this, city); }
        int ResolveCheckpointProgress(const char* city)          { return hook::Thunk<0x527890>::Call<int>(this, city); }
        int ResolveCrashProgress(const char* city)               { return hook::Thunk<0x5279D0>::Call<int>(this, city); }

        LPCSTR GetName()                                         { return Name; }
        LPCSTR GetNetName()                                      { return NetName; }

        //lua
        static void BindLua(LuaState L) {
            LuaBinding(L).beginExtendClass<mmPlayerData, mmInfoBase>("mmPlayerData")
                .addPropertyReadOnly("Name", &GetName)
                .addPropertyReadOnly("NetName", &GetNetName)
                .addProperty("TagID", &GetTagID, &SetTagID)
                .addFunction("GetProgress", &GetProgress)
                .addFunction("GetPassedMask", &GetPassedMask)
                .addFunction("GetNumPassed", &GetNumPassed)
                .addFunction("GetTotalPassed", &GetTotalPassed)
                .addFunction("GetScore", &GetScore)
                .addFunction("GetTotalScore", &GetTotalScore)
                .addFunction("GetCheckpointProgress", &GetCheckpointProgress)
                .addFunction("ResolveCheckpointProgress", &ResolveCheckpointProgress)
                .addFunction("ResolveCrashProgress", &ResolveCrashProgress)
                .endClass();
        }
    };
    ASSERT_SIZEOF(mmPlayerData, 0x1E0);
}