#pragma once
#include "mm2_common.h"

#include <modules\gfx.h>
#include <modules\lights.h>

namespace MM2
{
    //Forward declarations

    //External declarations

    //Lua initialization
    template<>
    void luaAddModule<module_gfx>(LuaState L) {
        luaBind<gfxRenderState>(L);
        luaBind<gfxPipeline>(L);

        luaBind<ltLight>(L);
        luaBind<ltLensFlare>(L);
        luaBind<gfxTexture>(L);
        luaBind<gfxTextureCacheEntry>(L);
        luaBind<gfxTextureCachePool>(L);
        luaBind<gfxMaterial>(L);

        luaBind<gfxPacket>(L);

        luaBind<gfxViewport>(L);

        LuaBinding(L)
            .addFunction("gfxGetTexture", &gfxGetTexture)
            .addFunction("gfxFreeTexture", &gfxFreeTexture);
    }
}
