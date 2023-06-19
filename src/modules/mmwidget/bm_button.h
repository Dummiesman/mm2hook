#pragma once
#include "widget.h"

namespace MM2
{
    // Forward declarations
    class UIBMButton;

    // External declarations


    // Class definitions
    class UIBMButton : public uiWidget {
    private:
        char _buffer[0x6C];
    public:

        /*
            asNode virtuals
        */

        virtual AGE_API void Cull(void) override            { hook::Thunk<0x4EF2B0>::Call<void>(this); };
        virtual AGE_API void Update(void) override          { hook::Thunk<0x4EF060>::Call<void>(this); };

        /*
            uiWidget virtuals
        */

        virtual AGE_API void Disable(void)                  { hook::Thunk<0x4EF6B0>::Call<void>(this); }
        virtual AGE_API void Enable(void)                   { hook::Thunk<0x4EF690>::Call<void>(this); }
        virtual AGE_API void Action(eqEvent event)          { hook::Thunk<0x4EEF20>::Call<void>(this, event); }
        virtual AGE_API void Switch(int a1)                 { hook::Thunk<0x4EEDF0>::Call<void>(this, a1); }
        virtual AGE_API char * ReturnDescription(void)      { return hook::Thunk<0x4EF1F0>::Call<char *>(this); }
        virtual AGE_API void SetPosition(float x, float y)
                                                            { hook::Thunk<0x4EEB30>::Call<void>(this, x, y); }
        virtual AGE_API float GetScreenHeight(void)         { return hook::Thunk<0x4EF2A0>::Call<float>(this); }
        

        static void BindLua(LuaState L) {
            LuaBinding(L).beginExtendClass<UIBMButton, uiWidget>("UIBMButton")
                .endClass();
        }
    };
}