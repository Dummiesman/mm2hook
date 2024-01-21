#pragma once
#include "pu_menu.h"

namespace MM2
{
    // Forward declarations
    class PUResults;

    // External declarations


    // Class definitions
    class PUResults : public PUMenuBase
    {
    private:
        char _buf[0x38];
    public:


        /*
            asNode virtuals
        */

        virtual AGE_API void Reset(void) override           { hook::Thunk<0x509B50>::Call<void>(this); };

        /*
            PUResults members
        */
        
        AGE_API void ClearNames()                           { hook::Thunk<0x509E70>::Call<void>(this); }
        AGE_API void DisableNextRace()                      { hook::Thunk<0x509EE0>::Call<void>(this); }
        AGE_API void EnableNextRace()                       { hook::Thunk<0x509EF0>::Call<void>(this); }

        AGE_API void AddName(int pos, LPCSTR name, float time)  { hook::Thunk<0x509BE0>::Call<void>(this, pos, name, time); }
        AGE_API void AddName(int pos, LPCSTR name, int score)   { hook::Thunk<0x509D40>::Call<void>(this, pos, name, score); }
        AGE_API void AddName(int pos, LPCSTR name, LPCSTR text) { hook::Thunk<0x509CA0>::Call<void>(this, pos, name, text); }
        AGE_API void AddLoser(int pos, LPCSTR name)             { hook::Thunk<0x509E10>::Call<void>(this, pos, name); }
        AGE_API void AddTitle(LPCSTR line1, LPCSTR line2)       { hook::Thunk<0x509B20>::Call<void>(this, line1, line2); }
        AGE_API void SetMessage(LPCSTR message)                 { hook::Thunk<0x509F00>::Call<void>(this, message); }

        static void BindLua(LuaState L) {
            LuaBinding(L).beginExtendClass<PUResults, PUMenuBase>("PUResults")
                .addFunction("ClearNames", &ClearNames)
                .addFunction("SetMessage", &SetMessage)
                .addFunction("DisableNextRace", &DisableNextRace)
                .addFunction("EnableNextRace", &EnableNextRace)
                .addFunction("AddNameTimed", static_cast<void (PUResults::*)(int, LPCSTR, float)>(&AddName))
                .addFunction("AddNameScored", static_cast<void (PUResults::*)(int, LPCSTR, int)>(&AddName))
                .addFunction("AddNameGeneric", static_cast<void (PUResults::*)(int, LPCSTR, LPCSTR)>(&AddName))
                .addFunction("AddLoser", &AddLoser)
                .addFunction("AddTitle", &AddTitle)
                .addFunction("SetMessage", &SetMessage)
                .endClass();
        }
    };
    ASSERT_SIZEOF(PUResults, 0xF4);
}