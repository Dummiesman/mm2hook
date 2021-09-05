#pragma once
#include <modules\phys.h>

namespace MM2
{
    // Forward declarations
    struct phPolygon;

    // External declarations


    // Class definitions
    struct phPolygon
    {
        Vector3 Normal;
        float Radius;
        Vector4 EdgeNormalCross[4];
        ushort Indices[4];
        ushort Indices2[4];

        inline byte getMaterialIndex() 
        {
            return *(byte*)&Radius;
        }

        inline void setMaterialIndex(byte index) 
        {
            *(byte*)&Radius = index;
        }
    };


    // Lua initialization

}