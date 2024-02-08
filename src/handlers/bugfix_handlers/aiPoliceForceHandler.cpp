#include "aiPoliceForceHandler.h"

using namespace MM2;

/*
    aiPoliceForceHandler
*/

void* aiPoliceForceHandler::AllocationSizeHook(unsigned int size)
{
    return hook::StaticThunk<0x577360>::Call<void*>(sizeof(aiPoliceForce));
}

BOOL aiPoliceForceHandler::UnRegisterCop(MM2::vehCar* copCar, MM2::vehCar* perpCar)
{
    return reinterpret_cast<aiPoliceForce*>(this)->UnRegisterCop(copCar, perpCar);
}

BOOL aiPoliceForceHandler::RegisterPerp(MM2::vehCar* copCar, MM2::vehCar* perpCar)
{
    return reinterpret_cast<aiPoliceForce*>(this)->RegisterPerp(copCar, perpCar);
}

int aiPoliceForceHandler::State(MM2::vehCar* copCar, MM2::vehCar* perpCar, float dist)
{
    return reinterpret_cast<aiPoliceForce*>(this)->State(copCar, perpCar, dist);
}

void aiPoliceForceHandler::Reset(void) {
    // reset number of cops pursuing player
    // fixes incorrect music bug
    vehPoliceCarAudio::iNumCopsPursuingPlayer = 0;
    reinterpret_cast<aiPoliceForce*>(this)->Reset();
}

void aiPoliceForceHandler::Install() {
    InstallCallback("aiPoliceForce::Reset", "Resets the number of cops pursuing the player upon reset.",
        &Reset, {
            cb::call(0x536AAE),
            cb::call(0x550ECA),
        }
    );

    InstallCallback("aiPoliceForce::State", "Use new State function.",
        &State, {
            cb::call(0x53DDE8),
        }
    );

    InstallCallback("aiPoliceForce::UnRegisterCop", "Use new UnRegisterCop function.",
        &UnRegisterCop, {
            cb::call(0x53F1B7),
        }
    );

    InstallCallback("aiPoliceForce::RegisterPerp", "Use new RegisterPerp function.",
        &RegisterPerp, {
            cb::call(0x53E06E),
            cb::call(0x53E1C7),
        }
    );

    InstallCallback("new aiPoliceForce()", "Use new size.",
        &AllocationSizeHook, {
            cb::call(0x535C91),
        }
    );
}