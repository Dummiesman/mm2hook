#pragma once
#include <mm2_common.h>

namespace MM2
{
    // Forward declarations
    class aiCityData;

    // External declarations


    // Class definitions

    class aiCityData {
    private:
        byte _buffer[0x30];
    public:
        aiCityData(void)                                    DONOTCALL;
        aiCityData(const aiCityData &&)                     DONOTCALL;

        virtual ~aiCityData(void)                           DONOTCALL;
    };

    ASSERT_SIZEOF(aiCityData, 0x34);
}