#pragma once
#include <modules\node.h>

namespace MM2
{
    // Forward declarations
    class RadialGauge;

    // External declarations


    // Class definitions
    class RadialGauge : public asNode
    {
    private:
        asLinearCS LinearCS;
        float* ValuePtr;
        float MaxValue;
        float RotMin;
        float RotMax;
        float MinValue;
        Vector3 dword_ac;
        int ShaderSet;
        int ModStatic;
        Vector3 Pivot;
        Vector3 Offset;
        Vector3 PivotOffset;
        Matrix34 dword_e4;
    };
}