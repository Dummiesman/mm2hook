#pragma once
#include <modules\phys.h>

namespace MM2
{
    // Forward declarations
    class phBoundPolygonal;
    struct phHotEdge;

    // External declarations

    // Class definitions
    struct phHotEdge
    {
        ushort Indices[2];
    };

    class phBoundPolygonal : public phBound
    {
        int VertexCount;
        int FaceCount;
        int EdgeCount;
        Vector3* VertexPtr;
        phPolygon* Polygons;
        phHotEdge* HotEdgePtr;
    };
    ASSERT_SIZEOF(phBoundPolygonal, 0x64);

    // Lua initialization

}