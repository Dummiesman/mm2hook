#include "mmHudMapFeatureHandler.h"

using namespace MM2;

/*
    mmHudMapFeatureHandler
*/

int hudMapColorStyle = 0;
int playerTriColor = 5;
int playerTriOutlineColor = 0;
int policeTriColor = 1;
int policeTriOutlineColor = 0;
int opponentTriColor = 7;
int opponentTriOutlineColor = 0;

static ConfigValue<int> cfgHudMapColorStyle("HudMapColorStyle", 0);
static ConfigValue<int> cfgPlayerTriColor("PlayerTriColor", 5);
static ConfigValue<int> cfgPlayerTriOutlineColor("PlayerTriOutlineColor", 0);
static ConfigValue<int> cfgPoliceTriColor("PoliceTriColor", 1);
static ConfigValue<int> cfgPoliceTriOutlineColor("PoliceTriOutlineColor", 0);
static ConfigValue<int> cfgOpponentTriColor("OpponentTriColor", 7);
static ConfigValue<int> cfgOpponentTriOutlineColor("OpponentTriOutlineColor", 0);

hook::Type<unsigned int> HudmapIconColors(0x5C4740);
Matrix34 mtx;

void mmHudMapFeatureHandler::DrawColoredTri(unsigned int color, const Matrix34 *a2) {
    rglEnableDisable(RGL_DEPTH_TEST, false);
    gfxRenderState::SetWorldMatrix(*a2);
    vglBindTexture(0);
    vglBegin(DRAWMODE_TRIANGLELIST, 0);
    vglCurrentColor = color;
    vglVertex(0.f, 0.f, -1.f);
    vglVertex(-0.69999999f, 0.f, 1.f);
    vglVertex(0.69999999f, 0.f, 1.f);
    vglEnd();
    rglEnableDisable(RGL_DEPTH_TEST, true);
}

void mmHudMapFeatureHandler::DrawWhiteTri(const Matrix34 *a1) {
    rglEnableDisable(RGL_DEPTH_TEST, false);
    gfxRenderState::SetWorldMatrix(*a1);
    vglBindTexture(0);
    vglBegin(DRAWMODE_TRIANGLELIST, 0);
    vglCurrentColor = 0xFFFFFFFF;
    vglVertex(0.f, 0.f, -1.f);
    vglVertex(-0.69999999f, 0.f, 1.f);
    vglVertex(0.69999999f, 0.f, 1.f);
    vglEnd();
    rglEnableDisable(RGL_DEPTH_TEST, true);
}

void mmHudMapFeatureHandler::DrawLightOrangeTri(const Matrix34 *a1) {
    rglEnableDisable(RGL_DEPTH_TEST, false);
    gfxRenderState::SetWorldMatrix(*a1);
    vglBindTexture(0);
    vglBegin(DRAWMODE_TRIANGLELIST, 0);
    vglCurrentColor = 0xFFFDBF72;
    vglVertex(0.f, 0.f, -1.f);
    vglVertex(-0.69999999f, 0.f, 1.f);
    vglVertex(0.69999999f, 0.f, 1.f);
    vglEnd();
    rglEnableDisable(RGL_DEPTH_TEST, true);
}

void mmHudMapFeatureHandler::DrawLightGreenTri(const Matrix34 *a1) {
    rglEnableDisable(RGL_DEPTH_TEST, false);
    gfxRenderState::SetWorldMatrix(*a1);
    vglBindTexture(0);
    vglBegin(DRAWMODE_TRIANGLELIST, 0);
    vglCurrentColor = 0xFFC0EC42;
    vglVertex(0.f, 0.f, -1.f);
    vglVertex(-0.69999999f, 0.f, 1.f);
    vglVertex(0.69999999f, 0.f, 1.f);
    vglEnd();
    rglEnableDisable(RGL_DEPTH_TEST, true);
}

