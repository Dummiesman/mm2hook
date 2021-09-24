#pragma once
#include "gamemulti.h"

namespace MM2
{
    // Forward declarations
    class mmMultiRace;

    // External declarations


    // Class definitions
    class mmMultiRace : public mmGameMulti
    {
    private:
        byte _buffer[0xB208 - sizeof(asNode) - 0x4];
    public:
        ANGEL_ALLOCATOR

        AGE_API mmMultiRace(void) {
            scoped_vtable x(this);
            hook::Thunk<0x428260>::Call<void>(this);
        };

        virtual AGE_API ~mmMultiRace(void) {
            scoped_vtable x(this);
            hook::Thunk<0x4282A0>::Call<void>(this);
        };

        /*
            asNode virtuals
        */

        virtual AGE_API void Reset(void) override           { hook::Thunk<0x428AD0>::Call<void>(this); };

        /*
            mmGame overrides
        */

        virtual AGE_API int Init() override                 { return hook::Thunk<0x428340>::Call<int>(this); };
        virtual AGE_API void InitMyPlayer() override        { hook::Thunk<0x428430>::Call<void>(this); };
        virtual AGE_API void InitGameObjects() override     { hook::Thunk<0x4284F0>::Call<void>(this); };
        virtual AGE_API void InitHUD() override             { hook::Thunk<0x4284A0>::Call<void>(this); };
        virtual AGE_API void UpdateGameInput(int a1) override 
                                                            { hook::Thunk<0x428BA0>::Call<void>(this, a1); }
        virtual AGE_API void UpdateGame() override          { hook::Thunk<0x428BE0>::Call<void>(this); }
        virtual AGE_API void SwitchState(int a1) override   { hook::Thunk<0x429440>::Call<void>(this, a1); };
        virtual AGE_API void* GetWaypoints() override       { return hook::Thunk<0x429D80>::Call<void*>(this); };

        /*
            mmGameMulti overrides
        */
        virtual AGE_API void InitNetworkPlayers() override  { hook::Thunk<0x428700>::Call<void>(this); };
        virtual AGE_API void SystemMessage(void* msg) override 
                                                            { hook::Thunk<0x429450>::Call<void>(this, msg); };
        virtual AGE_API void GameMessage(void* msg) override 
                                                            { hook::Thunk<0x429560>::Call<void>(this, msg); };

        static void BindLua(LuaState L) {
            LuaBinding(L).beginExtendClass<mmMultiRace, mmGameMulti>("mmMultiRace")
                .endClass();
        }
    };

    ASSERT_SIZEOF(mmMultiRace, 0xB208);
}