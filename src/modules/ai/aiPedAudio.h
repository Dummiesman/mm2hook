#pragma once
#include <modules\audio\audcreaturecontainer.h>

namespace MM2
{
    // Forward declarations
    class aiPedAudio;

    // External declarations

    // Class definitions

    class aiPedAudio : public AudCreatureContainer {
    public:
        static void BindLua(LuaState L) {
            LuaBinding(L).beginExtendClass<aiPedAudio, AudCreatureContainer>("aiPedAudio")
                .endClass();
        }
    };
}