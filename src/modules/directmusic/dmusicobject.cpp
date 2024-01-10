#pragma once
#include "DMusicObject.h"

namespace MM2
{
    void DMusicObject::BindLua(LuaState L) {
        LuaBinding(L).beginClass<DMusicObject>("DMusicObject")
            .addPropertyReadOnly("CurrentSegment", &GetCurrentSegment)
            .addPropertyReadOnly("IsPlaying", &IsPlaying)
            .addFunction("PlayMotif", &PlayMotif)
            .addFunction("PlaySegment", &PlaySegment)
            .addFunction("SetGrooveLevel", &SetGrooveLevel)
            .addFunction("SegmentSwitch", &SegmentSwitch)
            .endClass();
    }
}