#pragma once
#include "gamesingle.h"

namespace MM2
{
    // Forward declarations
    class mmSingleStunt;

    // External declarations


    // Class definitions
    class mmSingleStunt : public mmGameSingle
    {
    private:
        byte _buffer[0x7970];
    public:
        ANGEL_ALLOCATOR

        AGE_API mmSingleStunt(void) {
            scoped_vtable x(this);
            hook::Thunk<0x415B00>::Call<void>(this);
        };

        virtual AGE_API ~mmSingleStunt(void) {
            scoped_vtable x(this);
            hook::Thunk<0x415BC0>::Call<void>(this);
        };

        /*
            asNode virtuals
        */

        virtual AGE_API void Update(void) override          { hook::Thunk<0x416970>::Call<void>(this); };
        virtual AGE_API void Reset(void) override           { hook::Thunk<0x416760>::Call<void>(this); };

        /*
            mmGame overrides
        */

        virtual AGE_API int Init() override                 { return hook::Thunk<0x415C40>::Call<int>(this); };
        virtual AGE_API void InitMyPlayer() override        { hook::Thunk<0x415D40>::Call<void>(this); };
        virtual AGE_API void InitOtherPlayers() override    { hook::Thunk<0x416720>::Call<void>(this); };
        virtual AGE_API void InitGameObjects() override     { hook::Thunk<0x416450>::Call<void>(this); };
        virtual AGE_API void InitHUD() override             { hook::Thunk<0x415DB0>::Call<void>(this); };
        virtual AGE_API void UpdateGameInput(int a1) override 
                                                            { hook::Thunk<0x416980>::Call<void>(this, a1); }
        virtual AGE_API void UpdateDebugKeyInput(int a1) override
                                                            { hook::Thunk<0x41ABF0>::Call<void>(this, a1); }
        virtual AGE_API void UpdateGame() override          { hook::Thunk<0x416990>::Call<void>(this); }
        virtual AGE_API void NextRace() override            { hook::Thunk<0x41A490>::Call<void>(this); };
        virtual AGE_API void HitWaterHandler() override     { hook::Thunk<0x416920>::Call<void>(this); };
        virtual AGE_API void SwitchState(int a1) override   { hook::Thunk<0x41A460>::Call<void>(this, a1); };

        static void BindLua(LuaState L) {
            LuaBinding(L).beginExtendClass<mmSingleStunt, mmGameSingle>("mmSingleStunt")
                .endClass();
        }
    };

    //ASSERT_SIZEOF(mmSingleStunt, 0x7970); Don't know size of base classes yet
}