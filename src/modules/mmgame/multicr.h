#pragma once
#include "gamemulti.h"

namespace MM2
{
    // Forward declarations
    class mmMultiCR;

    // External declarations


    // Class definitions
    class mmMultiCR : public mmGameMulti
    {
    private:
        byte _buffer[0xB2A8 - sizeof(asNode) - 0x4];
    public:
        ANGEL_ALLOCATOR

        AGE_API mmMultiCR(void) {
            scoped_vtable x(this);
            hook::Thunk<0x4236E0>::Call<void>(this);
        };

        virtual AGE_API ~mmMultiCR(void) {
            scoped_vtable x(this);
            hook::Thunk<0x4237C0>::Call<void>(this);
        };

        /*
            asNode virtuals
        */

        virtual AGE_API void Reset(void) override           { hook::Thunk<0x424440>::Call<void>(this); };

        /*
            mmGame overrides
        */

        virtual AGE_API int Init() override                 { return hook::Thunk<0x423870>::Call<int>(this); };
        virtual AGE_API void InitMyPlayer() override        { hook::Thunk<0x4239F0>::Call<void>(this); };
        virtual AGE_API void InitGameObjects() override     { hook::Thunk<0x423C80>::Call<void>(this); };
        virtual AGE_API void InitHUD() override             { hook::Thunk<0x423B00>::Call<void>(this); };
        virtual AGE_API void UpdateGameInput(int a1) override 
                                                            { hook::Thunk<0x424F80>::Call<void>(this, a1); }
        virtual AGE_API void UpdateDebugKeyInput(int a1) override 
                                                            { hook::Thunk<0x4272D0>::Call<void>(this, a1); }
        virtual AGE_API void UpdateGame() override          { hook::Thunk<0x424F90>::Call<void>(this); }
        virtual AGE_API void HitWaterHandler() override     { hook::Thunk<0x4253A0>::Call<void>(this); }
        virtual AGE_API void DropThruCityHandler() override { hook::Thunk<0x425460>::Call<void>(this); }
        virtual AGE_API void SwitchState(int a1) override   { hook::Thunk<0x425390>::Call<void>(this, a1); };
        virtual AGE_API void* GetWaypoints() override       { return hook::Thunk<0x427510>::Call<void*>(this); };

        /*
            mmGameMulti overrides
        */
        virtual AGE_API void InitNetworkPlayers() override  { hook::Thunk<0x424090>::Call<void>(this); };
        virtual AGE_API void SystemMessage(void* msg) override 
                                                            { hook::Thunk<0x4261B0>::Call<void>(this, msg); };
        virtual AGE_API void GameMessage(void* msg) override 
                                                            { hook::Thunk<0x426340>::Call<void>(this, msg); };

        static void BindLua(LuaState L) {
            LuaBinding(L).beginExtendClass<mmMultiCR, mmGameMulti>("mmMultiCR")
                .endClass();
        }
    };

    //ASSERT_SIZEOF(mmMultiCR, 0xB2A8);  unknown base class size, TODO
}