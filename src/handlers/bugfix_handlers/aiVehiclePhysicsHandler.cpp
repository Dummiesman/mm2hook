#include "aiVehiclePhysicsHandler.h"

using namespace MM2;

/*
    aiVehiclePhysicsHandler
*/

void aiVehiclePhysicsHandler::InitCar(char const* basename, int variant, int colliderID, bool useFullBound, bool hasTrailer)
{
    reinterpret_cast<vehCar*>(this)->Init(basename, variant, colliderID, useFullBound, false);
}

void aiVehiclePhysicsHandler::Install() 
{
    InstallCallback("aiVehiclePhysics::Init", "Disallow trailers on AI vehicles as they universally do not get updated, and actually break things.",
        &InitCar, {
            cb::call(0x55942D),
        }
    );
}