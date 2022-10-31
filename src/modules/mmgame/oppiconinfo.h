#pragma once

namespace MM2
{
    // Forward declarations
    struct OppIconInfo;

    // External declarations
    extern class gfxBitmap;
    extern class Matrix34;

    // Class definitions
	struct OppIconInfo {
    private:
        // lua helpers
        LPCSTR GetText() const
        {
            return Text;
        }

        void SetText(LPCSTR text)
        {
            strncpy_s(Text, text, sizeof(Text));
        }
    public:
        unsigned int Color;
        bool Enabled;
        Matrix34* MatrixPtr;
        int IconIndex;
        char Text[16];
        gfxBitmap* Bitmap;
        float Scale;
    public:
        static void BindLua(LuaState L) {
            LuaBinding(L).beginClass<OppIconInfo>("OppIconInfo")
                .addVariable("Color", &OppIconInfo::Color)
                .addVariable("Enabled", &OppIconInfo::Enabled)
                .addVariable("IconIndex", &OppIconInfo::IconIndex)
                .addProperty("Text", &GetText, &SetText)
                .addVariable("Bitmap", &OppIconInfo::Bitmap)
                .addVariable("Scale", &OppIconInfo::Scale)
                .endClass();
        }
    };
}

