#pragma once
#include "mm2_game.h"

#include <modules\mmwidget.h>
#include <modules\mmui.h>

namespace MM2
{
    template<>
    void luaAddModule<module_ui>(LuaState L) {
        luaBind<uiWidget>(L);
        luaBind<UIButton>(L);
        luaBind<UILabel>(L);
        luaBind<UIIcon>(L);
        luaBind<UIBMLabel>(L);
        luaBind<UISlider>(L);
        luaBind<UIBMButton>(L);
        luaBind<UITextRoller2>(L);
        luaBind<UITextDropdown>(L);
        luaBind<UITextField>(L);

        luaBind<mmPopup>(L);
        luaBind<UIMenu>(L);
        luaBind<luaMenu>(L);
        luaBind<PUMenuBase>(L);
        luaBind<LuaPUMenu>(L);
        luaBind<MenuManager>(L);

        luaBind<VehicleSelectBase>(L);
        luaBind<Vehicle>(L);
        luaBind<mmInterface>(L);
    }
}

