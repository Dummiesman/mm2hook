#pragma once
#include <mm2_common.h>

namespace MM2
{
    // Forward declarations
    class gfxPipeline;
    struct gfxInterface;

    // External declarations
    extern class gfxViewport;
    extern class gfxTexture;
    extern class gfxBitmap;
    extern class gfxImage;

    // Statically available functions
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

    declhook(0x6830A8, _Type<IDirectDraw7*>, lpDD);
    declhook(0x6830AC, _Type<IDirect3D7*>, lpD3D);
    declhook(0x6830C8, _Type<IDirect3DDevice7*>, lpD3DDev);
    declhook(0x6830CC, _Type<IDirectDrawSurface7*>, lpdsRend);

    declhook(0x683130, _Type<gfxInterface>, gfxInterfaces);
    declhook(0x6844C0, _Type<unsigned int>, gfxInterfaceCount);

    declhook(0x6844C8, _Type<int>, gfxInterfaceChoice);

    declhook(0x6844B0, _Type<int>, gfxMinScreenWidth);
    declhook(0x6844CC, _Type<int>, gfxMinScreenHeight);

    declhook(0x6844FC, _Type<int>, gfxMaxScreenWidth);
    declhook(0x6844D8, _Type<int>, gfxMaxScreenHeight);

    declhook(0x6B165C, _Type<int>, gfxTexQuality);
    declhook(0x6857D0, _Type<int>, gfxTexReduceSize);

    declhook(0x6B0454, _Type<uint>, mmCpuSpeed);

    declhook(0x6830F4, _Type<float>, window_fWidth);
    declhook(0x683120, _Type<float>, window_fHeight);

    declhook(0x683128, _Type<int>, window_iWidth);
    declhook(0x683100, _Type<int>, window_iHeight);

    declhook(0x6830E4, _Type<int>, window_ZDepth);
    declhook(0x6830F8, _Type<int>, window_ColorDepth);

    declhook(0x6830EC, _Type<int>, window_X);
    declhook(0x683110, _Type<int>, window_Y);

    // Class definitions

    /*
        gfxInterface
    */
    enum gfxDeviceType {
        Software = 0,    // Software (No 3D Video Card)
        Hardware = 1,    // Hardware (3D Video Card)
        HardwareWithTnL = 2     // Hardware (3D Video Card With T&L)
    };

    enum gfxDepthFlags {
        Depth16 = 0x400,
        Depth24 = 0x200,
        Depth32 = 0x100
    };

    struct gfxResData {
        unsigned short ScreenWidth;
        unsigned short ScreenHeight;
        unsigned short ColorDepth;
        unsigned short Flags; // = ((ColorDepth == 16) + 6)
    };

    struct gfxInterface {
        /*0x00*/ GUID GUID;
        /*0x10*/ char Name[64];

        /*0x50*/ unsigned int DeviceCaps;

        /*0x54*/ gfxDeviceType DeviceType;

        /*0x58*/ unsigned int ResolutionCount;   // Max of 64 resolutions
        /*0x5C*/ unsigned int ResolutionChoice;

        /*0x60*/ gfxDepthFlags AcceptableDepths;  // Used to check if mmResolution::Depth is allowed

        /*0x64*/ unsigned int AvailableMemory;
        /*0x68*/ unsigned int VendorID;
        /*0x6C*/ unsigned int DeviceID;

        /*0x70*/ gfxResData Resolutions[64];
    };

    /*
      gfxPipeline
    */
    class gfxPipeline {
    private:
        static hook::Type<gfxViewport*> m_Viewport;
        static hook::Type<gfxViewport*> VP;
        static hook::Type<gfxViewport*> OrthoVP;
    private:
        static void luaStartFade(Vector4 color, float time) {
            gfxPipeline::StartFade(color.PackColorBGRA(), time);
        }

        static void luaSetFade(Vector4 color) {
            gfxPipeline::SetFade(color.PackColorBGRA());
        }
    public:
        static int GetWidth() {
            return window_iWidth.get();
        }

        static int GetHeight() {
            return window_iHeight.get();
        }

        static float GetFWidth() {
            return window_fWidth.get();
        }

        static float GetFHeight() {
            return window_fHeight.get();
        }

        static bool SetRenderTarget(gfxTexture const* target) {
            return hook::StaticThunk<0x4AB570>::Call<bool>(target);
        }

        static gfxViewport * CreateViewport() {
            return hook::StaticThunk<0x4A90B0>::Call<gfxViewport*>();
        }

        static void ForceSetViewport(gfxViewport* viewport) {
            hook::StaticThunk<0x4B2EE0>::Call<void>(viewport);
        }

        static gfxViewport* GetMainViewport()
        {
            return gfxPipeline::VP.get();
        }

        static gfxViewport* GetCurrentViewport()
        {
            return gfxPipeline::m_Viewport.get();
        }

        static gfxViewport* GetOrthoViewport()
        {
            return gfxPipeline::OrthoVP.get();
        }

        static void CopyBitmap(int destX, int destY, gfxBitmap* bitmap, int srcX, int srcY, int width, int height, bool srcColorKey) {
            hook::StaticThunk<0x4AB4C0>::Call<void>(destX, destY, bitmap, srcX, srcY, width, height, srcColorKey);
        }

        //static void CopyClippedBitmap(int destX, int destY, gfxBitmap* bitmap, int srcX, int srcY, int width, int height)

        static void StartFade(uint color, float time) {
            hook::StaticThunk<0x4B2CE0>::Call<void>(color, time);
        }

        static void SetFade(uint color) {
            hook::StaticThunk<0x4B2D20>::Call<void>(color);
        }

        static gfxImage* CreateReadbackImage() {
            return hook::StaticThunk<0x4AAC90>::Call<gfxImage*>();
        }

        static void Readback(gfxImage* image, int a1 = 0, int a2 = 0, int a3 = 0, int a4 = 0) {
            hook::StaticThunk<0x4AACC0>::Call<void>(image, a1, a2, a3, a4);
        }

        static void BindLua(LuaState L) {
            LuaBinding(L).beginClass<gfxPipeline>("gfxPipeline")
                .addStaticProperty("OrthoViewport", &GetOrthoViewport)
                .addStaticProperty("MainViewport", &GetMainViewport)
                .addStaticProperty("CurrentViewport", &GetCurrentViewport)
                .addStaticFunction("SetFade", &luaSetFade)
                .addStaticFunction("StartFade", &luaStartFade)
                .addStaticFunction("CopyBitmap", &CopyBitmap)
                .addStaticProperty("Width", &GetFWidth)
                .addStaticProperty("Height", &GetFHeight)
                .endClass();
        }
    };
}