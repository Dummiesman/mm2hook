#pragma once
#include "common.h"
#include "patch.h"
#include "mm2.h"

class aiPoliceForceHandler {
public:
    static void* AllocationSizeHook(unsigned int size);
    BOOL UnRegisterCop(MM2::vehCar* copCar, MM2::vehCar* perpCar);
    BOOL RegisterPerp(MM2::vehCar* copCar, MM2::vehCar* perpCar);
    int State(MM2::vehCar* copCar, MM2::vehCar* perpCar, float dist);
    void Reset();

    static void Install();
};