#pragma once
#include "gamesingle.h"

namespace MM2
{
    // Forward declarations
    class mmSingleRoam;

    // External declarations


    // Class definitions
    class mmSingleRoam : public mmGameSingle
    {
    private:
        byte _buffer[0x76F0];
    public:
        AGE_API mmSingleRoam(void) {
            //scoped_vtable x(this);
            hook::Thunk<0x41F990>::Call<void>(this);
        };

        virtual AGE_API ~mmSingleRoam(void) {
            scoped_vtable x(this);
            hook::Thunk<0x41F9C0>::Call<void>(this);
        };

        /*
            asNode virtuals
        */

        virtual AGE_API void Update(void) override          { hook::Thunk<0x41FCE0>::Call<void>(this); };
        virtual AGE_API void Reset(void) override           { hook::Thunk<0x41FC90>::Call<void>(this); };

        /*
            mmGame overrides
        */

        virtual AGE_API int Init() override                 { return hook::Thunk<0x41FA30>::Call<int>(this); };
        virtual AGE_API void InitMyPlayer() override        { hook::Thunk<0x41FAA0>::Call<void>(this); };
        virtual AGE_API void InitOtherPlayers() override    { hook::Thunk<0x41FC40>::Call<void>(this); };
        virtual AGE_API void InitGameObjects() override     { hook::Thunk<0x41FB40>::Call<void>(this); };
        virtual AGE_API void InitHUD() override             { hook::Thunk<0x41FB10>::Call<void>(this); };
        virtual AGE_API void UpdateGameInput(int a1) override  
                                                            { hook::Thunk<0x41FCF0>::Call<void>(this, a1); }
        virtual AGE_API void UpdateDebugKeyInput(int a1) override 
                                                            { hook::Thunk<0x41FD00>::Call<void>(this, a1); }
        virtual AGE_API void UpdateGame() override          { hook::Thunk<0x433CA0>::Call<void>(this); }
        virtual AGE_API void NextRace() override            { hook::Thunk<0x41FDA0>::Call<void>(this); };
        virtual AGE_API void HitWaterHandler() override     { hook::Thunk<0x41FCD0>::Call<void>(this); };
        virtual AGE_API void SwitchState(int a1) override   { hook::Thunk<0x41FD90>::Call<void>(this, a1); };

        static void BindLua(LuaState L) {
            LuaBinding(L).beginExtendClass<mmSingleRoam, mmGameSingle>("mmSingleRoam")
                .endClass();
        }
    };

    //ASSERT_SIZEOF(mmSingleRoam, 0x76F0); Don't know size of base classes yet
}