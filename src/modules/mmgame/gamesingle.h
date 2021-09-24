#pragma once
#include "game.h"

namespace MM2
{
    // Forward declarations
    class mmGameSingle;

    // External declarations


    // Class definitions
    class mmGameSingle : public mmGame {
    public:
        AGE_API mmGameSingle(void) {
            scoped_vtable x(this);
            hook::Thunk<0x433A60>::Call<void>(this);
        };

        virtual AGE_API ~mmGameSingle(void) {
            scoped_vtable x(this);
            hook::Thunk<0x433A90>::Call<void>(this);
        };

        /*
            asNode virtuals
        */

        virtual AGE_API void Update(void) override          { hook::Thunk<0x433C80>::Call<void>(this); };
        virtual AGE_API void Reset(void) override           { hook::Thunk<0x433AC0>::Call<void>(this); };

        /*
            mmGame overrides
        */

        virtual AGE_API int Init() override                 { return hook::Thunk<0x433AA0>::Call<int>(this); };
        virtual AGE_API void InitOtherPlayers() override    { hook::Thunk<0x433AB0>::Call<void>(this); };
        virtual AGE_API void UpdateGameInput(int a1) override  
                                                            { hook::Thunk<0x433C90>::Call<void>(this, a1); }
        virtual AGE_API void UpdateDebugKeyInput(int a1) override 
                                                            { hook::Thunk<0x433F20>::Call<void>(this, a1); }
        virtual AGE_API void UpdateGame() override          { hook::Thunk<0x433CA0>::Call<void>(this); }
        virtual AGE_API void HitWaterHandler() override     { hook::Thunk<0x433C70>::Call<void>(this); };
        virtual AGE_API void* GetWaypoints() override       { return hook::Thunk<0x41ABA0>::Call<void*>(this); }

        static void BindLua(LuaState L) {
            LuaBinding(L).beginExtendClass<mmGameSingle, mmGame>("mmGameSingle")
                .endClass();
        }
    };
}