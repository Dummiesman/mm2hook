#include "aud3DObjectHandler.h"

using namespace MM2;

/*
    aud3DObjectHandler
*/

static ConfigValue<float> cfgDopplerScale("DopplerScale", 1.0f);

float aud3DObjectHandler::CalculateDoppler(float speed) {
    // get original
    float doppler = hook::Thunk<0x512000>::Call<float>(this, speed);
    return ((doppler - 1.0f) * cfgDopplerScale.Get()) + 1.0f;
}

void aud3DObjectHandler::Install() {
    InstallCallback("aud3DObjectHandler::CalculateDoppler", "Allow for doppler scaling.",
        &CalculateDoppler, {
            cb::call(0x4CFBF8),
            cb::call(0x4DBDA2),
            cb::call(0x510ACE),
            cb::call(0x515297),
            cb::call(0x59D4DD),
        }
    );
}