#pragma once
#include <mm2_common.h>

namespace MM2
{
    // Forward declarations
    struct tagPositionEntry;
    class mmPositions;

    // External declarations


    // Class definitions
    struct tagPositionEntry
    {
        char* Message;
        Vector4 PositionRotation;
        int PolyCount;
        int FrameRate;
        int StateChanges;
        int TextureChanges;
    };

    class mmPositions {
    private:
        tagPositionEntry** Positions;
        int Count;
        int Capacity;
    private:
        bool loadLua(const char* path) {
            return this->Load(path) == TRUE;
        }
    public:
        AGE_API mmPositions(void) {
            hook::Thunk<0x52A0B0>::Call<void>(this);
        }

        AGE_API ~mmPositions(void) {
            hook::Thunk<0x52A0C0>::Call<void>(this);
        }

        // mmPositions members
        void Init(int capacity)                                  { hook::Thunk<0x52A0E0>::Call<void>(this, capacity); }
        BOOL Load(const char* path)                              { return hook::Thunk<0x52A110>::Call<BOOL>(this, path); }
        int GetCount() const                                     { return hook::Thunk<0x52A300>::Call<int>(this); }
        Vector4& GetVector4(int index)                           { return hook::Thunk<0x52A310>::Call<Vector4&>(this, index); }
        int GetFrameRate(int index) const                        { return hook::Thunk<0x52A340>::Call<int>(this); }

        //lua
        static void BindLua(LuaState L) {
            LuaBinding(L).beginClass<mmPositions>("mmPositions")
                .addConstructor(LUA_ARGS())
                .addPropertyReadOnly("Count", &GetCount)
                .addFunction("Init", &Init)
                .addFunction("Load", &loadLua)
                .addFunction("GetVector4", &GetVector4)
                .addFunction("GetFrameRate", &GetFrameRate)
                .endClass();
        }
    };
    ASSERT_SIZEOF(mmPositions, 0xC);
}