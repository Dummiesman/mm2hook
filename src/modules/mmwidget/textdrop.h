#pragma once
#include "widget.h"

namespace MM2
{
    // Forward declarations
    class UITextDropdown;

    // External declarations	

    // Class definitions
    class UITextDropdown : public uiWidget {
	private:
		char _buffer[0x94];
    protected:
        static hook::Field<0xAC, int*> _pValue;
    private:
        void AssignStringLua(LPCSTR cstring)
        {
            string str = string(cstring);
            this->AssignString(str);
        }
    public:

        /*
            asNode virtuals
        */

        virtual AGE_API void Cull(void) override            { hook::Thunk<0x4E8830>::Call<void>(this); };
        virtual AGE_API void Update(void) override          { hook::Thunk<0x4E8280>::Call<void>(this); };

        /*
            uiWidget virtuals
        */

        virtual AGE_API void Action(eqEvent event)          { hook::Thunk<0x4E85D0>::Call<void>(this, event); }
        virtual AGE_API void CaptureAction(eqEvent event)   { hook::Thunk<0x4E8300>::Call<void>(this, event); }
        virtual AGE_API void Switch(int a1)                 { hook::Thunk<0x4E8680>::Call<void>(this, a1); }
        virtual AGE_API float GetScreenHeight(void)         { return hook::Thunk<0x4E8180>::Call<float>(this); }
        
        /*
            UITextDropdown members
        */
        void AssignString(string str)                       { hook::Thunk<0x4E81A0>::Call<void>(this, str); }
        void SetDisabledMask(int mask)                      { hook::Thunk<0x4E8800>::Call<void>(this, mask); }

        int GetSelectedIndex() const
        {
            return *_pValue.get(this);
        }

        void SetSelectedIndex(int index)
        {
            *_pValue.get(this) = index;
        }

        static void BindLua(LuaState L) {
            LuaBinding(L).beginExtendClass<UITextDropdown, uiWidget>("UITextDropdown")
                .addProperty("SelectedIndex", &GetSelectedIndex, &SetSelectedIndex)
                .addFunction("AssignString", &AssignStringLua)
                .addFunction("SetDisabledMask", &SetDisabledMask)
                .endClass();
        }

    };
}