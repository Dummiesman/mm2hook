#pragma once
#include "widget.h"

namespace MM2
{
    // Forward declarations
    class UILabel;

    // External declarations
    extern class mmTextNode;

    // Class definitions
    class UILabel : public uiWidget {
    public:
        mmTextNode *pTextNode;
        HFONT hFont;
        uint32_t Flags;
        uint32_t State;
        float ElapsedTime;

        AGE_API void SetText(char* text) {
            hook::Thunk<0x4ED3A0>::Call<void>(this, text);
        }

        AGE_API void SetBlink(bool blink) {
            hook::Thunk<0x4ED400>::Call<void>(this, blink);
        }
    };
}