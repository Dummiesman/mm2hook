#pragma once
#include "common.h"
#include "patch.h"
#include "mm2.h"

class aiVehiclePhysicsHandler {
public:
	void InitCar(char const *basename, int variant, int colliderID, bool useFullBound, bool hasTrailer);
    static void Install();
};