#pragma once
#include "caraudio.h"

namespace MM2
{
    // Forward declarations


    // External declarations


    // Class definitions
    class vehSemiCarAudio : public vehCarAudio
    {
    private:
        float ReverseVolume;
        float AirBlowVolume;
        AudSoundBase* ReverseAudio;
        AudSoundBase* AirBlowAudio;
        int AirBlowSampleIndex;
        int ReverseSampleIndex;
    public:
        /*
            Aud3DObject virtuals
        */
        AGE_API void AssignSounds() override                    { hook::Thunk<0x4DCC50>::Call<void>(this); }
        AGE_API void UnAssignSounds(int a1) override            { hook::Thunk<0x4DCCF0>::Call<void>(this, a1); }
        AGE_API void UpdateAudio() override                     { hook::Thunk<0x4DCA40>::Call<void>(this); }
        AGE_API void Update() override                          { hook::Thunk<0x4DCF40>::Call<void>(this); }
        AGE_API void SetNon3DParams() override                  { hook::Thunk<0x4DCE90>::Call<void>(this); }
        AGE_API void Set3DParams()  override                    { hook::Thunk<0x4DCF30>::Call<void>(this); }

        static void BindLua(LuaState L) {
            LuaBinding(L).beginExtendClass<vehSemiCarAudio, vehCarAudio>("vehSemiCarAudio")
                .endClass();
        }
    };

    ASSERT_SIZEOF(vehSemiCarAudio, 0x148);
}