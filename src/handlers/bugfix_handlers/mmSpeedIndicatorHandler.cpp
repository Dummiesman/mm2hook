#include "mmSpeedIndicatorHandler.h"

using namespace MM2;

static ConfigValue<bool> cfgSpeedoUseUpperLimit("SpeedoUseUpperLimit", true);

/*
    mmSpeedIndicator
*/

// due to _ftol's non-standard calling convention (and because we patched the call),
// we have to make the compiler think there's an int in ECX (for __fastcall)
// otherwise, it'll try popping a float from the stack :/
int __fastcall Float2Long(int fValueNotAnInt) {
    // this compiles down to moving ECX into a stack variable
    // which then allows us to _properly_ convert the value
    float actualValue = *(float*)&fValueNotAnInt;
    int result = (int)actualValue;

    return (result < 1000) ? result : 999;
}

void mmSpeedIndicatorHandler::Install() {
    if (cfgSpeedoUseUpperLimit)
    {
        InstallCallback("mmSpeedIndicator::Draw", "Fixes graphical UI errors that occur when a vehicle travels too fast.",
            &Float2Long, {
                cb::call(0x43F345),
            }
        );

        // change 'fld (...)' to 'mov ecx, (...)'
        // this way we can pass it to Float2Long
        InstallPatch({ 0x8B, 0x88 }, {
            0x43F33F,
            });
    }
}