#pragma once
#include "image.h"

namespace MM2
{
    // Forward declarations

    // External declarations
    
    // Statically available functions
    declhook(0x5CD680, _Type<gfxImage* (*)(gfxImage*, const char*, bool)>, gfxPrepareImage);

    static void gfxImageSetPixel(gfxImage* image, int x, int y, int color, int lineHeight)
    {
        y += lineHeight * (x / image->Width);
        x &= image->Width - 1;
        if (x >= 0 && y >= 0)
        {
            if (x < image->Width && y < image->Height)
            {
                switch (image->Type)
                {
                    case gfxImage::gfxImageFormat::rif8888:
                    {
                        *(DWORD*)((char*)image->pImageData + 4 * x + y * image->Stride) = color;
                        break;
                    }
                    case gfxImage::gfxImageFormat::rif888:
                    {
                        auto pImageData = ((BYTE*)image->pImageData + y * image->Stride + (3 * x));
                        pImageData[0] = color & 0xFF;
                        pImageData[1] = (color >> 8) & 0xFF;
                        pImageData[2] = (color >> 16) & 0xFF;
                        break;
                    }
                    case gfxImage::gfxImageFormat::rif5551:
                    case gfxImage::gfxImageFormat::rif555:
                    {
                        *(WORD*)((char*)image->pImageData + 2 * x + y * image->Stride) = color;
                        break;
                    }
                    case gfxImage::gfxImageFormat::rif8:
                    {
                        *((BYTE*)image->pImageData + y * image->Stride + x) = color;
                        break;
                    }
                    case gfxImage::gfxImageFormat::rif4:
                    {
                        auto pImageData = (char*)image->pImageData + y * image->Stride + (x >> 1);
                        if ((x & 1) != 0)
                            *pImageData = *pImageData & 0xF | (16 * color);
                        else
                            *pImageData ^= (color ^ *pImageData) & 0xF;
                        break;
                    }
                    default:
                        return;
                }
            }
        }
    }

    static void gfxAnnotateImage(gfxImage* image, char const* text) {
        int colWhite = 0xFFFFFFFF;
        int colBlack = 0xFF000000;

        if (image->PaletteType != 0) {
            // overwrite the first couple of colors
            // and replace our color vars with indices
            colWhite = 1;
            colBlack = 0;

            if (image->PaletteType == 1)
            {
                *(DWORD*)image->pPaletteData = 0xFF000000;
                *((DWORD*)image->pPaletteData + 1) = 0xFFFFFFFF;
            }
            else
            {
                *(WORD*)image->pPaletteData = 0x8000;
                *((WORD*)image->pPaletteData + 1) = 0xFFFF;
            }
        }

        //annotate!
        int xOffset = 0;
        while (char i = *text++) {
            auto charIndex = i - 32;
            if (charIndex >= 0 && charIndex < 96)
            {
                for (int y = 0; y < 8; y++) {
                    for (int x = 0; x < 8; x++) {
                        auto charData = gfxImage::sm_CharSet[(8 * charIndex) + y];
                        int color = ((charData & (128 >> x)) == 0) ? colBlack : colWhite;

                        gfxImageSetPixel(image, x + xOffset, y, color, 8);
                        gfxImageSetPixel(image, x + xOffset, image->Height - y - 1, color, -8);
                    }
                }
                xOffset += 8;
            }
        }
    }

    static gfxTexture * gfxGetTexture(const char* a1, bool a2 = true) {
        return hook::StaticThunk<0x4B30F0>::Call<gfxTexture*>(a1, a2);
    }

    // Class definitions

    // Lua initialization

}