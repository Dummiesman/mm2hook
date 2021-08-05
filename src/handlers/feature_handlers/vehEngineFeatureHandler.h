#pragma once
#include "common.h"
#include "patch.h"
#include "mm2.h"

class vehEngineFeatureHandler {
public:
    float CalcTorque(float a1);
    static void Install();
};