#include <events\dispatcher.h>
#include "CityListHandler.h"

using namespace MM2;

/*
    CityListHandler
*/
void CityListHandler::EndPhase()
{
    mmCityList* cityList = CityListPtr.get();
    if (cityList) {
        delete cityList;
        CityListPtr.set(nullptr);
    }
}

void CityListHandler::Install()
{
    GameEventDispatcher::RegisterEndPhaseCallback(EndPhase);
}