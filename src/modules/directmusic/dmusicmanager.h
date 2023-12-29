#pragma once
#include <mm2_common.h>
#include "dmusicobject.h"

namespace MM2
{
    // Forward declarations
    class DMusicManager;

    // External declarations


    // Class definitions
    class DMusicManager {
    private:
        DMusicObject* m_DMusObject;
        BOOL m_Active;
    public:
        AGE_API void SetVolume(float volume);
        AGE_API void SetPan(float pan);

        DMusicObject* GetDMusicObjectPtr() const;

        static void BindLua(LuaState L);
    };    
}