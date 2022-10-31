#pragma once
#include "mm2_common.h"
#include "mm2_base.h"

#include <modules\mmgame.h>

namespace MM2
{
    template<>
    void luaAddModule<module_game>(LuaState L) {
        luaBind<mmGame>(L);
        luaBind<mmGameManager>(L);
        luaBind<mmTimer>(L);
        luaBind<mmArrow>(L);
        luaBind<mmNumberFont>(L);
        luaBind<mmNumber>(L);
        luaBind<mmHUD>(L);
        luaBind<mmCDPlayer>(L);
        luaBind<OppIconInfo>(L);
        luaBind<mmHudMap>(L);
        luaBind<mmPlayer>(L);
    }
}