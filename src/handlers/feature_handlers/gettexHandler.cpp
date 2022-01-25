#include "gettexHandler.h"

using namespace MM2;

/*
    gettexHandler
*/

gfxImage* gettexHandler::PrepareHook(gfxImage* image, const char* name, bool lastMip) {
    auto prepared = gfxPrepareImage(image, name, lastMip);
    if (datArgParser::Get("annotate"))
        gfxAnnotateImage(prepared, name);
    return prepared;
}

void gettexHandler::Install() {
    InstallCallback("gfxGetTexture", "Add annotate argument.",
        &PrepareHook, {
            cb::call(0x4B3164),
        }
    );
}