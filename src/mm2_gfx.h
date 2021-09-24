#pragma once
#include "mm2_common.h"

#include <modules\gfx.h>
#include <modules\lights.h>

namespace MM2
{
    //Forward declarations
    class gfxRenderState;

    //External declarations
    extern class datParser;

    class cltLight
    {

    };

    // what kind of primitive to draw
    // same values as D3DPRIMITIVETYPE
    enum gfxDrawMode {
        DRAWMODE_POINTLIST      = 1,
        DRAWMODE_LINELIST       = 2,
        DRAWMODE_LINESTRIP      = 3,
        DRAWMODE_TRIANGLELIST   = 4,
        DRAWMODE_TRIANGLESTRIP  = 5,
        DRAWMODE_TRIANGLEFAN    = 6,
    };

    // Statically available functions
    static gfxTexture * gfxGetTexture(const char *a1, bool a2 = true) {
        return hook::StaticThunk<0x4B30F0>::Call<gfxTexture*>(a1, a2);
    }

    //
    declhook(0x4ABE00, _Func<bool>, $gfxAutoDetect);
    declhook(0x4A8CE0, _Func<void>, $gfxPipeline_SetRes);

    declhook(0x4A88F0, _Func<WNDPROC>, $gfxPipeline_gfxWindowProc);

    declhook(0x4AC3D0, _Func<LPD3DENUMDEVICESCALLBACK7>, $DeviceCallback);
    declhook(0x4AC6F0, _Func<LPDDENUMMODESCALLBACK2>, $ResCallback);

    declhook(0x682FA0, _Type<HWND>, hWndParent);
    declhook(0x6830B8, _Type<HWND>, hWndMain);

    declhook(0x68311C, _Type<LPCSTR>, lpWindowTitle);

    declhook(0x6830F0, _Type<ATOM>, ATOM_Class);
    declhook(0x683108, _Type<LPCSTR>, IconID);

    declhook(0x5CA3EC, _Type<bool>, pageFlip);
    declhook(0x5CA3ED, _Type<bool>, hasBorder);
    declhook(0x5CA3EE, _Type<bool>, useMultiTexture);
    declhook(0x5CA664, _Type<bool>, enableHWTnL);

    declhook(0x68451D, _Type<bool>, novblank);

    declhook(0x6830D0, _Type<bool>, inWindow);
    declhook(0x6830D1, _Type<bool>, isMaximized);
    declhook(0x6830D2, _Type<bool>, tripleBuffer);
    declhook(0x6830D3, _Type<bool>, useReference);
    declhook(0x6830D4, _Type<bool>, useSoftware);
    declhook(0x6830D5, _Type<bool>, useAgeSoftware);
    declhook(0x6830D6, _Type<bool>, useBlade);
    declhook(0x6830D7, _Type<bool>, useSysMem);

    declhook(0x6830D8, _Type<int>, useInterface);

    declhook(0x684518, _Type<LPDIRECTDRAWCREATEEX>, lpDirectDrawCreateEx);

    declhook(0x6830A8, _Type<IDirectDraw7 *>, lpDD);
    declhook(0x6830AC, _Type<IDirect3D7 *>, lpD3D);
    declhook(0x6830C8, _Type<IDirect3DDevice7 *>, lpD3DDev);
    declhook(0x6830CC, _Type<IDirectDrawSurface7 *>, lpdsRend);

    declhook(0x683130, _Type<gfxInterface>, gfxInterfaces);
    declhook(0x6844C0, _Type<unsigned int>, gfxInterfaceCount);

    declhook(0x6844C8, _Type<int>, gfxInterfaceChoice);

    declhook(0x6844B0, _Type<int>, gfxMinScreenWidth);
    declhook(0x6844CC, _Type<int>, gfxMinScreenHeight);

    declhook(0x6844FC, _Type<int>, gfxMaxScreenWidth);
    declhook(0x6844D8, _Type<int>, gfxMaxScreenHeight);

    declhook(0x6B165C, _Type<int>, gfxTexQuality);
    declhook(0x6857D0, _Type<int>, gfxTexReduceSize);

    declhook(0x5CD38C, _Type<gfxImage * (*)(const char *, bool)>, gfxLoadImage);
    declhook(0x5CD680, _Type<gfxImage * (*)(gfxImage*, const char *, bool)>, gfxPrepareImage);

    declhook(0x6B0454, _Type<uint>, mmCpuSpeed);
    
    declhook(0x683124, _Type<gfxViewport*>, gfxCurrentViewport);

    declhook(0x6830F4, _Type<float>, window_fWidth);
    declhook(0x683120, _Type<float>, window_fHeight);

    declhook(0x683128, _Type<int>, window_iWidth);
    declhook(0x683100, _Type<int>, window_iHeight);

    declhook(0x6830E4, _Type<int>, window_ZDepth);
    declhook(0x6830F8, _Type<int>, window_ColorDepth);

    declhook(0x6830EC, _Type<int>, window_X);
    declhook(0x683110, _Type<int>, window_Y);

    template<>
    void luaAddModule<module_gfx>(LuaState L) {
        luaBind<ltLight>(L);
        luaBind<ltLensFlare>(L);
        luaBind<gfxTexture>(L);
        luaBind<gfxTextureCacheEntry>(L);
        luaBind<gfxTextureCachePool>(L);

        LuaBinding(L).addFunction("gfxGetTexture", &gfxGetTexture);
    }

    ASSERT_SIZEOF(gfxRenderStateData, 0x50);
    ASSERT_SIZEOF(gfxRenderState, 0x98);
}
