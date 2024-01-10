#pragma once
#include <mm2_common.h>
#include "list.h"

namespace MM2
{
    // Forward declarations
    class audManager;

    // External declarations
    extern class audControl;

    // Class definitions
    class audManager {
    public:
        enum AUDTYPE : int
        {
            AUDTYPE_Wave,
            AUDTYPE_Unk1,
            AUDTYPE_MIDI,
            AUDTYPE_Unk4,
            AUDTYPE_Unk5
        };
    private:
        char _buf[0xC8]; // at least
    public:
        audControl* AllocControl(AUDTYPE type)  { return hook::Thunk<0x5A1C80>::Call<audControl*>(this, type); }
    };
    ASSERT_SIZEOF(audManager, 0xC8);
}