void mmHudMapFeatureHandler::DrawIcon(int iconType, const Matrix34 *matrix) {
    mtx.Set(matrix);

    mtx.SetRow(1, Vector3::YAXIS);
    mtx.Normalize();

    mtx.m31 += 15.f;
    mtx.Scale(*getPtr<float>(this, 0x64));

    uint color = *HudmapIconColors[iconType];

    if (iconType >= 0)
        DrawColoredTri(color, &mtx);
    if (iconType < 0)
        DrawWhiteTri(&mtx);
}

void mmHudMapFeatureHandler::DrawNfsMwPlayerIcon(const Matrix34 *matrix) {
    mtx.Set(matrix);

    mtx.SetRow(1, Vector3::YAXIS);
    mtx.Normalize();

    mtx.m31 += 15.f;
    mtx.Scale(*getPtr<float>(this, 0x64));

    DrawLightOrangeTri(&mtx);
}

void mmHudMapFeatureHandler::DrawNfsMwOpponentIcon(const Matrix34 *matrix) {
    mtx.Set(matrix);

    mtx.SetRow(1, Vector3::YAXIS);
    mtx.Normalize();

    mtx.m31 += 15.f;
    mtx.Scale(*getPtr<float>(this, 0x64));

    DrawLightGreenTri(&mtx);
}

void mmHudMapFeatureHandler::DrawPlayer() {
    auto mgr = *mmGameManager::Instance;
    auto game = mgr->getGame();
    auto player = game->getPlayer();
    auto car = player->getCar();
    auto audio = car->GetCarAudioContainerPtr();
    auto siren = car->GetSiren();
    char *vehName = car->GetCarDamage()->GetName();
    bool elapsedTime1 = fmod(datTimeManager::ElapsedTime, 0.15f) > 0.1f;
    bool elapsedTime2 = fmod(datTimeManager::ElapsedTime, 0.125f) > 0.1f;
    bool elapsedTime3 = fmod(datTimeManager::ElapsedTime, 0.5f) > 0.25f;

    // draw triangle outline
    auto playerMtx = *getPtr<Matrix34*>(this, 0x48);
    float triSize = *getPtr<float>(this, 0x64) * 1.3f;
    auto sizeHandler = *getPtr<Matrix34*>(this, 0x64);
    *getPtr<float>(this, 0x64) = triSize;

    if (hudMapColorStyle == 0) {
        DrawIcon(0, playerMtx);
        *getPtr<Matrix34*>(this, 0x64) = sizeHandler;
        DrawIcon(5, playerMtx);
    }
    if (hudMapColorStyle == 1) {
        if (audio->IsPolice(vehName)) {
            DrawIcon(2, playerMtx);
            *getPtr<Matrix34*>(this, 0x64) = sizeHandler;
            DrawIcon(1, playerMtx);
        }
        else {
            DrawIcon(-1, playerMtx);
            *getPtr<Matrix34*>(this, 0x64) = sizeHandler;
            DrawIcon(0, playerMtx);
        }
    }
    if (hudMapColorStyle == 2) {
        DrawIcon(0, playerMtx);
        *getPtr<Matrix34*>(this, 0x64) = sizeHandler;
        if (audio->IsPolice(vehName)) {
            DrawIcon(2, playerMtx);
            if (siren != nullptr && siren->Active) {
                if (elapsedTime3)
                    DrawIcon(1, playerMtx);
            }
        }
        else {
            DrawIcon(5, playerMtx);
        }
    }
    if (hudMapColorStyle == 3) {
        DrawIcon(0, playerMtx);
        *getPtr<Matrix34*>(this, 0x64) = sizeHandler;
        if (audio->IsPolice(vehName)) {
            if (siren != nullptr && siren->Active) {
                DrawIcon(2, playerMtx);
                if (elapsedTime1)
                    DrawIcon(1, playerMtx);
                if (elapsedTime2)
                    DrawIcon(-1, playerMtx);
            }
            if (siren != nullptr && !siren->Active) {
                DrawIcon(-1, playerMtx);
            }
        }
        else {
            DrawNfsMwPlayerIcon(playerMtx);
        }
    }
    if (hudMapColorStyle == 4) {
        DrawIcon(0, playerMtx);
        *getPtr<Matrix34*>(this, 0x64) = sizeHandler;
        if (audio->IsPolice(vehName)) {
            if (siren != nullptr && siren->Active) {
                DrawIcon(2, playerMtx);
                if (elapsedTime1)
                    DrawIcon(1, playerMtx);
                if (elapsedTime2)
                    DrawIcon(-1, playerMtx);
            }
            if (siren != nullptr && !siren->Active) {
                DrawIcon(4, playerMtx);
            }
        }
        else {
            DrawIcon(8, playerMtx);
        }
    }
    if (hudMapColorStyle >= 5) {
        DrawIcon(playerTriOutlineColor, playerMtx);
        *getPtr<Matrix34*>(this, 0x64) = sizeHandler;
        DrawIcon(playerTriColor, playerMtx);
    }
}

