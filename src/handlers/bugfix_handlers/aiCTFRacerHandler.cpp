#include "aiCTFRacerHandler.h"

using namespace MM2;

/*
    aiCTFRacerHandler
*/

void aiCTFRacerHandler::Init(int id, char* basename)
{
    //by default aiCTFRacer uses a hardcoded basename only present in Midnight Club
    LPCSTR aiType = aiMap::GetCTFOpponentType(id % aiMap::GetCTFOpponentTypeCount());
    reinterpret_cast<aiCTFRacer*>(this)->aiCTFRacer::Init(id, aiType);
}

void aiCTFRacerHandler::Install()
{
    InstallCallback("aiMap::Init", "Change basename for aiCTFRacer.",
        &Init, {
            cb::call(0x5354DE), // aiCTFRacer::Init
        }
    );
}