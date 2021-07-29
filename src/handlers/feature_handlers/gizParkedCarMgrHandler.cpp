#include "gizParkedCarMgrHandler.h"

using namespace MM2;

static ConfigValue<bool> cfgDynamicParkedCarDensity("DynamicParkedCarDensity", true);

/*
    gizParkedCarMgrHandler
*/

void gizParkedCarMgrHandler::EnumeratePath(LPCSTR a1, const Matrix34* a2, bool a3) {
    //only apply car scaling in cruise
    if (dgStatePack::Instance->GameMode == Cruise) {
        int oldRandomSeed = gRandSeed;
        float rand = hook::StaticThunk<0x4BBE30>::Call<float>();

        if (dgStatePack::Instance->TrafficDensity > rand) 
            hook::StaticThunk<0x579BD0>::Call<void>(a1, a2, a3); //gizParkedCarMgr_EnumeratePath

        gRandSeed = oldRandomSeed;
    }
    else {
        hook::StaticThunk<0x579BD0>::Call<void>(a1, a2, a3); //gizParkedCarMgr_EnumeratePath
    }
}

void gizParkedCarMgrHandler::Install() {
    if (cfgDynamicParkedCarDensity) {
        InstallCallback("gizParkedCarMgr::Init", "Scales parked cars with traffic density.",
            &EnumeratePath, {
                cb::push(0x579B80),
            }
        );
    }
}