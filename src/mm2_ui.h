#pragma once
#include "mm2_game.h"

#include <modules\mmwidget.h>

namespace MM2
{
    template<>
    void luaAddModule<module_ui>(LuaState L) {
        luaBind<mmPopup>(L);
        luaBind<MenuManager>(L);
    }
}
