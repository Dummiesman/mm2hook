#pragma once
#include "base.h"

namespace MM2
{
    AGE_API Base::~Base() {
        scoped_vtable x(this);
        hook::Thunk<0x4C8200>::Call<void>(this);
    };

    //lua
    void Base::BindLua(LuaState L) {
        LuaBinding(L).beginClass<Base>("Base")
            .endClass();
    }
}