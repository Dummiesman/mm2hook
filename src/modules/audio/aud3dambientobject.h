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
        byte _buffer[0x20];
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
        AGE_API void Update(float a1)                            { hook::Thunk<0x515BE0>::Call<void>(this, a1); }

        static void BindLua(LuaState L) {
            LuaBinding(L).beginExtendClass<Aud3DAmbientObject, Aud3DObject>("Aud3DAmbientObject")
                .addFunction("ActivateSound", &ActivateSound)
                .addFunction("DeactivateSound", &DeactivateSound)
                .addFunction("Update", &Update, LUA_ARGS(_def<float, 0>))
                .endClass();
        }
    };
}