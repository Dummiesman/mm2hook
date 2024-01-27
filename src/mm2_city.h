#pragma once
#include "mm2_common.h"
#include "mm2_level.h"

#include <modules\city.h>
#include <modules\mmcityinfo.h>

namespace MM2
{
    typename typedef void(*SDLIteratorCB)(const void *, int, int, int, const ushort *, void *);

    template<>
    void luaAddModule<module_city>(LuaState L) {
        luaBind<sdlPolyCollection>(L);
        luaBind<sdlPoly>(L);
        luaBind<sdlPage16>(L);
        luaBind<lvlSDL>(L);
        luaBind<cityTimeWeatherLighting>(L);
        luaBind<cityLevel>(L);

        luaBind<mmCityInfo>(L);
        luaBind<mmCityList>(L);

        luaBind<mmRaceData>(L);
        luaBind<mmInfoBase>(L);
        luaBind<mmPlayerData>(L);
        luaBind<dgStatePack>(L);
        luaBind<mmStatePack>(L);
        luaBind<mmPositions>(L);
    }
}