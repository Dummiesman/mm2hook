#pragma once
#include <modules\audio\speech.h>

namespace MM2
{
    // Forward declarations
    class mmRaceSpeech;

    class mmRaceSpeech : public AudSpeech {
    public:
        AGE_API void PlayUnlockVehicle()                     { hook::Thunk<0x51A6C0>::Call<void>(this); }
        AGE_API void PlayUnlockTexture()                     { hook::Thunk<0x51A720>::Call<void>(this); }
        AGE_API void PlayUnlockRace()                        { hook::Thunk<0x51A660>::Call<void>(this); }
        AGE_API void PlayResultsWin()                        { hook::Thunk<0x51A890>::Call<void>(this); }
        AGE_API void PlayResultsPoor()                       { hook::Thunk<0x51A840>::Call<void>(this); }
        AGE_API void PlayResultsMid()                        { hook::Thunk<0x51A8E0>::Call<void>(this); }
        AGE_API void PlayResults(int a1, int a2)             { hook::Thunk<0x51A800>::Call<void>(this, a1, a2); }
        AGE_API void PlayRaceProgress()                      { hook::Thunk<0x51A7D0>::Call<void>(this); }
        AGE_API void PlayPreRace()                           { hook::Thunk<0x51A590>::Call<void>(this); }
        AGE_API void PlayFinalLap()                          { hook::Thunk<0x51A780>::Call<void>(this); }
        AGE_API void PlayFinalCheckPoint()                   { hook::Thunk<0x51A750>::Call<void>(this); }
        AGE_API void PlayDamagePenalty()                     { hook::Thunk<0x51A7A0>::Call<void>(this); }

        static void BindLua(LuaState L) {
            LuaBinding(L).beginExtendClass<mmRaceSpeech, AudSpeech>("mmRaceSpeech")
                .addFunction("PlayUnlockVehicle", &PlayUnlockVehicle)
                .addFunction("PlayUnlockTexture", &PlayUnlockTexture)
                .addFunction("PlayUnlockRace", &PlayUnlockRace)
                .addFunction("PlayResultsWin", &PlayResultsWin)
                .addFunction("PlayResultsPoor", &PlayResultsPoor)
                .addFunction("PlayResultsMid", &PlayResultsMid)
                .addFunction("PlayResults", &PlayResults)
                .addFunction("PlayRaceProgress", &PlayRaceProgress)
                .addFunction("PlayPreRace", &PlayPreRace)
                .addFunction("PlayFinalLap", &PlayFinalLap)
                .addFunction("PlayFinalCheckPoint", &PlayFinalCheckPoint)
                .addFunction("PlayDamagePenalty", &PlayDamagePenalty)
            .endClass();
        }
    };
}