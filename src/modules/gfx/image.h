#pragma once

namespace MM2
{
    // Forward declarations
    class gfxImage;

    // External declarations


    // Class definitions
    class gfxImage {
    public:
        static hook::Type<unsigned char[768]> sm_CharSet;

        enum class gfxImageFormat : uint8_t {
            none = 0,
            rif8888 = 1,
            rif888 = 2,
            rif5551 = 3,
            rif555 = 4,
            rif8 = 5,
            rif4 = 6,
        };


        uint16_t Width;
        uint16_t Height;
        uint16_t Stride;
        gfxImageFormat Type;
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