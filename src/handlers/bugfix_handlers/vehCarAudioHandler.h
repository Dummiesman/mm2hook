#pragma once
#include "common.h"
#include "patch.h"
#include "mm2.h"

class vehCarAudioHandler {
public:
    ANGEL_ALLOCATOR // Use MM2 memory management

    void DestructEngineSampleWrapper();
    void DestructSurfaceAudioData();

    bool IsAirBorne();
    void Update();
    void Reset();

    static void Install();
};