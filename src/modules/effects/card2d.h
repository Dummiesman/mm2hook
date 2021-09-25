#pragma once

namespace MM2
{
    // Forward declarations
    class Card2D;

    // External declarations


    // Class definitions
    class Card2D : public asNode {
    public:
        Vector2 Position;
        Vector2 Size;
        uint8_t R;
        uint8_t G;
        uint8_t B;
        uint8_t A;
    };

    // Lua initialization

}