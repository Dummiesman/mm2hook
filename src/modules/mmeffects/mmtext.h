#pragma once
#include <modules\node\node.h>

namespace MM2
{
    // Forward declarations
    class mmText;
    class mmTextData;
    class mmTextNode;

    // External declarations


    // Class definitions
    class mmText {
    public:
        BYTE byte0;
        BYTE byte1;
    };

    class mmTextData {
    public:
        Vector2 Pos;
        uint32_t Flags;
        HFONT Font;
        char Text[256];
    };

    class mmTextNode : public asNode {
        Vector2 Pos;
        uint32_t EntryCount;
        uint32_t MaxEntries;
        uint32_t DrawBits;
        mmText dword2C;
        mmTextData *pTextEntries;
        gfxBitmap *Bitmap;
        BOOL bModified;
        uint32_t dword3C;
        uint32_t dword40;
        uint8_t byte44;
        uint32_t FGColor;
        uint32_t BGColor;
        uint32_t HiglightColor;
    };

    // Lua initialization

}