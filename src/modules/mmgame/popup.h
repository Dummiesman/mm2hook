#pragma once
#include <modules\node.h>

namespace MM2
{
    // Forward declarations
    class mmPopup;

    // External declarations
    extern class mmGame;

    // Class definitions
    class mmPopup : public asNode {
        // PUMain: 0x1C (size: 0xC8)
        // PUQuit: 0x20 (size: 0xC4)
        // PUExit: 0x24 (size: 0xBC)
        // TODO...
    private:
        byte _buffer[0x48];

        bool getIsEnabledLua()
        {
            return this->IsEnabled() == TRUE;
        }
    protected:
        static hook::Field<0x18, mmGame *> _game;
    public:
        inline mmGame* getGame(void) const {
            return _game.get(this);
        };

        AGE_API BOOL IsEnabled(void)                        { return hook::Thunk<0x42A280>::Call<BOOL>(this); };
        AGE_API void Lock(void)                             { hook::Thunk<0x42B4F0>::Call<void>(this); };
        AGE_API void Unlock(void)                           { hook::Thunk<0x42B500>::Call<void>(this); };
        AGE_API void ProcessChat(void)                      { hook::Thunk<0x42A400>::Call<void>(this); };
        AGE_API void ProcessEscape(int a1)                  { hook::Thunk<0x42A320>::Call<void>(this, a1); };

        static void BindLua(LuaState L) {
            LuaBinding(L).beginClass<mmPopup>("mmPopup")
                .addPropertyReadOnly("IsEnabled", &getIsEnabledLua)
                .addFunction("Lock", &Lock)
                .addFunction("Unlock", &Unlock)
                .addFunction("ProcessChat", &ProcessChat)
            .endClass();
        }
    };

    ASSERT_SIZEOF(mmPopup, 0x60);
}