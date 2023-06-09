#pragma once
#include "gamemulti.h"

namespace MM2
{
    // Forward declarations
    class mmMultiBlitz;

    // External declarations


    // Class definitions
    class mmMultiBlitz : public mmGameMulti
    {
    private:
        byte _buffer[0xB218 - sizeof(asNode) - 0x4];
    public:
        ANGEL_ALLOCATOR

        AGE_API mmMultiBlitz(void) {
            scoped_vtable x(this);
            hook::Thunk<0x41FF30>::Call<void>(this);
        };

        virtual AGE_API ~mmMultiBlitz(void) {
            scoped_vtable x(this);
            hook::Thunk<0x41FF60>::Call<void>(this);
        };

        /*
            asNode virtuals
        */

        virtual AGE_API void Reset(void) override           { hook::Thunk<0x420880>::Call<void>(this); };

        /*
            mmGame overrides
        */

        virtual AGE_API int Init() override                 { return hook::Thunk<0x420000>::Call<int>(this); };
        virtual AGE_API void InitMyPlayer() override        { hook::Thunk<0x420130>::Call<void>(this); };
        virtual AGE_API void InitGameObjects() override     { hook::Thunk<0x420220>::Call<void>(this); };
        virtual AGE_API void InitHUD() override             { hook::Thunk<0x4201A0>::Call<void>(this); };
        virtual AGE_API void UpdateGameInput(int a1) override 
                                                            { hook::Thunk<0x420940>::Call<void>(this, a1); }
        virtual AGE_API void UpdateGame() override          { hook::Thunk<0x420980>::Call<void>(this); }
        virtual AGE_API void SwitchState(int a1) override   { hook::Thunk<0x421250>::Call<void>(this, a1); };
        virtual AGE_API mmWaypoints* GetWaypoints() override       
                                                            { return hook::Thunk<0x421B80>::Call<mmWaypoints*>(this); };

        /*
            mmGameMulti overrides
        */
        virtual AGE_API void InitNetworkPlayers() override  { hook::Thunk<0x4204B0>::Call<void>(this); };
        virtual AGE_API void SystemMessage(void* msg) override 
                                                            { hook::Thunk<0x421260>::Call<void>(this, msg); };
        virtual AGE_API void GameMessage(void* msg) override 
                                                            { hook::Thunk<0x421370>::Call<void>(this, msg); };

        static void BindLua(LuaState L) {
            LuaBinding(L).beginExtendClass<mmMultiBlitz, mmGameMulti>("mmMultiBlitz")
                .endClass();
        }
    };

    //ASSERT_SIZEOF(mmMultiBlitz, 0xB218);  unknown base class size, TODO
}