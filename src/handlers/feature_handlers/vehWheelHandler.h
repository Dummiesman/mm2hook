#pragma once
#include "common.h"
#include "patch.h"
#include "mm2.h"

class vehWheelHandler {
public:
    float GetBumpDisplacement(float a1);
    static void Install();
};

