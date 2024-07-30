#include "ViewDistanceHandler.h"

using namespace MM2;

/*
    ViewDistanceHandler
*/

static ConfigValue <float> cfgMaxViewDistance("MaxViewDistance", 1000.0f);

void ViewDistanceHandler::Install() {
    Installf("Setting max view distance to %.6f", cfgMaxViewDistance.Get());
    mem::write(0x50A1BB + 1, cfgMaxViewDistance.Get()); // PUGraphics
    mem::write(0x4F47F9 + 1, cfgMaxViewDistance.Get()); // GraphicsOptions
}

