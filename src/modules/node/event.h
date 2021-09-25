#pragma once

namespace MM2
{
    // Forward declarations
    union eqEvent;

    // External declarations


    // Class definitions
    union eqEvent {
        struct data {
            int p0;
            int p1;
            int p2;
            int p3;
            int p4;
            int p5;
            int p6;
            int p7;
            int p8;
        } _s;
        char buffer[36];
    };

    // Lua initialization

}