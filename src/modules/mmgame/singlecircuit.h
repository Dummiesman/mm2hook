#pragma once
#include "gamesingle.h"

namespace MM2
{
    // Forward declarations
    class mmSingleCircuit;

    // External declarations


    // Class definitions
    class mmSingleCircuit : public mmGameSingle
    {
    private:
        byte _buffer[0x7720 - sizeof(asNode) - 0x4];
    public:
        ANGEL_ALLOCATOR

        AGE_API mmSingleCircuit(void) {
            scoped_vtable x(this);
            hook::Thunk<0x41C4A0>::Call<void>(this);
        };

        virtual AGE_API ~mmSingleCircuit(void) {
            scoped_vtable x(this);
            hook::Thunk<0x41C4E0>::Call<void>(this);
        };

        /*
            asNode virtuals
        */

        virtual AGE_API void Update(void) override          { hook::Thunk<0x41CB80>::Call<void>(this); };
        virtual AGE_API void Reset(void) override           { hook::Thunk<0x41CA50>::Call<void>(this); };

        /*
            mmGame overrides
        */

        virtual AGE_API int Init() override                 { return hook::Thunk<0x41C590>::Call<int>(this); };
        virtual AGE_API void InitMyPlayer() override        { hook::Thunk<0x41C6B0>::Call<void>(this); };
        virtual AGE_API void InitOtherPlayers() override    { hook::Thunk<0x41CA40>::Call<void>(this); };
        virtual AGE_API void InitGameObjects() override     { hook::Thunk<0x41C770>::Call<void>(this); };
        virtual AGE_API void InitHUD() override             { hook::Thunk<0x41C720>::Call<void>(this); };
        virtual AGE_API void UpdateGameInput(int a1) override 
                                                            { hook::Thunk<0x41CB90>::Call<void>(this, a1); }
        virtual AGE_API void UpdateDebugKeyInput(int a1) override
                                                            { hook::Thunk<0x41F970>::Call<void>(this, a1); }
        virtual AGE_API void UpdateGame() override          { hook::Thunk<0x41CEE0>::Call<void>(this); }
        virtual AGE_API void NextRace() override            { hook::Thunk<0x41DBC0>::Call<void>(this); };
        virtual AGE_API void HitWaterHandler() override     { hook::Thunk<0x41CAA0>::Call<void>(this); };
        virtual AGE_API void SwitchState(int a1) override   { hook::Thunk<0x41D690>::Call<void>(this, a1); };
        virtual AGE_API void* GetWaypoints() override       { return hook::Thunk<0x41DF20>::Call<void*>(this); };

        static void BindLua(LuaState L) {
            LuaBinding(L).beginExtendClass<mmSingleCircuit, mmGameSingle>("mmSingleCircuit")
                .endClass();
        }
    };

    ASSERT_SIZEOF(mmSingleCircuit, 0x7720);
}