#pragma once
#include <modules\audio\audmanagerbase.h>
#include <modules\mmaudio\ccspeech.h>
#include <modules\mmaudio\racespeech.h>
#include <modules\mmaudio\cnrspeech.h>

namespace MM2
{
    // Forward declarations
    class AudManager;

    class AudManager : public AudManagerBase {
    public:
        AGE_API AudManager() {
            scoped_vtable x(this);
            hook::Thunk<0x519290>::Call<void>(this);
        };

        AGE_API ~AudManager() {
            scoped_vtable x(this);
            hook::Thunk<0x5192D0>::Call<void>(this);
        };

        static hook::Type<AudManager*> Instance;

        //members
        AGE_API mmCNRSpeech* GetCNRSpeechPtr()               { return hook::Thunk<0x5195C0>::Call<mmCNRSpeech*>(this); }
        AGE_API mmRaceSpeech* GetRaceSpeechPtr()             { return hook::Thunk<0x519580>::Call<mmRaceSpeech*>(this); }
        AGE_API mmCCSpeech* GetCCSpeechPtr()                 { return hook::Thunk<0x5195A0>::Call<mmCCSpeech*>(this); }

        AGE_API void AssignWaveVolume(float vol)             { hook::Thunk<0x5197D0>::Call<void>(this, vol); }
        AGE_API void AssignWaveBalance(float vol)            { hook::Thunk<0x519610>::Call<void>(this, vol); }
        AGE_API void AssignCDVolume(float vol)               { hook::Thunk<0x519A30>::Call<void>(this, vol); }
        AGE_API void AssignCDBalance(float vol)              { hook::Thunk<0x519880>::Call<void>(this, vol); }

        AGE_API bool MinInstall() const                      { return hook::Thunk<0x519470>::Call<bool>(this); }

        //asNode overrides
        AGE_API virtual void Update() override               { hook::Thunk<0x519D00>::Call<void>(this); }

        static void BindLua(LuaState L) {
            LuaBinding(L).beginExtendClass<AudManager, AudManagerBase>("AudManager")
                .addStaticFunction("Instance", [] { return (AudManager*)Instance; })
                .addPropertyReadOnly("MinInstall", &MinInstall)

                .addFunction("GetCCSpeechPtr", &GetCCSpeechPtr)
                .addFunction("GetCNRSpeechPtr", &GetCNRSpeechPtr)
                .addFunction("GetRaceSpeechPtr", &GetRaceSpeechPtr)

                .addFunction("AssignWaveVolume", &AssignWaveVolume)
                .addFunction("AssignCDVolume", &AssignCDVolume)
                .addFunction("AssignWaveBalance", &AssignWaveBalance)
                .addFunction("AssignCDBalance", &AssignCDBalance)
            .endClass();
        }
    };

    declhook(0x6B15D0, _Type<AudManager*>, MMAUDMGRPTR);
}