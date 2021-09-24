#pragma once

namespace MM2
{
    // Forward declarations
    class gfxImage;

    // External declarations


    // Class definitions
    class gfxImage {
    public:
        enum gfxImageFormat {
            NONE = 0,
            ARGB_8888 = 1,
            RGB_0888  = 2,
            ARGB_1555 = 3,
            RGB_0555  = 4,
            Palette8  = 5,
            Palette4  = 6,
        };


        uint16_t Width;
        uint16_t Height;
        uint16_t Size;
        uint8_t Type;
        uint8_t PaletteType;
        uint32_t TexEnv;
        void *pImageData;
        void *pPaletteData;
        uint32_t RefCount;
        gfxImage *Next;

        void Scale(int a1, int a2)                          { hook::Thunk<0x4AEDC0>::Call<void>(this, a1, a2); }

        static gfxImage * Create(int width, int height, gfxImageFormat format, gfxImageFormat paletteType, int mipCount)
                                                            { return hook::StaticThunk<0x4AE920>::Call<gfxImage*>(width, height, format, paletteType, mipCount); }
    };

    // Lua initialization

}