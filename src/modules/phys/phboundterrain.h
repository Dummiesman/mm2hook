#pragma once
#include <modules\phys.h>

namespace MM2
{
    // Forward declarations
    class phBoundTerrain;

    // External declarations


    // Class definitions
    class phBoundTerrain : public phBoundGeometry
    {
        bool UseHotEdges;
        Vector3 Size;
        int WidthSectionCount;
        int HeightSectionCount;
        int DepthSectionCount;
        int TotalSectionCount;
        int TotalReferencedFacesCount;
        ushort* SectionIndexOffsets;
        ushort* SectionIndexCounts;
        ushort* SectionIndices;
        int dword_ac;
        int dword_b0;
        Vector3 SectionSizeFactors;
        int dword_c0;
        int dword_c4;
        int dword_c8;
    };
    ASSERT_SIZEOF(phBoundTerrain, 0xCC);

    // Lua initialization

}