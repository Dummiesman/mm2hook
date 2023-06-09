#pragma once
#include "gamesingle.h"

namespace MM2
{
    // Forward declarations
    class mmSingleRace;

    // External declarations


    // Class definitions
    class mmSingleRace : public mmGameSingle
    {
    private:
        byte _buffer[0x76F0 - sizeof(asNode) - 0x4];
    public:
        ANGEL_ALLOCATOR

        AGE_API mmSingleRace(void) {
            scoped_vtable x(this);
            hook::Thunk<0x41DF30>::Call<void>(this);
        };

        virtual AGE_API ~mmSingleRace(void) {
            scoped_vtable x(this);
            hook::Thunk<0x41DF60>::Call<void>(this);
        };

        /*
            asNode virtuals
        */

        virtual AGE_API void Update(void) override          { hook::Thunk<0x41E5A0>::Call<void>(this); };
        virtual AGE_API void Reset(void) override           { hook::Thunk<0x41E4F0>::Call<void>(this); };

        /*
            mmGame overrides
        */

        virtual AGE_API int Init() override                 { return hook::Thunk<0x41E010>::Call<int>(this); };
        virtual AGE_API void InitMyPlayer() override        { hook::Thunk<0x41E130>::Call<void>(this); };
        virtual AGE_API void InitOtherPlayers() override    { hook::Thunk<0x41E4E0>::Call<void>(this); };
        virtual AGE_API void InitGameObjects() override     { hook::Thunk<0x41E200>::Call<void>(this); };
        virtual AGE_API void InitHUD() override             { hook::Thunk<0x41E1A0>::Call<void>(this); };
        virtual AGE_API void UpdateGameInput(int a1) override  
                                                            { hook::Thunk<0x41E5B0>::Call<void>(this, a1); }
        virtual AGE_API void UpdateDebugKeyInput(int a1) override 
                                                            { hook::Thunk<0x41F970>::Call<void>(this, a1); }
        virtual AGE_API void UpdateGame() override          { hook::Thunk<0x41E5F0>::Call<void>(this); }
        virtual AGE_API void NextRace() override            { hook::Thunk<0x41F600>::Call<void>(this); };
        virtual AGE_API void HitWaterHandler() override     { hook::Thunk<0x41E530>::Call<void>(this); };
        virtual AGE_API void SwitchState(int a1) override   { hook::Thunk<0x41EDB0>::Call<void>(this, a1); };
        virtual AGE_API mmWaypoints* GetWaypoints() override      
                                                            { return hook::Thunk<0x41F980>::Call<mmWaypoints*>(this); };

        static void BindLua(LuaState L) {
            LuaBinding(L).beginExtendClass<mmSingleRace, mmGameSingle>("mmSingleRace")
                .endClass();
        }
    };

    //ASSERT_SIZEOF(mmSingleRace, 0x76F0); unknown base class size, TODO
}