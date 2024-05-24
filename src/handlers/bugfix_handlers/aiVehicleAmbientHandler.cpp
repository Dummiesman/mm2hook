#include "aiVehicleAmbientHandler.h"

using namespace MM2;

/*
    aiVehicleAmbientHandler
*/

void* aiVehicleAmbientHandler::CloneImpactAudio(void* otherAudio)
{
    auto audio = hook::Thunk<0x512F40>::Call<void*>(this, otherAudio);
    *getPtr<BOOL>(audio, 0x18) = *getPtr<BOOL>(otherAudio, 0x18); // Copy SurpressPlay , this is normally set in the parameterless constructor
                                                                  // but was never set in the copy constructor, causing impact audio to instantly play on city load
    return audio;
}

void aiVehicleAmbientHandler::Install() {
    // fixes traffic vehicles using a different color
    // than their respective breakables
    InstallPatch({ 0x90, 0x90, 0x90, 0x90, 0x90 }, {
        0x5513E2,
        });

    InstallCallback("AudCreatureImpact copy constructor", "Fix impact sounds being played instantly on city load",
        &CloneImpactAudio, {
            cb::call(0x512806),
        }
    );
}