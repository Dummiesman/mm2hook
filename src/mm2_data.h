#pragma once
#include "mm2_common.h"
#include "mm2_stream.h"

#include <modules\core.h>
#include <modules\data.h>
#include <modules\parse.h>

namespace MM2
{
    template<>
    void luaAddModule<module_data>(LuaState L) {
        luaBind<datOutput>(L);
        luaBind<datStack>(L);
        luaBind<datTimeManager>(L);
        luaBind<datBaseTokenizer>(L);
        luaBind<datBinTokenizer>(L);
        luaBind<datAsciiTokenizer>(L);
        luaBind<datTokenizer>(L);
        luaBind<datMultiTokenizer>(L);
        luaBind<datAssetManager>(L);
        luaBind<datArgParser>(L);
    }
}