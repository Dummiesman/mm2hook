#pragma once
#include "common.h"
#include "patch.h"
#include "mm2.h"

class vehPoliceCarAudioHandler {
public:
    void InitSirenAudio(MM2::vehCarSim* a1, MM2::vehCarDamage* a2, LPCSTR basename, LPCSTR sirenCsvFile, bool a5);
    void Reset();
    static void Install();
};