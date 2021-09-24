#pragma once
#include "game.h"

namespace MM2
{
    // Forward declarations
    class mmGameSingle;

    // External declarations


    // Class definitions
    class mmGameMulti : public mmGame {
    public:
        AGE_API mmGameMulti(void) {
            scoped_vtable x(this);
            hook::Thunk<0x438CC0>::Call<void>(this);
        };

        virtual AGE_API ~mmGameMulti(void) {
            scoped_vtable x(this);
            hook::Thunk<0x438E60>::Call<void>(this);
        };

        /*
            mmGameMulti virtuals
        */
        virtual AGE_API void InitNetworkPlayers()           PURE;
        virtual AGE_API void InitRoster()                   { hook::Thunk<0x43A750>::Call<void>(this); }
        virtual AGE_API void SystemMessage(void*)           PURE;
        virtual AGE_API void GameMessage(void*)             PURE;

        /*
            asNode virtuals
        */

        virtual AGE_API void Update(void) override          { hook::Thunk<0x43A900>::Call<void>(this); };
        virtual AGE_API void Reset(void) override           { hook::Thunk<0x43A5D0>::Call<void>(this); };

        /*
            mmGame overrides
        */

        virtual AGE_API int Init() override                 { return hook::Thunk<0x438F40>::Call<int>(this); };
        virtual AGE_API void InitGameStrings() override     { hook::Thunk<0x439240>::Call<void>(this); };
        virtual AGE_API void InitOtherPlayers() override    { hook::Thunk<0x43A720>::Call<void>(this); };
        virtual AGE_API void UpdateDebugKeyInput(int a1) override 
                                                            { hook::Thunk<0x43ADC0>::Call<void>(this, a1); }
        virtual AGE_API void UpdateGame() override          { hook::Thunk<0x43A980>::Call<void>(this); }
        virtual AGE_API void NextRace() override            { hook::Thunk<0x421B70>::Call<void>(this); };
        virtual AGE_API void HitWaterHandler() override     { hook::Thunk<0x439130>::Call<void>(this); };
        virtual AGE_API void DropThruCityHandler() override { hook::Thunk<0x439230>::Call<void>(this); };
        virtual AGE_API void SendChatMessage(char *a1) override       
                                                            { hook::Thunk<0x43B530>::Call<void*>(this, a1); }
        virtual AGE_API void BeDone(int a1) override        { hook::Thunk<0x43A4D0>::Call<void>(this, a1); };

        static void BindLua(LuaState L) {
            LuaBinding(L).beginExtendClass<mmGameMulti, mmGame>("mmGameMulti")
                .endClass();
        }
    };
}