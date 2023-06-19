#pragma once
#include "widget.h"

namespace MM2
{
    // Forward declarations
    class UITextRoller2;

    // External declarations	

    // Class definitions
    class UITextRoller2 : public uiWidget {
	private:
		char _buffer[0xC8];
    public:

        /*
            asNode virtuals
        */

        virtual AGE_API void Cull(void) override            { hook::Thunk<0x4EA0D0>::Call<void>(this); };
        virtual AGE_API void Update(void) override          { hook::Thunk<0x4E9BA0>::Call<void>(this); };

        /*
            uiWidget virtuals
        */

        virtual AGE_API void Action(eqEvent event)          { hook::Thunk<0x4E9CA0>::Call<void>(this, event); }
        virtual AGE_API void Switch(int a1)                 { hook::Thunk<0x4E9C00>::Call<void>(this, a1); }
        

        static void BindLua(LuaState L) {
            LuaBinding(L).beginExtendClass<UITextRoller2, uiWidget>("UITextRoller2")
                .endClass();
        }

    };
}