#pragma once
#include "widget.h"

namespace MM2
{
    // Forward declarations
    class UIIcon;

    // External declarations


    // Class definitions
    class UIIcon : public uiWidget {
    private:
        Vector2 Position;
        gfxBitmap *pBitmap;
        datCallback Callback;
    public:
        /*
            asNode virtuals
        */

        virtual AGE_API void Update(void) override          { hook::Thunk<0x4ED9F0>::Call<void>(this); };
        virtual AGE_API void Cull(void) override            { hook::Thunk<0x4EDA10>::Call<void>(this); };

        /*
            uiWidget virtuals
        */

        virtual AGE_API void Switch(int a1)                 { hook::Thunk<0x4ED9B0>::Call<void>(this, a1); }

        /*
            uiIcon members
        */

        AGE_API void LoadBitchmap(LPCSTR name)              { hook::Thunk<0x4ED920>::Call<void>(this, name); }
        AGE_API void LoadBitmap(LPCSTR name)                { hook::Thunk<0x4ED930>::Call<void>(this, name); }

        static void BindLua(LuaState L) {
            LuaBinding(L).beginExtendClass<UIIcon, uiWidget>("UIIcon")
                .addFunction("LoadBitmap", &LoadBitmap)
                .addFunction("LoadBitchmap", &LoadBitchmap)
                .endClass();
        }
    };
}