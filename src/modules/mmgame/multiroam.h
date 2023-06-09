#pragma once
#include "gamemulti.h"

namespace MM2
{
    // Forward declarations
    class mmMultiRoam;

    // External declarations


    // Class definitions
    class mmMultiRoam : public mmGameMulti
    {
    private:
        byte _buffer[0xB208 - sizeof(asNode) - 0x4];
    public:
        ANGEL_ALLOCATOR

        AGE_API mmMultiRoam(void) {
            scoped_vtable x(this);
            hook::Thunk<0x427530>::Call<void>(this);
        };

        virtual AGE_API ~mmMultiRoam(void) {
            scoped_vtable x(this);
            hook::Thunk<0x427560>::Call<void>(this);
        };

        /*
            asNode virtuals
        */

        virtual AGE_API void Reset(void) override           { hook::Thunk<0x427C50>::Call<void>(this); };

        /*
            mmGame overrides
        */

        virtual AGE_API int Init() override                 { return hook::Thunk<0x427600>::Call<int>(this); };
        virtual AGE_API void InitMyPlayer() override        { hook::Thunk<0x427700>::Call<void>(this); };
        virtual AGE_API void InitGameObjects() override     { hook::Thunk<0x4277A0>::Call<void>(this); };
        virtual AGE_API void InitHUD() override             { hook::Thunk<0x427770>::Call<void>(this); };
        virtual AGE_API void UpdateGameInput(int a1) override 
                                                            { hook::Thunk<0x427CC0>::Call<void>(this, a1); }
        virtual AGE_API void UpdateGame() override          { hook::Thunk<0x427CD0>::Call<void>(this); }
        virtual AGE_API void SwitchState(int a1) override   { hook::Thunk<0x427EB0>::Call<void>(this, a1); };
        virtual AGE_API mmWaypoints* GetWaypoints() override       
                                                            { return hook::Thunk<0x428250>::Call<mmWaypoints*>(this); };

        /*
            mmGameMulti overrides
        */
        virtual AGE_API void InitNetworkPlayers() override  { hook::Thunk<0x427880>::Call<void>(this); };
        virtual AGE_API void SystemMessage(void* msg) override 
                                                            { hook::Thunk<0x427EC0>::Call<void>(this, msg); };
        virtual AGE_API void GameMessage(void* msg) override 
                                                            { hook::Thunk<0x428050>::Call<void>(this, msg); };

        static void BindLua(LuaState L) {
            LuaBinding(L).beginExtendClass<mmMultiRoam, mmGameMulti>("mmMultiRoam")
                .endClass();
        }
    };

    //ASSERT_SIZEOF(mmMultiRoam, 0xB208);  unknown base class size, TODO
}