#include "fxTexelDamageHandler.h"

using namespace MM2;

/*
    fxTexelDamageHandler
*/

void fxTexelDamageHandler::Install()
{
    InstallCallback("fxTexelDamage::ApplyDamage", "",
        &fxTexelDamage::ApplyDamage, {
            cb::call(0x4CAE46),
        }
    );

    InstallCallback("fxTexelDamage::Init", "",
        &fxTexelDamage::Init, {
            cb::call(0x4CD492)
        }
    );

    InstallCallback("fxTexelDamage::Reset", "",
        &fxTexelDamage::Reset, {
            cb::call(0x4CE018)
        }
    );

    InstallCallback("fxTexelDamage::Kill", "",
        &fxTexelDamage::Kill, {
            cb::jmp(0x591CC0)
        }
    );
}

