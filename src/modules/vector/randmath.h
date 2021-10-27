#pragma once
#include <mm2_common.h>

namespace MM2
{
    const auto DisableGlobalSeed = (void(*)(void))(0x4BBD50);
    const auto EnableGlobalSeed = (void(*)(void))(0x4BBD80);
    const auto frand = (float(*)(void))(0x4BBE30);
    const auto irand = (int(*)(void))(0x4BBDF0);
}