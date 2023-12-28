#pragma once
#include "mm2_common.h"
#include "mm2_gfx.h"
#include <modules\rgl.h>

namespace MM2
{
    /*
        Function declarations
    */

    template<>
    void luaAddModule<module_rgl>(LuaState L) {
        LuaBinding(L)
            .addFunction("vglVertex", [](float x, float y, float z) { vglVertex(x, y, z); })
            .addFunction("gfxDrawFont", &luaGfxDrawFont)

            .addFunction("vglTexCoord2f", static_cast<void(*)(Vector2 vector)>(&vglTexCoord2f))
            .addFunction("vglVertex3f", static_cast<void(*)(const Vector3 vector)>(&vglVertex3f))
            .addFunction("vglBegin", &vglBegin, LUA_ARGS(MM2::gfxDrawMode, _def<int, 0>))
            .addFunction("vglEnd", &vglEnd)
            .addFunction("vglBeginBatch", &vglBeginBatch)
            .addFunction("vglEndBatch", &vglEndBatch)
            .addFunction("vglBindTexture", &vglBindTexture)
            .addFunction("vglUnbindTexture", &vglUnbindTexture)
            .addFunction("vglDrawLabel", &vglDrawLabel)

            .addFunction("mkfrgba", &mkfrgba)

            .addFunction("rglIsEnabled", &rglIsEnabled)
            .addFunction("rglEnableDisable", &rglEnableDisable)

            .addFunction("rglWorldMatrix", &rglWorldMatrix)
            .addFunction("rglWorldIdentity", &rglWorldIdentity)
            .addFunction("rglCameraMatrix", &rglCameraMatrix)

            .addFunction("rglDrawBox", &rglDrawBox)
            .addFunction("rglDrawSolidBox", &rglDrawSolidBox)
            .addFunction("rglDrawAxis", &rglDrawAxis)
            .addFunction("rglDrawSphere", static_cast<void(*)(float p1, int p2)>(&rglDrawSphere))
            .addFunction("rglDrawParticle", &rglDrawParticle)
            .addFunction("rglDrawEllipsoid", &rglDrawEllipsoid)
            .addFunction("rglResetDrawTexture", &rglResetDrawTexture)
            .addFunction("rglDrawTexture", &rglDrawTexture)
            .addFunction("rglPushMatrix", &rglPushMatrix)
            .addFunction("rglPopMatrix", &rglPopMatrix)
            .addFunction("rglMultMatrix", &rglMultMatrix)

            .addFunction("rglSetColor", &luaSetColor);
    }
}