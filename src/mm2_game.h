#pragma once
#include "mm2_common.h"
#include "mm2_base.h"

#include <modules\mmgame.h>

namespace MM2
{
    template<>
    void luaAddModule<module_game>(LuaState L) {
        luaBind<mmGame>(L);
        luaBind<mmGameSingle>(L);
        luaBind<mmGameMulti>(L);
        luaBind<mmGameManager>(L);
        luaBind<mmTimer>(L);
        luaBind<mmArrow>(L);
        luaBind<mmNumberFont>(L);
        luaBind<mmNumber>(L);
        luaBind<mmText>(L);
        luaBind<mmTextNode>(L);
        luaBind<mmMirror>(L);
        luaBind<mmWaypoints>(L);
        luaBind<mmWaypointObject>(L);
        luaBind<mmGameHUD>(L);
        luaBind<mmCRHUD>(L);
        luaBind<mmSpeedIndicator>(L);
        luaBind<mmGearIndicator>(L);
        luaBind<mmLinearGauge>(L);
        luaBind<mmSlidingGauge>(L);
        luaBind<RadialGauge>(L);
        luaBind<mmDashView>(L);
        luaBind<mmExternalView>(L);
        luaBind<mmHUD>(L);
        luaBind<mmCDPlayer>(L);
        luaBind<OppIconInfo>(L);
        luaBind<LuaOppIconInfo>(L);
        luaBind<mmHudMap>(L);
        luaBind<mmIcons>(L);
        luaBind<mmViewMgr>(L);
        luaBind<mmPlayer>(L);
    }
}