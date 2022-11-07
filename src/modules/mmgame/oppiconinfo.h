#pragma once
#include <modules\vector.h>

namespace MM2
{
    // Forward declarations
    struct OppIconInfo;

    // External declarations
    extern class gfxBitmap;
    extern class Matrix34;

    // Class definitions
	struct OppIconInfo {
    public:
        unsigned int Color;
        bool Enabled;
        Matrix34* MatrixPtr;
        int IconIndex;
        char Text[16];
        gfxBitmap* Bitmap;
        float Scale;
    public:
        LPCSTR GetText() const
        {
            return Text;
        }

        void SetText(LPCSTR text)
        {
            strncpy_s(Text, text, sizeof(Text));
        }

        Vector4 GetColor() const
        {
            Vector4 vec;
            vec.UnpackColorBGRA(this->Color);
            return vec;
        }

        void SetColor(const Vector4& color)
        {
            this->Color = color.PackColorBGRA();
        }

        static void BindLua(LuaState L) {
            LuaBinding(L).beginClass<OppIconInfo>("OppIconInfo")
                .addProperty("Color", &GetColor, &SetColor)
                .addVariable("Enabled", &OppIconInfo::Enabled)
                .addVariable("IconIndex", &OppIconInfo::IconIndex)
                .addProperty("Text", &GetText, &SetText)
                .addVariable("Bitmap", &OppIconInfo::Bitmap)
                .addVariable("Scale", &OppIconInfo::Scale)
                .endClass();
        }
    };
}

