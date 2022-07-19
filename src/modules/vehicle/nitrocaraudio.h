#pragma once
#include "caraudio.h"

namespace MM2
{
    // Forward declarations


    // External declarations


    // Class definitions
    class vehNitroCarAudio : public vehCarAudio
    {
    private:
        float NitroVolume;
        int NitroSampleID;
        AudSoundBase* NitroSample;
    public:
        /*
            Aud3DObject virtuals
        */
        AGE_API void AssignSounds() override                    { hook::Thunk<0x4DD390>::Call<void>(this); }
        AGE_API void UnAssignSounds(int a1) override            { hook::Thunk<0x4DD400>::Call<void>(this, a1); }
        AGE_API void UpdateAudio() override                     { hook::Thunk<0x4DD2A0>::Call<void>(this); }
        AGE_API void Update() override                          { hook::Thunk<0x4DD5E0>::Call<void>(this); }
        AGE_API void SetNon3DParams() override                  { hook::Thunk<0x4DD560>::Call<void>(this); }
        AGE_API void Set3DParams()  override                    { hook::Thunk<0x4DD5D0>::Call<void>(this); }

        /*
            vehNitroCarAudio
        */
        AGE_API void PlayNitro()                                { hook::Thunk<0x4DD6D0>::Call<void>(this); }
        AGE_API void StopNitro()                                { hook::Thunk<0x4DD760>::Call<void>(this); }
        
        static void BindLua(LuaState L) {
            LuaBinding(L).beginExtendClass<vehNitroCarAudio, vehCarAudio>("vehNitroCarAudio")
                .addFunction("PlayNitro", &PlayNitro)
                .addFunction("StopNitro", &StopNitro)
                .endClass();
        }
    };

    ASSERT_SIZEOF(vehNitroCarAudio, 0x13C);
}