#pragma once
#include <mm2_common.h>

namespace MM2
{
    // Forward declarations
    class Random;

    // External declarations

    // Class definitions

    class Random {
    private:
        byte Index1;
        byte Index2;
        int Array[56];
    public:
        AGE_API float Normal(float a1, float a2)                { return hook::Thunk<0x4A38E0>::Call<float>(this, a1, a2); }
        AGE_API float Number()                                  { return hook::Thunk<0x4A3880>::Call<float>(this); }
        AGE_API void Seed(int seed)                             { hook::Thunk<0x4A37D0>::Call<void>(this, seed); }

        static void BindLua(LuaState L) {
            LuaBinding(L).beginClass<Random>("Random")
                .addConstructor(LUA_ARGS())
                .addFunction("Normal", &Normal)
                .addFunction("Number", &Number)
                .addFunction("Seed", &Seed)
                .endClass();
        }
    };
}