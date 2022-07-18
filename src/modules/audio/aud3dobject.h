#pragma once
#include <mm2_common.h>
#include <modules\node\node.h>

namespace MM2
{
    // Forward declarations
    class Aud3DObject;

    // External declarations

    // Class definitions
    class Aud3DObject {
    private:
        byte _buffer[0x60];
    public:
        ANGEL_ALLOCATOR

        AGE_API Aud3DObject() {
            scoped_vtable x(this);
            hook::Thunk<0x511AB0>::Call<void>(this);
        };

        AGE_API ~Aud3DObject() {
            scoped_vtable x(this);
            hook::Thunk<0x511B20>::Call<void>(this);
        };

        /*
            Aud3DObject virtuals
        */
        AGE_API virtual void AssignSounds()                     { hook::Thunk<0x5122F0>::Call<void>(this); }
        AGE_API virtual void UnAssignSounds(int a1)             { hook::Thunk<0x512300>::Call<void>(this, a1); }
        AGE_API virtual void UpdateAudio()                      { hook::Thunk<0x512310>::Call<void>(this); }
        AGE_API virtual void Update()                           { hook::Thunk<0x512390>::Call<void>(this); }
        AGE_API virtual void SetNon3DParams()                   { hook::Thunk<0x512360>::Call<void>(this); }
        AGE_API virtual void Set3DParams()                      { hook::Thunk<0x512370>::Call<void>(this); }

        /*
            Aud3DObject
        */
        AGE_API void SetPositionPtr(Vector3 * position)         { return hook::Thunk<0x511B80>::Call<void>(this, position); }

        static void BindLua(LuaState L) {
            LuaBinding(L).beginClass<Aud3DObject>("Aud3DObject")
                .endClass();
        }
    };
}