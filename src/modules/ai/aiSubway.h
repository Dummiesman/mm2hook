#pragma once
#include "aiVehicle.h"

namespace MM2
{
    // Forward declarations
    class aiSubway;

    // External declarations


    // Class definitions

    class aiSubway : public aiVehicle {
    private:
        byte _buffer[0x13C];
    };

    ASSERT_SIZEOF(aiSubway, 0x14C);
}