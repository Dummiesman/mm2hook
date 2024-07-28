#pragma once
#include "common.h"
#include "patch.h"
#include "mm2.h"

class aiPoliceOfficerFeatureHandler {
public:
    static void* AllocHook(unsigned int size);
    void PlacementNew();
    void DetectPerpetrator();
    static void Install();
};

