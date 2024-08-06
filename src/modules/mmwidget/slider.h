#pragma once
#include "widget.h"

namespace MM2
{
    // Forward declarations
    class UISlider;

    // External declarations


    // Class definitions
    class UISlider : public uiWidget {
    private:
       char _buffer[0x5C];
    protected:
        static hook::Field<0x9C, float*> _pValue;
    public:
        /*
            asNode virtuals
        */

        virtual AGE_API void Update(void) override          { hook::Thunk<0x4EC9A0>::Call<void>(this); };

        /*
            uiWidget virtuals
        */

        virtual AGE_API void Action(eqEvent event)          { hook::Thunk<0x4ECA00>::Call<void>(this, event); }
        virtual AGE_API void Switch(int a1)                 { hook::Thunk<0x4ECAC0>::Call<void>(this, a1); }
        virtual AGE_API void SetPosition(float x, float y)
                                                            { hook::Thunk<0x4EC860>::Call<void>(this, x, y); }
        virtual AGE_API float GetScreenHeight(void)         { return hook::Thunk<0x4ECDA0>::Call<float>(this); }

        float GetValue() const
        {
            return *_pValue.get(this);
        }

        void SetValue(float value)
        {
            *_pValue.get(this) = value;
        }

        static void BindLua(LuaState L) {
            LuaBinding(L).beginExtendClass<UISlider, uiWidget>("UISlider")
                .addProperty("Value", &GetValue, &SetValue)
                .endClass();
        }
    };
}