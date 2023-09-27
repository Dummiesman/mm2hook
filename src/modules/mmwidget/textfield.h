#pragma once
#include "widget.h"
#include <modules\mmeffects\mmtext.h>

namespace MM2
{
    // Forward declarations
    class UITextField;

    // External declarations	

    // Class definitions
    class UITextField : public uiWidget {
	private:
		char _buffer[0x178];
    private:
        static hook::Field<0x74, mmTextNode*> _textNode;
    public:

        /*
            asNode virtuals
        */

        virtual AGE_API void Update(void) override          { hook::Thunk<0x4E7430>::Call<void>(this); };

        /*
            uiWidget virtuals
        */

        virtual AGE_API void Action(eqEvent event)          { hook::Thunk<0x4E6BB0>::Call<void>(this, event); }
        virtual AGE_API void CaptureAction(eqEvent event)   { hook::Thunk<0x4E6BA0>::Call<void>(this, event); }
        virtual AGE_API void Switch(int a1)                 { hook::Thunk<0x4E6E00>::Call<void>(this, a1); }
        
        /*
            UITextDropdown members
        */
        void SetText(LPCSTR str)                            { hook::Thunk<0x4E6B20>::Call<void>(this, str); }
        void SetField(LPCSTR str)                            { hook::Thunk<0x4E6B40>::Call<void>(this, str); }

        static void BindLua(LuaState L) {
            LuaBinding(L).beginExtendClass<UITextField, uiWidget>("UITextField")
                .addFunction("SetText", &SetText)
                .addFunction("SetField", &SetField)
                .endClass();
        }

    };
}