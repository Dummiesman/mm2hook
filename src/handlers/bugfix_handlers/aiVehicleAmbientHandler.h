#pragma once
#include "common.h"
#include "patch.h"
#include "mm2.h"

class aiVehicleAmbientHandler {
private:
    void* CloneImpactAudio(void* otherAudio);
public:
    static void Install();
};