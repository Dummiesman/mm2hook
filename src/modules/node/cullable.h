#pragma once
#include "..\data\base.h"

namespace MM2
{
    // Forward declarations
    class asCullable;

    // External declarations
    extern class Base;

    // Class definitions

    class asCullable : public Base {
    public:
        virtual AGE_API ~asCullable() {
            scoped_vtable x(this);
            hook::Thunk<0x460EA0>::Call<void>(this);
        };

        virtual AGE_API void Cull(void) {
            hook::Thunk<0x4A3440>::Call<void>(this);
        };

        static void BindLua(LuaState L) {
            LuaBinding(L).beginExtendClass<asCullable, Base>("asCullable")
                .addFunction("Cull", &Cull)
                .endClass();
        }
    };

    // Lua initialization

}