void mmHudMapFeatureHandler::DrawCops() {
    auto AIMAP = aiMap::GetInstance();
    bool elapsedTime1 = fmod(datTimeManager::ElapsedTime, 0.15f) > 0.1f;
    bool elapsedTime2 = fmod(datTimeManager::ElapsedTime, 0.125f) > 0.1f;
    bool elapsedTime3 = fmod(datTimeManager::ElapsedTime, 0.5f) > 0.25f;
    BOOL ShowAllCops = *getPtr<BOOL>(this, 0x38);

    for (int i = 0; i < AIMAP->numCops; i++) {
        auto police = AIMAP->Police(i);
        auto policeMtx = getPtr<Matrix34>(*getPtr<Matrix34*>(police, 0xCC), 0x6C);
        WORD policeState = *getPtr<WORD>(police, 0x977A);

        // check if the cop in pursuit
        if (policeState || ShowAllCops) {
            // draw triangle outline
            float triSize = *getPtr<float>(this, 0x64) * 1.3f;
            auto sizeHandler = *getPtr<Matrix34*>(this, 0x64);
            *getPtr<float>(this, 0x64) = triSize;

            if (hudMapColorStyle == 0) {
                DrawIcon(0, policeMtx);
                *getPtr<Matrix34*>(this, 0x64) = sizeHandler;
                DrawIcon(1, policeMtx);
            }
            if (hudMapColorStyle == 1) {
                DrawIcon(2, policeMtx);
                *getPtr<Matrix34*>(this, 0x64) = sizeHandler;
                DrawIcon(1, policeMtx);
            }
            if (hudMapColorStyle == 2) {
                DrawIcon(0, policeMtx);
                *getPtr<Matrix34*>(this, 0x64) = sizeHandler;
                DrawIcon(2, policeMtx);
                if (elapsedTime3)
                    DrawIcon(1, policeMtx);
                if (policeState == 12 || policeState == 0)
                    DrawIcon(2, policeMtx);
            }
            if (hudMapColorStyle == 3) {
                DrawIcon(0, policeMtx);
                *getPtr<Matrix34*>(this, 0x64) = sizeHandler;
                DrawIcon(2, policeMtx);
                if (elapsedTime1)
                    DrawIcon(1, policeMtx);
                if (elapsedTime2)
                    DrawIcon(-1, policeMtx);
                if (policeState == 12 || policeState == 0)
                    DrawIcon(-1, policeMtx);
            }
            if (hudMapColorStyle == 4) {
                DrawIcon(0, policeMtx);
                *getPtr<Matrix34*>(this, 0x64) = sizeHandler;
                DrawIcon(2, policeMtx);
                if (elapsedTime1)
                    DrawIcon(1, policeMtx);
                if (elapsedTime2)
                    DrawIcon(-1, policeMtx);
                if (policeState == 12 || policeState == 0)
                    DrawIcon(4, policeMtx);
            }
            if (hudMapColorStyle >= 5) {
                DrawIcon(policeTriOutlineColor, policeMtx);
                *getPtr<Matrix34*>(this, 0x64) = sizeHandler;
                DrawIcon(policeTriColor, policeMtx);
            }
        }
    }
}

