#pragma once

namespace MM2
{
    // Forward declarations
    class ltLensFlare;

    // External declarations

    // Class definitions
    class ltLensFlare {
    public:
        byte _buffer[0x20];

        ANGEL_ALLOCATOR

        AGE_API ltLensFlare(int a1) { hook::Thunk<0x59BE80>::Call<void>(this, a1); }
        AGE_API ~ltLensFlare() { hook::Thunk<0x59BF80>::Call<void>(this); }

        //member funcs
        AGE_API void DrawBegin() { hook::Thunk<0x59BFA0>::Call<void>(this); }
        AGE_API void DrawEnd() { hook::Thunk<0x59C0C0>::Call<void>(this); }
        AGE_API void Draw(Vector3* position, Vector3* color, float a3) { hook::Thunk<0x59C1C0>::Call<void>(this, position, color, a3); }

        static void BindLua(LuaState L) {
            LuaBinding(L).beginClass<ltLensFlare>("ltLensFlare")
                //members
                .addFunction("DrawBegin", &DrawBegin)
                .addFunction("DrawEnd", &DrawEnd)
                .addFunction("Draw", &Draw)
                .endClass();
        }
    };
    ASSERT_SIZEOF(ltLensFlare, 0x20);

    // Lua initialization

}