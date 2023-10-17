#include "lvlInstanceFeatureHandler.h"

using namespace MM2;

/*
    lvlInstanceFeatureHandler
*/

unsigned int lvlInstanceFeatureHandler::lvlLandmark_SizeOf()
{
    auto landmark = reinterpret_cast<lvlLandmark*>(this);
    return landmark->lvlLandmark::SizeOf();
}

void lvlInstanceFeatureHandler::lvlLandmark_DrawGlow()
{
    auto landmark = reinterpret_cast<lvlLandmark*>(this);
    landmark->lvlLandmark::DrawGlow();
}

int lvlInstanceFeatureHandler::lvlLandmark_Init(const char* a1, const Matrix34& a2, int a3)
{
    auto landmark = reinterpret_cast<lvlLandmark*>(this);
    return landmark->lvlLandmark::Init(a1, a2, a3);
}

void lvlInstanceFeatureHandler::Install() {
   // Install lvlLandmark extensions
   mem::write(0x46586C + 1, (byte)sizeof(lvlLandmark)); // Change allocation size
   InstallVTableHook("lvlLandmark::Init", &lvlLandmark_Init, { 0x5B1ACC });
   InstallVTableHook("lvlLandmark::DrawGlow", &lvlLandmark_DrawGlow, { 0x5B1AC0 });
   InstallVTableHook("lvlLandmark::SizeOf", &lvlLandmark_SizeOf, { 0x5B1AD0 });
}

