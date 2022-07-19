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
        byte _buffer[0x5C];
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
        AGE_API void AddTo3DMgr()                               { hook::Thunk<0x512230>::Call<void>(this); }
        AGE_API float CalculateAttenuation()                    { return hook::Thunk<0x512280>::Call<float>(this); }
        AGE_API float CalculateDoppler(float speed)             { return hook::Thunk<0x512000>::Call<float>(this, speed); }
        AGE_API float CalculatePan()                            { return hook::Thunk<0x5122C0>::Call<float>(this); }
        AGE_API int GetPriority()                               { return hook::Thunk<0x512380>::Call<int>(this); }
        AGE_API bool PastMaxDistance()                          { return hook::Thunk<0x512090>::Call<bool>(this); }
        AGE_API void Reset()                                    { hook::Thunk<0x511BB0>::Call<void>(this); }
        AGE_API void Set3D(bool threeD)                         { hook::Thunk<0x512320>::Call<void>(this, threeD); }
        AGE_API void SetDropOffs(float min, float max)          { hook::Thunk<0x512060>::Call<void>(this, min, max); }
        AGE_API void SetPositionPtr(Vector3 * position)         { hook::Thunk<0x511B80>::Call<void>(this, position); }
        AGE_API bool WithinMaxDistance()                        { return hook::Thunk<0x512130>::Call<bool>(this); }

        static void BindLua(LuaState L) {
            LuaBinding(L).beginClass<Aud3DObject>("Aud3DObject")
                .addFunction("CalculateDoppler", &CalculateDoppler)
                .addFunction("CalculatePan", &CalculatePan)
                .addFunction("PastMaxDistance", &PastMaxDistance)
                .addFunction("Reset", &Reset)
                .addFunction("Set3D", &Set3D)
                .addFunction("SetDropOffs", &SetDropOffs)
                .addPropertyReadOnly("Priority", &GetPriority)
                .endClass();
        }
    };

    ASSERT_SIZEOF(Aud3DObject, 0x60);
}