#pragma once
#include "widget.h"
#include <modules\effects.h>

namespace MM2
{
    // Forward declarations
    class UIButton;

    // External declarations


    // Class definitions
    class UIButton : public uiWidget {
    public:
        uint8_t gap6C[16];
        mmTextNode *pTextNode;
        Card2D *pCard2D;
        uint32_t *dword84;
        uint32_t dword88;
        uint32_t Type;
        uint32_t TextIndex;
        uint32_t Flags;
        datCallback Callback;

        AGE_API void SetType(int type)                      { hook::Thunk<0x4ED140>::Call<void>(this, type); }

        /*
            asNode virtuals
        */

        virtual AGE_API void Update(void) override          { hook::Thunk<0x4A0DB0>::Call<void>(this); };

        /*
            uiWidget virtuals
        */

        virtual AGE_API void Disable(void) override         { hook::Thunk<0x4ED1F0>::Call<void>(this); }
        virtual AGE_API void Enable(void) override          { hook::Thunk<0x4ED1D0>::Call<void>(this); }
        virtual AGE_API void SetReadOnly(BOOL readOnly) override
                                                            { hook::Thunk<0x4ED090>::Call<void>(this, readOnly); }
        virtual AGE_API void Action(eqEvent event) override { hook::Thunk<0x4ED040>::Call<void>(this, event); }
        virtual AGE_API void Switch(int a1) override        { hook::Thunk<0x4ED0D0>::Call<void>(this, a1); }

        static void BindLua(LuaState L) {
            LuaBinding(L).beginExtendClass<UIButton, uiWidget>("UIButton")
                .endClass();
        }
    };
}