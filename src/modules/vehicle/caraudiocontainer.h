#pragma once
#include <modules\vehicle.h>

#include "nitrocaraudio.h"
#include "semicaraudio.h"
#include "policecaraudio.h"
#include "caraudio.h"

namespace MM2
{
    // Forward declarations
    class vehCarAudioContainer;

    // External declarations
    extern class AudImpact;
    extern class Stream;

    // Class definitions

    class vehCarAudioContainer {
    private:
        BOOL HornPlaying;
        int Type;
        vehCarAudio* AudioPtr;
        vehPoliceCarAudio* PoliceAudioPtr;
        vehSemiCarAudio* SemiAudioPtr;
        vehNitroCarAudio* NitroAudioPtr;
    private:
        void silenceEngineLua(bool value)
        {
            this->SilenceEngine(value ? TRUE : FALSE);
        }

        int getActiveAudioLua(lua_State* L)
        {
            if (AudioPtr != nullptr)
                LuaState(L).push(AudioPtr);
            else if (PoliceAudioPtr != nullptr)
                LuaState(L).push(PoliceAudioPtr);
            else if (SemiAudioPtr != nullptr)
                LuaState(L).push(SemiAudioPtr);
            else if (NitroAudioPtr != nullptr)
                LuaState(L).push(NitroAudioPtr);
            else 
                LuaState(L).push(nullptr);
            return 1;
        }
    public:
        AGE_API static bool IsPolice(LPCSTR name)               { return hook::StaticThunk<0x4D1A70>::Call<bool>(name); }
        AGE_API static bool IsSemiOrBus(LPCSTR name)            { return hook::StaticThunk<0x4D1A00>::Call<bool>(name); }
        AGE_API static void RegisterSemiNames(Stream* a1, LPCSTR a2) 
                                                                { hook::StaticThunk<0x4D0E30>::Call<void>(a1, a2); }
        AGE_API static void RegisterPoliceNames(Stream* a1, LPCSTR a2) 
                                                                { hook::StaticThunk<0x4D0F10>::Call<void>(a1, a2); }
        AGE_API static void SetSirenCSVName(LPCSTR name)        { hook::StaticThunk<0x4D0C80>::Call<void>(name); }
        AGE_API void Set3D(bool a1)                             { hook::Thunk<0x4D1840>::Call<void>(this, a1); }
        AGE_API void StartSiren()                               { hook::Thunk<0x4D18B0>::Call<void>(this); }
        AGE_API void StopSiren()                                { hook::Thunk<0x4D18C0>::Call<void>(this); }
        AGE_API void SilenceEngine(BOOL a1)                     { hook::Thunk<0x4D1910>::Call<void>(this, a1); }
        AGE_API vehPoliceCarAudio* GetPoliceCarAudioPtr()       { return hook::Thunk<0x4D1790>::Call<vehPoliceCarAudio*>(this); }
        AGE_API vehSemiCarAudio* GetSemiCarAudioPtr()           { return hook::Thunk<0x4D17A0>::Call<vehSemiCarAudio*>(this); }        
        AGE_API vehNitroCarAudio* GetNitroCarAudioPtr()         { return hook::Thunk<0x4D1780>::Call<vehNitroCarAudio*>(this); }
        AGE_API vehCarAudio * GetCarAudioPtr()                  { return hook::Thunk<0x4D1770>::Call<vehCarAudio*>(this); }
        AGE_API AudImpact * GetAudImpactPtr()                   { return hook::Thunk<0x4D1730>::Call<AudImpact*>(this); }

        AGE_API void PlayHorn() {
            // rewritten to support police audio horns
            // MM2 only calls stop horn on base/nitro/semi audio
            if (this->GetIsHornPlaying())
                return;

            auto activeAudio = GetActiveAudio();
            if (activeAudio != nullptr) {
                activeAudio->PlayHorn();
            }
            this->HornPlaying = TRUE;
        }


        AGE_API void StopHorn() {
            // rewritten to support police audio horns
            // MM2 only calls stop horn on base/nitro/semi audio
            if (!this->GetIsHornPlaying())
                return;

            auto activeAudio = GetActiveAudio();
            if (activeAudio != nullptr) {
                activeAudio->StopHorn();
            }
            this->HornPlaying = FALSE;
        }
        
        vehCarAudio * GetActiveAudio() {
            if (AudioPtr != nullptr)
                return AudioPtr;
            else if (PoliceAudioPtr != nullptr)
                return PoliceAudioPtr;
            else if (SemiAudioPtr != nullptr)
                return SemiAudioPtr;
            else if (NitroAudioPtr != nullptr)
                return NitroAudioPtr;
            return nullptr;
        }

        bool GetIsHornPlaying() {
            return HornPlaying == TRUE;
        }

        static void BindLua(LuaState L) {
            LuaBinding(L).beginClass<vehCarAudioContainer>("vehCarAudioContainer")
                .addStaticFunction("IsPolice", &IsPolice)
                .addStaticFunction("IsSemiOrBus", &IsSemiOrBus)
                .addStaticFunction("SetSirenCSVName", &SetSirenCSVName)
                .addPropertyReadOnly("HornPlaying", &GetIsHornPlaying)
                .addFunction("GetActiveAudio", &getActiveAudioLua)
                .addFunction("Set3D", &Set3D)
                .addFunction("StartSiren", &StartSiren)
                .addFunction("StopSiren", &StopSiren)
                .addFunction("PlayHorn", &PlayHorn)
                .addFunction("StopHorn", &StopHorn)
                .addFunction("SilenceEngine", &silenceEngineLua)
                .endClass();
        }
    };

    ASSERT_SIZEOF(vehCarAudioContainer, 0x18);
}