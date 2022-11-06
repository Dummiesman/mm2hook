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
    private:
        // lua helpers
        LPCSTR luaGetText() const
        {
            return Text;
        }

        void luaSetText(LPCSTR text)
        {
            strncpy_s(Text, text, sizeof(Text));
        }

        Vector4 luaGetColor() const
        {
            Vector4 vec;
            vec.UnpackColorARGB(this->Color);
            return vec;
        }

        void luaSetColor(const Vector4& color)
        {
            this->Color = color.PackColorARGB();
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
                .addProperty("Color", &luaGetColor, &luaSetColor)
                .addVariable("Enabled", &OppIconInfo::Enabled)
                .addVariable("IconIndex", &OppIconInfo::IconIndex)
                .addProperty("Text", &luaGetText, &luaSetText)
                .addVariable("Bitmap", &OppIconInfo::Bitmap)
                .addVariable("Scale", &OppIconInfo::Scale)
                .endClass();
        }
    };
}

