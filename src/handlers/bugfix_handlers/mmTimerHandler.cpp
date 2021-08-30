#include "mmTimerHandler.h"

using namespace MM2;

/*
    mmTimerHandler
*/

void mmTimerHandler::Install()
{
    //nop out a weird check where if(time == startTime), ticks are reset
    //this causes the tick based timers to freeze as one or no ticks pass
    //from there to the point time is actually calculated
    mem::nop(0x42E50B, 27);
}