#pragma once
#include <mm2_common.h>
#include "aud3dobject.h"

namespace MM2
{
    // Forward declarations
    class Aud3DAmbientObject;

    // External declarations

    // Class definitions
    class Aud3DAmbientObject : public Aud3DObject {
    private:
        int NumSounds;
        int AudibleArea;
        char EchoOn;
        float Attenuation;
        float Doppler;
        float Pan;
        float Speed;
        void** SoundData;
    public:
        AGE_API Aud3DAmbientObject() {
            scoped_vtable x(this);
            hook::Thunk<0x5150A0>::Call<void>(this);
        };

        AGE_API ~Aud3DAmbientObject() {
            scoped_vtable x(this);
            hook::Thunk<0x5150E0>::Call<void>(this);
        };

        /*
            Aud3DObject virtuals
        */
        AGE_API void AssignSounds() override                    { hook::Thunk<0x515690>::Call<void>(this); }
        AGE_API void UnAssignSounds(int a1) override            { hook::Thunk<0x5156E0>::Call<void>(this, a1); }
        AGE_API void UpdateAudio() override                     { hook::Thunk<0x5151C0>::Call<void>(this); }

        /*
            Aud3DAmbientObject
        */
        AGE_API void Init(Vector3* positionPtr, LPCSTR csvName)  { hook::Thunk<0x515180>::Call<void>(this, positionPtr, csvName); }
        AGE_API void ActivateSound(int index)                    { hook::Thunk<0x5155A0>::Call<void>(this, index); }
        AGE_API void DeactivateSound(int index)                  { hook::Thunk<0x5155C0>::Call<void>(this, index); }
        AGE_API int GetSoundIndex(LPCSTR soundName)              { return hook::Thunk<0x515610>::Call<int>(this, soundName); }
        AGE_API void PlayOneShot(int index)                      { hook::Thunk<0x5154B0>::Call<void>(this, index); }
        AGE_API void Reset()                                     { hook::Thunk<0x5151B0>::Call<void>(this); }
        AGE_API void Update(float speed)                         { hook::Thunk<0x515BE0>::Call<void>(this, speed); }

        int GetSoundCount() const 
        {
            return NumSounds;
        }

        static void BindLua(LuaState L) {
            LuaBinding(L).beginExtendClass<Aud3DAmbientObject, Aud3DObject>("Aud3DAmbientObject")
                .addPropertyReadOnly("NumSounds", &GetSoundCount)
                .addFunction("ActivateSound", &ActivateSound)
                .addFunction("DeactivateSound", &DeactivateSound)
                .addFunction("GetSoundIndex", &GetSoundIndex)
                .addFunction("PlayOneShot", &PlayOneShot)
                .addFunction("Reset", &Reset)
                .addFunction("Update", &Update, LUA_ARGS(_def<float, 0>))
                .endClass();
        }
    };

    ASSERT_SIZEOF(Aud3DAmbientObject, 0x80);
}