void mmHudMapFeatureHandler::DrawOpponents() {
    auto AIMAP = aiMap::GetInstance();

    for (int i = 0; i < *getPtr<__int16>(this, 0xBC); i++) {
        int v2 = *getPtr<int>(this, 0x34) + (i * 0x28);
        auto opponentMtx = *(Matrix34**)(v2 + 8);

        if (*(int*)(v2 + 4) && opponentMtx) {
            // draw triangle outline
            float triSize = *getPtr<float>(this, 0x64) * 1.3f;
            auto sizeHandler = *getPtr<Matrix34*>(this, 0x64);
            *getPtr<float>(this, 0x64) = triSize;

            // check if we're in multiplayer
            if (MMSTATE->unk_EC) {
                DrawIcon(0, opponentMtx);
                *getPtr<Matrix34*>(this, 0x64) = sizeHandler;
                DrawIcon(i + 2, opponentMtx);
            } 
            else {
                auto opponent = AIMAP->Opponent(i);
                auto car = opponent->GetCar();
                auto curDamage = car->GetCarDamage()->getCurDamage();
                auto maxDamage = car->GetCarDamage()->getMaxDamage();

                if (curDamage < maxDamage) {
                    if (hudMapColorStyle == 0) {
                        DrawIcon(0, opponentMtx);
                        *getPtr<Matrix34*>(this, 0x64) = sizeHandler;
                        DrawIcon(7, opponentMtx);
                    }
                    if (hudMapColorStyle == 1) {
                        DrawIcon(0, opponentMtx);
                        *getPtr<Matrix34*>(this, 0x64) = sizeHandler;
                        DrawIcon(i + 2, opponentMtx);
                    }
                    if (hudMapColorStyle == 2) {
                        DrawIcon(0, opponentMtx);
                        *getPtr<Matrix34*>(this, 0x64) = sizeHandler;
                        DrawIcon(3, opponentMtx);
                    }
                    if (hudMapColorStyle == 3) {
                        DrawIcon(0, opponentMtx);
                        *getPtr<Matrix34*>(this, 0x64) = sizeHandler;
                        DrawNfsMwOpponentIcon(opponentMtx);
                    }
                    if (hudMapColorStyle == 4) {
                        DrawIcon(0, opponentMtx);
                        *getPtr<Matrix34*>(this, 0x64) = sizeHandler;
                        DrawIcon(6, opponentMtx);
                    }
                    if (hudMapColorStyle >= 5) {
                        DrawIcon(opponentTriOutlineColor, opponentMtx);
                        *getPtr<Matrix34*>(this, 0x64) = sizeHandler;
                        DrawIcon(opponentTriColor, opponentMtx);
                    }
                }
                else {
                    DrawIcon(0, opponentMtx);
                    *getPtr<Matrix34*>(this, 0x64) = sizeHandler;
                    DrawIcon(16, opponentMtx);
                }
            }
        }
    }
}

void mmHudMapFeatureHandler::Install() {
    hudMapColorStyle = cfgHudMapColorStyle.Get();
    playerTriColor = cfgPlayerTriColor.Get();
    playerTriOutlineColor = cfgPlayerTriOutlineColor.Get();
    policeTriColor = cfgPoliceTriColor.Get();
    policeTriOutlineColor = cfgPoliceTriOutlineColor.Get();
    opponentTriColor = cfgOpponentTriColor.Get();
    opponentTriOutlineColor = cfgOpponentTriOutlineColor.Get();

    InstallCallback("mmHudMap::DrawPlayer",
        &DrawPlayer, {
            cb::call(0x42F527),
        }
    );

    InstallCallback("mmHudMap::DrawCops",
        &DrawCops, {
            cb::call(0x42F519),
        }
    );

    InstallCallback("mmHudMap::DrawOpponents",
        &DrawOpponents, {
            cb::call(0x42F520),
        }
    );
}

