#include "modStaticHandler.h"

using namespace MM2;

/*
    modStaticHandler
*/

void modStaticHandler::Install()
{
    // Use rewritten modStatic functions
    InstallCallback("modStatic::Draw", "",
        &modStatic::Draw, {
            cb::jmp(0x4A4550),
        }
    );

    InstallCallback("modStatic::DrawEnvMapped", "",
        &modStatic::DrawEnvMapped, {
            cb::jmp(0x4A4A50),
        }
    );

    // TODO: gfxPacketHandler?
    InstallCallback("gfxPacket::Draw", "",
        static_cast<void(gfxPacket::*)(int)>(&gfxPacket::Draw), {
            cb::call(0x4B487F),
            cb::call(0x597E4A),
            cb::call(0x597F8B),
        }
    );
    InstallCallback("gfxPacket::Draw", "",
        static_cast<void(gfxPacket::*)(const Matrix44*, int)>(&gfxPacket::Draw), {
            cb::call(0x597E41),
        }
    );
}

