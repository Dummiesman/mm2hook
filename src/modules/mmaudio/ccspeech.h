#pragma once
#include <modules\audio\speech.h>

namespace MM2
{
    // Forward declarations
    class mmCCSpeech;

    class mmCCSpeech : public AudSpeech {
    public:
        AGE_API void PlayUnlock()                            { hook::Thunk<0x51B950>::Call<void>(this); }
        AGE_API void PlayResultsWin()                        { hook::Thunk<0x51B9F0>::Call<void>(this); }
        AGE_API void PlayResultsPoor()                       { hook::Thunk<0x51B9D0>::Call<void>(this); }
        AGE_API void PlayResults(int a1, BOOL playWin)       { hook::Thunk<0x51B9B0>::Call<void>(this, a1, playWin); }
        AGE_API void PlayPreRace()                           { hook::Thunk<0x51B900>::Call<void>(this); }
        AGE_API void PlayCheckPoint(int num, float delay = 0.0099999998f)
                                                             { hook::Thunk<0x51B970>::Call<void>(this, num, delay); }

        static void BindLua(LuaState L) {
            LuaBinding(L).beginExtendClass<mmCCSpeech, AudSpeech>("mmCCSpeech")
                .addFunction("PlayUnlock", &PlayUnlock)
                .addFunction("PlayResultsWin", &PlayResultsWin)
                .addFunction("PlayResultsPoor", &PlayResultsPoor)
                .addFunction("PlayResults", &PlayResults)
                .addFunction("PlayPreRace", &PlayPreRace)
                //.addFunction("PlayCheckPoint", &PlayCheckPoint, LUA_ARGS(int, _def<float, 0.0f>))
            .endClass();
        }
    };
}