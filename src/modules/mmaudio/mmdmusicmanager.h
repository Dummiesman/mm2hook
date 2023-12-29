#pragma once
#include <modules\directmusic\dmusicmanager.h>

namespace MM2
{
    // Forward declarations
    class MMDMusicManager;

    class MMDMusicManager : public DMusicManager {
    private:
        char buffer[0x50];
    public:
        static hook::Type<MMDMusicManager*> Instance;

        static void BindLua(LuaState L) {
            LuaBinding(L).beginExtendClass<MMDMusicManager, DMusicManager>("MMDMusicManager")
            .endClass();
        }
    };
}