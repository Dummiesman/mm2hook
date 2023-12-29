#pragma once
#include <modules\audio\speech.h>

namespace MM2
{
    // Forward declarations
    class mmCNRSpeech;

    class mmCNRSpeech : public AudSpeech {
    public:
        AGE_API void Play(LPCSTR a1)                         { hook::Thunk<0x5A7800>::Call<void>(this, a1); }

        static void BindLua(LuaState L) {
            LuaBinding(L).beginExtendClass<mmCNRSpeech, AudSpeech>("mmCNRSpeech")
                .addFunction("Play", &Play)
            .endClass();
        }
    };
}