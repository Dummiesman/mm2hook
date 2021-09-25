#pragma once
#include "widget.h"

namespace MM2
{
    // Forward declarations
    class UIIcon;

    // External declarations


    // Class definitions
    class UIIcon : public uiWidget {
        Vector2 Position;
        gfxBitmap *pBitmap;
        datCallback Callback;
    };
}