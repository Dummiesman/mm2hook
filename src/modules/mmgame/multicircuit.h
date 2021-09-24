#pragma once
#include "gamemulti.h"

namespace MM2
{
    // Forward declarations
    class mmMultiCircuit;

    // External declarations


    // Class definitions
    class mmMultiCircuit : public mmGameMulti
    {
    private:
        byte _buffer[0xB208 - sizeof(asNode) - 0x4];
    public:
        ANGEL_ALLOCATOR

        AGE_API mmMultiCircuit(void) {
            scoped_vtable x(this);
            hook::Thunk<0x421B90>::Call<void>(this);
        };

        virtual AGE_API ~mmMultiCircuit(void) {
            scoped_vtable x(this);
            hook::Thunk<0x421BC0>::Call<void>(this);
        };

        /*
            asNode virtuals
        */

        virtual AGE_API void Reset(void) override           { hook::Thunk<0x422470>::Call<void>(this); };

        /*
            mmGame overrides
        */

        virtual AGE_API int Init() override                 { return hook::Thunk<0x421C60>::Call<int>(this); };
        virtual AGE_API void InitMyPlayer() override        { hook::Thunk<0x421DD0>::Call<void>(this); };
        virtual AGE_API void InitGameObjects() override     { hook::Thunk<0x421E90>::Call<void>(this); };
        virtual AGE_API void InitHUD() override             { hook::Thunk<0x421E40>::Call<void>(this); };
        virtual AGE_API void UpdateGameInput(int a1) override 
                                                            { hook::Thunk<0x422530>::Call<void>(this, a1); }
        virtual AGE_API void UpdateDebugKeyInput(int a1) override 
                                                            { hook::Thunk<0x422DA0>::Call<void>(this, a1); }
        virtual AGE_API void UpdateGame() override          { hook::Thunk<0x422570>::Call<void>(this); }
        virtual AGE_API void SwitchState(int a1) override   { hook::Thunk<0x422D90>::Call<void>(this, a1); };
        virtual AGE_API void* GetWaypoints() override       { return hook::Thunk<0x4236A0>::Call<void*>(this); };

        /*
            mmGameMulti overrides
        */
        virtual AGE_API void InitNetworkPlayers() override  { hook::Thunk<0x4220A0>::Call<void>(this); };
        virtual AGE_API void SystemMessage(void* msg) override 
                                                            { hook::Thunk<0x422DB0>::Call<void>(this, msg); };
        virtual AGE_API void GameMessage(void* msg) override 
                                                            { hook::Thunk<0x422EC0>::Call<void>(this, msg); };

        static void BindLua(LuaState L) {
            LuaBinding(L).beginExtendClass<mmMultiCircuit, mmGameMulti>("mmMultiCircuit")
                .endClass();
        }
    };

    ASSERT_SIZEOF(mmMultiCircuit, 0xB208);
}