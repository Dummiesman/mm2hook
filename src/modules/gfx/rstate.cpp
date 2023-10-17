#include "rstate.h"
#include "pipeline.h"
#include <modules\gfx\viewport.h>

namespace MM2
{
    bool gfxRenderState::sm_EnableTextures = true;

    declfield(gfxRenderState::sm_Camera) = 0x685460;
    declfield(gfxRenderState::sm_Composite) = 0x6854F8;
    declfield(gfxRenderState::sm_World) = 0x685538;
    declfield(gfxRenderState::sm_FullComposite) = 0x6855E0;
    declfield(gfxRenderState::ScaleZ) = 0x685620;
    declfield(gfxRenderState::sm_View) = 0x685660;
    declfield(gfxRenderState::sm_Modelview) = 0x685738;
    declfield(gfxRenderState::m_Touched) = 0x685778;
    declfield(gfxRenderState::m_TouchedMask) = 0x685780;
}