#include "vehTrailerHandler.h"

using namespace MM2;

/*
    vehTrailerHandler
*/

void vehTrailerHandler::Install()
{
    Installf("Removes default.dgTrailerJoint check preventing trailers from loading properly.");
    mem::nop(0x4D7DCC, 19); // nop out default.dgTrailerJoint load attempt
}