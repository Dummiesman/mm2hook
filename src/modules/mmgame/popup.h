#pragma once
#include <modules\node.h>

namespace MM2
{
    // Forward declarations
    class mmPopup;

    // External declarations
    extern class mmGame;
    extern class PUResults;

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

        void showResultsLua()
        {
            this->ShowResults(FALSE);
        }

        void processEscapeLua(bool pause)
        {
            this->ProcessEscape(pause ? TRUE : FALSE);
        }
    protected:
        static hook::Field<0x18, mmGame *> _game;
        static hook::Field<0x40, PUResults*> _resultsPU;
    public:
        mmGame* GetGame() const 
        {
            return _game.get(this);
        };

        PUResults* GetResults() const
        {
            return _resultsPU.get(this);
        }

        AGE_API BOOL IsEnabled(void)                        { return hook::Thunk<0x42A280>::Call<BOOL>(this); };
        AGE_API void Lock(void)                             { hook::Thunk<0x42B4F0>::Call<void>(this); };
        AGE_API void Unlock(void)                           { hook::Thunk<0x42B500>::Call<void>(this); };
        AGE_API void ProcessChat(void)                      { hook::Thunk<0x42A400>::Call<void>(this); };
        AGE_API void ProcessEscape(BOOL pause)              { hook::Thunk<0x42A320>::Call<void>(this, pause); };
        AGE_API void ShowResults(BOOL disableRoster)        { hook::Thunk<0x42A5E0>::Call<void>(this, disableRoster); }
        AGE_API void ShowReplay()                           { hook::Thunk<0x42A760>::Call<void>(this); }

        static void BindLua(LuaState L) {
            LuaBinding(L).beginExtendClass<mmPopup, asNode>("mmPopup")
                .addPropertyReadOnly("IsEnabled", &getIsEnabledLua)
                .addPropertyReadOnly("Results", &GetResults)
                .addFunction("Lock", &Lock)
                .addFunction("Unlock", &Unlock)
                .addFunction("ProcessChat", &ProcessChat)
                .addFunction("ShowResults", &showResultsLua)
                .addFunction("ShowReplay", &ShowReplay)
                .addFunction("Show", &processEscapeLua)
            .endClass();
        }
    };

    ASSERT_SIZEOF(mmPopup, 0x60);
}