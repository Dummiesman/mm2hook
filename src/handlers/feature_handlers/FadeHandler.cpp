#include "FadeHandler.h"
#include <events\dispatcher.h>
using namespace MM2;

/*
    FadeHandler
*/

void FadeHandler::DoFade() {
    gfxPipeline::SetFade(0xFF000000);
    gfxPipeline::StartFade(0x00000000, 1.f);
}

void FadeHandler::Install()
{
    GameEventDispatcher::RegisterOnResetCallback(DoFade);
}

