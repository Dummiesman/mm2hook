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
    public:

        /*
            asNode virtuals
        */

        virtual AGE_API void Update(void) override          { hook::Thunk<0x4ED3C0>::Call<void>(this); };

        /*
            uiWidget virtuals
        */

        virtual AGE_API void Action(eqEvent event)          { hook::Thunk<0x4ED4C0>::Call<void>(this, event); }
        virtual AGE_API void Switch(int a1)                 { hook::Thunk<0x4ED430>::Call<void>(this, a1); }
        
        /*
            uiLabel members
        */
        AGE_API void SetText(char* text) {
            hook::Thunk<0x4ED3A0>::Call<void>(this, text);
        }

        AGE_API void SetBlink(bool blink) {
            hook::Thunk<0x4ED400>::Call<void>(this, blink);
        }

        static void BindLua(LuaState L) {
            LuaBinding(L).beginExtendClass<UILabel, uiWidget>("UILabel")
                .endClass();
        }
    };
}