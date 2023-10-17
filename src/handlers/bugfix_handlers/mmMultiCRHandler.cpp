#include "mmMultiCRHandler.h"

using namespace MM2;

/*
    mmMultiCRHandler
*/

void mmMultiCRHandler::GoldRotate(Vector3* axis, float amount)
{
    auto mtx = reinterpret_cast<Matrix34*>(this);
    float newAmount = 3.0f * datTimeManager::Seconds; // match mmPowerupInstance
    mtx->Rotate(*axis, newAmount);
}

bool mmMultiCRHandler::LoadMusic(char* a1, char* a2) {
    return hook::Thunk<0x433F40>::Call<bool>(this, "singlerace", a2);
}

void mmMultiCRHandler::LoadPositions(const char* a1)
{
    auto positions = reinterpret_cast<mmPositions*>(this);
    positions->Init(100);
    positions->Load(a1);
}

void mmMultiCRHandler::Install() {
    InstallCallback("mmMultiCR::Init", "Fixes results screen crash due to incorrect music.",
        &LoadMusic, {
            cb::call(0x4239CB),
        }
    );

    InstallCallback("mmMultiCR::LoadSets", "Fixes uninitialized mmPositions allowing multicopsets to load.",
        &LoadPositions, {
            cb::call(0x4246A4),
        }
    );

    InstallCallback("mmCRHUD::UpdateGold", "Fixes gold rotation being FPS dependent.",
        &GoldRotate, {
            cb::call(0x43828D),
        }
    );

    //changes VTable for music data from roam to race
    InstallPatch({ 0x10, 0x06, 0x5B }, {
        0x423715 + 6,
        });
}