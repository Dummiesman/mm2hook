#pragma once
#include <modules\node.h>

namespace MM2
{
    // Forward declarations
    class mmToolTip;

    // External declarations
    extern class UIMenu;
    extern class mmTextNode;

    // Class definitions
    class mmToolTip : public asNode {
        UIMenu *pParent;
        mmTextNode *pText;
        uint32_t dword20;
        Vector2 Size;
    };
}