#pragma once
#include "mm2_common.h"
#include "mm2_inst.h"

namespace MM2
{
    // Forward declarations
    class lvlLevel;

    class lvlLevel {
    public:
        AGE_API void MoveToRoom(lvlInstance *instance, int room) {
            ageHook::Thunk<0x465480>::Call<void>(this, instance, room);
        }
    };
    
}