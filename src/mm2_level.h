#pragma once
#include "mm2_common.h"
#include "mm2_inst.h"

#include <modules\gizmo.h>

namespace MM2
{
    template<>
    void luaAddModule<module_level>(LuaState L) {
        luaBind<dgPathPoint>(L);
        luaBind<dgPath>(L);
        luaBind<dgPathSet>(L);
        luaBind<gizPathspline>(L);
        luaBind<gizPathsplineWrapper>(L);
        luaBind<lvlMaterial>(L);
        luaBind<lvlSky>(L);
        luaBind<lvlRoomInfo>(L);
        luaBind<lvlLevel>(L);
        luaBind<gizBridge>(L);
        luaBind<gizBridgeMgr>(L);
        luaBind<gizTrainCar>(L);
        luaBind<gizTrain>(L);
        luaBind<gizTrainMgr>(L);

    }
}