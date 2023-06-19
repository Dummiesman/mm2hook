#pragma once
#include "mm2_game.h"

#include <modules\mmwidget.h>

namespace MM2
{
    template<>
    void luaAddModule<module_ui>(LuaState L) {
        luaBind<uiWidget>(L);
        luaBind<UILabel>(L);
        luaBind<UIIcon>(L);
        luaBind<UIBMLabel>(L);
        luaBind<UISlider>(L);
        luaBind<UITextRoller2>(L);
        luaBind<UITextDropdown>(L);

        luaBind<mmPopup>(L);
        luaBind<MenuManager>(L);
    }
}
