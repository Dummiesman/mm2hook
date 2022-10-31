#pragma once
#include <modules\node\node.h>
#include <modules\gfx\texture.h>

namespace MM2
{
    // Forward declarations
    class mmNumberFont;
    class mmNumber;

    // External declarations


    // Class definitions
    class mmNumberFont {
    public:
        HFONT Font;
        const char* CharacterSet;
        int NumCharacters;
        gfxBitmap** CharacterBitmaps;
    public:
        ANGEL_ALLOCATOR

        mmNumberFont(const char * charSet) {
            hook::Thunk<0x5332B0>::Call<void>(this, charSet);
        }

        ~mmNumberFont() {
            hook::Thunk<0x5332E0>::Call<void>(this);
        }

        void LoadFont(LPCSTR faceName, int size, unsigned int color)  { hook::Thunk<0x533361>::Call<void>(this, faceName, size, color); }
        void LoadLocFont(LPCSTR faceName, LocString* locStr, int screenSize, unsigned int color)
                                                                      { hook::Thunk<0x5333B0>::Call<void>(this, faceName, locStr, screenSize, color); }

        static void BindLua(LuaState L) {
            LuaBinding(L).beginClass<mmNumberFont>("mmNumberFont")
                .addFactory([](const char * charSet) { return new mmNumberFont(charSet); })
                .addFunction("LoadFont", &LoadFont)
                .addFunction("LoadLocFont", &LoadLocFont)
                .endClass();
        }
    };

    class mmNumber : public asNode {
    private:
        float XPosition;
        float YPosition;
        mmNumberFont* Font;
        char String[80];
    public:
        AGE_API void Init(mmNumberFont* font, float x, float y) { hook::Thunk<0x533450>::Call<void>(this, font, x, y); }
        AGE_API void SetString(LPCSTR str)                      { hook::Thunk<0x533470>::Call<void>(this, str); }

        static void BindLua(LuaState L) {
            LuaBinding(L).beginExtendClass<mmNumber, asNode>("mmNumber")
                .addFunction("Init", &Init)
                .addFunction("SetString", &SetString)
                .endClass();
        }
    };

    ASSERT_SIZEOF(mmNumber, 0x74);
}