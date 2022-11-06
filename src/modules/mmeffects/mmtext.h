#pragma once
#include <modules\node\node.h>

namespace MM2
{
    // Forward declarations
    class mmText;
    class mmTextData;
    class mmTextNode;

    // External declarations


    // Class definitions
    class mmText {
    public:
        BYTE byte0;
        BYTE byte1;
    public:
        AGE_API static void* CreateLocFont(LPCSTR locString, int screenSize)    { return hook::StaticThunk<0x5321C0>::Call<void*>(locString, screenSize); }
        //AGE_API static void DeleteFont(void* font)                              { hook::StaticThunk<0x5322B0>::Call<void>(font); }
    };

    class mmTextData {
    public:
        Vector2 Pos;
        uint32_t Flags;
        HFONT Font;
        char Text[256];
    };

    class mmTextNode : public asNode {
    private:
        Vector2 Pos;
        uint32_t EntryCount;
        uint32_t MaxEntries;
        uint32_t DrawBits;
        mmText dword2C;
        mmTextData *pTextEntries;
        gfxBitmap *Bitmap;
        BOOL bModified;
        uint32_t dword3C;
        uint32_t dword40;
        uint8_t SrcColorKey;
        uint32_t FGColor;
        uint32_t BGColor;
        uint32_t HiglightColor;
    public:

        // last parameter is unused (in thunk call)
        AGE_API void Init(float xPos, float yPos, float width, float height, int maxEntries)
                                                            { hook::Thunk<0x532840>::Call<void>(this, xPos, yPos, width, height, maxEntries, 1); }

        AGE_API int AddText(HFONT font, LPCSTR str, int flags, float xPosition, float yPosition)
                                                            { return hook::Thunk<0x532C70>::Call<int>(this, font, str, flags, xPosition, yPosition); }

        AGE_API void SetBGColor(Vector4& color)             { hook::Thunk<0x532970>::Call<void>(this, &color); }
        AGE_API void SetFGColor(Vector4& color)             { hook::Thunk<0x5329E0>::Call<void>(this, &color); }
        AGE_API void SetHlColor(Vector4& color)             { hook::Thunk<0x532A40>::Call<void>(this, &color); }
        AGE_API int GetEffects(int entryNum) const          { return hook::Thunk<0x532AE0>::Call<int>(this, entryNum); }
        AGE_API void SetString(int entryNum, LPCSTR str)    { hook::Thunk<0x532D50>::Call<void>(this, entryNum, str); }
        AGE_API void SetEffects(int entryNum, int effects)  { hook::Thunk<0x532AB0>::Call<void>(this, entryNum, effects); }
        AGE_API void SetTextPosition(int entryNum, float x, float y)
                                                            { hook::Thunk<0x532C20>::Call<void>(this, entryNum, x, y); }
        
        mmTextData* GetEntry(int entryNum)                  { return (entryNum < 0 || entryNum >= this->MaxEntries) ? nullptr : &this->pTextEntries[entryNum]; }
        int GetNumEntries() const                           { return this->EntryCount; }
        int GetMaxEntries() const                           { return this->MaxEntries; }

        Vector2 GetPosition() const                         { return this->Pos; }
        void SetPosition(Vector2 position)                  { this->Pos = position; this->bModified = true; }

        Vector4 GetBGColor() const
        {
            Vector4 vec;
            vec.UnpackColorRGBA(this->BGColor);
            return vec;
        }

        Vector4 GetFGColor() const
        {
            Vector4 vec;
            vec.UnpackColorRGBA(this->FGColor);
            return vec;
        }

        Vector4 GetHlColor() const
        {
            Vector4 vec;
            vec.UnpackColorRGBA(this->HiglightColor);
            return vec;
        }

        static void BindLua(LuaState L) {
            LuaBinding(L).beginExtendClass<mmTextNode, asNode>("mmTextNode")
                .addProperty("BackgroundColor", &GetBGColor, &SetBGColor)
                .addProperty("ForegroundColor", &GetFGColor, &SetFGColor)
                .addProperty("HighlightColor", &GetHlColor, &SetHlColor)
                .addPropertyReadOnly("NumEntries", &GetNumEntries)
                .addPropertyReadOnly("MaxEntries", &GetMaxEntries)
                .addFunction("Init", &Init)
                .addFunction("SetString", &SetString)
                .addFunction("GetEffects", &GetEffects)
                .addFunction("SetEffects", &SetEffects)
                .addFunction("SetTextPosition", &SetTextPosition)
                .addFunction("GetPosition", &GetPosition)
                .addFunction("SetPosition", &SetPosition)
                .endClass();
        }
    };

    ASSERT_SIZEOF(mmTextNode, 0x54);
}