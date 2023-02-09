#pragma once
#include "gamesingle.h"

namespace MM2
{
    // Forward declarations
    class mmSingleBlitz;

    // External declarations


    // Class definitions
    class mmSingleBlitz : public mmGameSingle
    {
    private:
        byte _buffer[0x7710 - sizeof(asNode) - 0x4];
    public:
        ANGEL_ALLOCATOR

        AGE_API mmSingleBlitz(void) {
            scoped_vtable x(this);
            hook::Thunk<0x41AC00>::Call<void>(this);
        };

        virtual AGE_API ~mmSingleBlitz(void) {
            scoped_vtable x(this);
            hook::Thunk<0x41AC70>::Call<void>(this);
        };

        /*
            asNode virtuals
        */

        virtual AGE_API void Update(void) override          { hook::Thunk<0x41B460>::Call<void>(this); };
        virtual AGE_API void Reset(void) override           { hook::Thunk<0x41B370>::Call<void>(this); };

        /*
            mmGame overrides
        */

        virtual AGE_API int Init() override                 { return hook::Thunk<0x41AD30>::Call<int>(this); };
        virtual AGE_API void InitMyPlayer() override        { hook::Thunk<0x41AE50>::Call<void>(this); };
        virtual AGE_API void InitOtherPlayers() override    { hook::Thunk<0x41B360>::Call<void>(this); };
        virtual AGE_API void InitGameObjects() override     { hook::Thunk<0x41B0D0>::Call<void>(this); };
        virtual AGE_API void InitHUD() override             { hook::Thunk<0x41AEC0>::Call<void>(this); };
        virtual AGE_API void UpdateGameInput(int a1) override 
                                                            { hook::Thunk<0x41B590>::Call<void>(this, a1); }
        virtual AGE_API void UpdateDebugKeyInput(int a1) override
                                                            { hook::Thunk<0x41C480>::Call<void>(this, a1); }
        virtual AGE_API void UpdateGame() override          { hook::Thunk<0x41B5D0>::Call<void>(this); }
        virtual AGE_API void NextRace() override            { hook::Thunk<0x41C080>::Call<void>(this); };
        virtual AGE_API void HitWaterHandler() override     { hook::Thunk<0x41B400>::Call<void>(this); };
        virtual AGE_API void SwitchState(int a1) override   { hook::Thunk<0x41BDB0>::Call<void>(this, a1); };
        virtual AGE_API void* GetWaypoints() override       { return hook::Thunk<0x41C490>::Call<void*>(this); };

        static void BindLua(LuaState L) {
            LuaBinding(L).beginExtendClass<mmSingleBlitz, mmGameSingle>("mmSingleBlitz")
                .endClass();
        }
    };

    //ASSERT_SIZEOF(mmSingleBlitz, 0x7710);  unknown base class size, TODO
}