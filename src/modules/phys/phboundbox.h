#pragma once
#include <modules\phys.h>

namespace MM2
{
    // Forward declarations
    class phBoundBox;

    // External declarations


    // Class definitions
    class phBoundBox : public phBoundPolygonal
    {
        Vector3 Size;
        Vector3 Vertices[8];
        phPolygon Polygons[6];
        phMaterial** Materials;
    };
    ASSERT_SIZEOF(phBoundBox, 0x314);

    // Lua initialization

}