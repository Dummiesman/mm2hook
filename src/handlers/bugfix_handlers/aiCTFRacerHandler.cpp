#include "aiCTFRacerHandler.h"

using namespace MM2;

/*
    aiCTFRacerHandler
*/

void aiCTFRacerHandler::Init(int id, char* basename)
{
    //by default aiCTFRacer uses a hardcoded basename only present in Midnight Club
    reinterpret_cast<aiCTFRacer*>(this)->aiCTFRacer::Init(id, "vpauditt");
}

void aiCTFRacerHandler::Install()
{
    InstallCallback("aiMap::Init", "Change basename for aiCTFRacer.",
        &Init, {
            cb::call(0x5354DE), // aiCTFRacer::Init
        }
    );
}