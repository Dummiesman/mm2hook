#pragma once
#include "common.h"
#include "patch.h"
#include "mm2.h"

class mmSingleRoamHandler {
public:
    void ResetToNearestLocation();
    void EscapeDeepWater();
    void HitWaterHandler();
    static void Install();
};

