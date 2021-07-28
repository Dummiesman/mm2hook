#pragma once
#include "common.h"
#include "patch.h"
#include "mm2.h"

class vehSemiCarAudioBugfixHandler {
public:
    void SetNon3DParams();
    void UpdateAirBlow();
    void UpdateReverse();
    void Init(MM2::vehCarSim* carsim, MM2::vehCarDamage* cardamage, char* basename, bool a5, bool a6, bool a7);
    static void Install();
};