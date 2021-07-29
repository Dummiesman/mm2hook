#include "fxTexelDamageHandler.h"

using namespace MM2;

static ConfigValue<bool> cfgMm1StyleDamage("MM1StyleDamage", false);

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

    if (!cfgMm1StyleDamage.Get())
        return;

    InstallPatch({ 0x8B, 0x55, 0x0 }, {
        0x59242D,
    });

    InstallPatch({ 0x8B, 0x55, 0x0 }, {
        0x59246C,
    });

    InstallPatch({ 0x8B, 0x55, 0x0 }, {
        0x5924A9,
    });

    InstallPatch({ 0x8B, 0x55, 0x0 }, {
        0x5924E5,
    });

    InstallPatch({ 0x8B, 0x55, 0x0 }, {
        0x592525,
    });

    InstallPatch({ 0x8B, 0x55, 0x0 }, {
        0x5925F7,
    });

    InstallPatch({ 0x8B, 0x45, 0x0 }, {
        0x592879,
    });

    //set TexelDamageRadius value to 0.5f
    InstallPatch({ 0xD9, 0x5, 0x60, 0x3, 0x5B, 0x0 }, {
        0x4CAE35, // vehCarDamage::Update
    });
}

