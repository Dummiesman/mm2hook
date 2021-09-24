#pragma once
#include <mm2_common.h>
#include <dplobby.h>

namespace MM2
{
    // Forward declarations
    struct netZoneScore;

    // External declarations


    // Class definitions
    struct netZoneScore {
        IDirectPlayLobby3* plobby;
        GUID GUID;
        void* pZoneScore; // unused, never initialized
    };
}