#pragma once

namespace MM2
{
    // Forward declarations
    class gfxPipeline;
    struct gfxInterface;

    // External declarations


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
    public:
        static bool SetRenderTarget(gfxTexture const* target) {
            return hook::StaticThunk<0x4AB570>::Call<bool>(target);
        }

        static gfxViewport * CreateViewport() {
            return hook::StaticThunk<0x4A90B0>::Call<gfxViewport*>();
        }

        static void ForceSetViewport(gfxViewport* viewport) {
            hook::StaticThunk<0x4B2EE0>::Call<void>(viewport);
        }

        static void CopyBitmap(int destX, int destY, gfxBitmap* bitmap, int srcX, int srcY, int width, int height, bool srcColorKey) {
            hook::StaticThunk<0x4AB4C0>::Call<void>(destX, destY, bitmap, srcX, srcY, width, height, srcColorKey);
        }

        static void StartFade(uint color, float time) {
            hook::StaticThunk<0x4B2CE0>::Call<void>(color, time);
        }

        static void SetFade(uint color) {
            hook::StaticThunk<0x4B2D20>::Call<void>(color);
        }
    };

    // Lua initialization

}