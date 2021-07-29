#include "mmCityListHandler.h"

using namespace MM2;

/*
    mmCityListHandler
*/

void mmCityListHandler::Load(char* cinfoName) {
    CityListPtr->Load("sf.cinfo");
    CityListPtr->Load("london.cinfo");
}

void mmCityListHandler::Install() {
    InstallCallback("mmCityList::LoadAll", "Makes London the 2nd city in the list.",
        &mmCityListHandler::Load, {
            cb::call(0x5244FE),
        }
    );
}