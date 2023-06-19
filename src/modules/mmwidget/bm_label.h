#pragma once
#include "widget.h"

namespace MM2
{
    // Forward declarations
    class UIBMLabel;

    // External declarations


    // Class definitions
    class UIBMLabel : public uiWidget {
    private:
        int dword_6c;
        gfxBitmap* curBitmap;
        int curBitmapWidth;
        int curBitmapHeight;
        int* pBitmapIndex;
        int lastBitmapIndex;
        int bitmapIndex; // used if pBitmapIndex is nullptr on Init
        string bitmapNames;
        int dword_90;
        int dword_94;
    public:
        /*
            asNode virtuals
        */

        virtual AGE_API void Update(void) override          { hook::Thunk<0x4ED7F0>::Call<void>(this); };
        virtual AGE_API void Cull(void) override            { hook::Thunk<0x4ED820>::Call<void>(this); };

        static void BindLua(LuaState L) {
            LuaBinding(L).beginExtendClass<UIBMLabel, uiWidget>("UIBMLabel")
                .endClass();
        }
    };
}