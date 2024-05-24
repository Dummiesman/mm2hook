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

void mmHudMapFeatureHandler::DrawColoredTri(unsigned int color, const Matrix34& a2) {
    rglEnableDisable(RGL_DEPTH_TEST, false);
    gfxRenderState::SetWorldMatrix(a2);
    vglBindTexture(0);
    vglBegin(DRAWMODE_TRIANGLELIST, 0);
    vglCurrentColor = color;
    vglVertex(0.f, 0.f, -1.f);
    vglVertex(-0.69999999f, 0.f, 1.f);
    vglVertex(0.69999999f, 0.f, 1.f);
    vglEnd();
    rglEnableDisable(RGL_DEPTH_TEST, true);
}

void mmHudMapFeatureHandler::DrawWhiteTri(const Matrix34& a1) 
{
    DrawColoredTri(0xFFFFFFFF, a1);
}

void mmHudMapFeatureHandler::DrawLightOrangeTri(const Matrix34& a1) 
{
    DrawColoredTri(0xFFFDBF72, a1);
}

void mmHudMapFeatureHandler::DrawLightGreenTri(const Matrix34& a1) {
    DrawColoredTri(0xFFC0EC42, a1);
}

void mmHudMapFeatureHandler::DrawIcon(int iconType, const Matrix34& matrix) {
    auto map = reinterpret_cast<mmHudMap*>(this);
    mtx.Set(matrix);

    mtx.SetRow(1, Vector3::YAXIS);
    mtx.Normalize();

    mtx.m31 += 15.f;
    mtx.Scale(map->GetIconScale());

    if (iconType >= 0)
        DrawColoredTri(HudmapIconColors.ptr()[iconType], mtx);
    if (iconType < 0)
        DrawWhiteTri(mtx);
}

void mmHudMapFeatureHandler::DrawOutlinedIcon(int iconType, int iconOutlineType, const MM2::Matrix34& matrix)
{
    Matrix34 tempMatrix;
    tempMatrix.Set(matrix);

    auto map = reinterpret_cast<mmHudMap*>(this);
    float iconScaleOld = map->GetIconScale();
    map->SetIconScale(iconScaleOld * 1.3f);

    DrawIcon(iconOutlineType, tempMatrix);

    map->SetIconScale(iconScaleOld);

    tempMatrix.m31 += 1.1f;
    DrawIcon(iconType, tempMatrix);
}

void mmHudMapFeatureHandler::DrawOutlinedIcon(int iconType, const MM2::Matrix34& matrix)
{
    DrawOutlinedIcon(iconType, 0, matrix);
}


void mmHudMapFeatureHandler::DrawNfsMwPlayerIcon(const Matrix34 &matrix) {
    auto map = reinterpret_cast<mmHudMap*>(this);
    mtx.Set(matrix);

    mtx.SetRow(1, Vector3::YAXIS);
    mtx.Normalize();

    mtx.m31 += 15.f;
    mtx.Scale(map->GetIconScale());

    DrawLightOrangeTri(mtx);
}

void mmHudMapFeatureHandler::DrawNfsMwOpponentIcon(const Matrix34 &matrix) {
    auto map = reinterpret_cast<mmHudMap*>(this);
    mtx.Set(matrix);

    mtx.SetRow(1, Vector3::YAXIS);
    mtx.Normalize();

    mtx.m31 += 15.f;
    mtx.Scale(map->GetIconScale());

    DrawLightGreenTri(mtx);
}

void mmHudMapFeatureHandler::DrawPlayer() {
    auto map = reinterpret_cast<mmHudMap*>(this);
    auto mgr = *mmGameManager::Instance;
    auto game = mgr->getGame();
    auto player = game->GetPlayer();
    auto car = player->GetCar();
    auto audio = car->GetCarAudioContainerPtr();
    auto siren = car->GetSiren();
    char *vehName = car->GetCarDamage()->GetName();
    bool elapsedTime1 = fmod(datTimeManager::ElapsedTime, 0.15f) > 0.1f;
    bool elapsedTime2 = fmod(datTimeManager::ElapsedTime, 0.125f) > 0.1f;
    bool elapsedTime3 = fmod(datTimeManager::ElapsedTime, 0.5f) > 0.25f;

    // draw triangle outline
    auto playerMtx = player->GetCar()->GetICS()->GetMatrix();;
    float outlineSize = map->GetIconScale() * 1.3f;
    auto oldSize = map->GetIconScale();

    if (hudMapColorStyle == 0) {
        DrawOutlinedIcon(5, playerMtx);
    }
    if (hudMapColorStyle == 1) 
    {
        if (audio->IsPolice(vehName)) 
        {
            DrawOutlinedIcon(1, 2, playerMtx);
        }
        else 
        {
            DrawOutlinedIcon(0, -1, playerMtx);
        }
    }
    if (hudMapColorStyle == 2) 
    {
        int iconColor = 5;
        if (audio->IsPolice(vehName)) {
            iconColor = 2;
            if (siren != nullptr && siren->IsActive()) {
                if (elapsedTime3)
                    iconColor = 1;
            }
        }
        DrawOutlinedIcon(iconColor, playerMtx);
    }
    if (hudMapColorStyle == 3) 
    {
        map->SetIconScale(outlineSize);
        DrawIcon(0, playerMtx);
        map->SetIconScale(oldSize);

        if (audio->IsPolice(vehName)) 
        {
            if (siren != nullptr && siren->IsActive()) 
            {
                DrawIcon(2, playerMtx);
                if (elapsedTime1)
                    DrawIcon(1, playerMtx);
                if (elapsedTime2)
                    DrawIcon(-1, playerMtx);
            }
            if (siren != nullptr && !siren->IsActive())
            {
                DrawIcon(-1, playerMtx);
            }
        }
        else 
        {
            DrawNfsMwPlayerIcon(playerMtx);
        }
    }
    if (hudMapColorStyle == 4) 
    {
        int iconIndex = 8;
        if (audio->IsPolice(vehName)) {
            if (siren != nullptr && siren->IsActive())
            {
                iconIndex = 2;
                if (elapsedTime1)
                    iconIndex = 1;
                if (elapsedTime2)
                    iconIndex = -1;
            }
            if (siren != nullptr && !siren->IsActive())
            {
                iconIndex = 4;
            }
        }
        DrawOutlinedIcon(iconIndex, playerMtx);
    }
    if (hudMapColorStyle >= 5) 
    {
        DrawOutlinedIcon(playerTriColor, playerTriOutlineColor, playerMtx);
    }

    // Hook to allow for custom icon drawing
    MM2Lua::OnRenderHudmap();
}

void mmHudMapFeatureHandler::DrawCops() {
    auto map = reinterpret_cast<mmHudMap*>(this);
    auto AIMAP = aiMap::GetInstance();
    bool elapsedTime1 = fmod(datTimeManager::ElapsedTime, 0.15f) > 0.1f;
    bool elapsedTime2 = fmod(datTimeManager::ElapsedTime, 0.125f) > 0.1f;
    bool elapsedTime3 = fmod(datTimeManager::ElapsedTime, 0.5f) > 0.25f;
    bool showAllCops = map->GetShowAllCops();

    for (int i = 0; i < AIMAP->numCops; i++) {
        auto police = AIMAP->Police(i);
        auto policeMtx = police->GetCar()->GetICS()->GetMatrix();
        WORD policeState = police->GetPoliceState();

        // check if the cop in pursuit
        if (policeState || showAllCops) {
            if (hudMapColorStyle == 0) {

                DrawOutlinedIcon(1, policeMtx);
            }
            if (hudMapColorStyle == 1) {
                DrawOutlinedIcon(1, 2, policeMtx);
            }
            if (hudMapColorStyle == 2) {
                int iconIndex = 2;
                if (elapsedTime3)
                    iconIndex = 1;
                if (policeState == 12 || policeState == 0)
                    iconIndex = 2;
                DrawOutlinedIcon(iconIndex, policeMtx);
            }
            if (hudMapColorStyle == 3) {
                int iconIndex = 2;
                if (elapsedTime1)
                    iconIndex = 1;
                if (elapsedTime2)
                    iconIndex = -1;
                if (policeState == 12 || policeState == 0)
                    iconIndex = -1;
                DrawOutlinedIcon(iconIndex, policeMtx);
            }
            if (hudMapColorStyle == 4) {
                int iconIndex = 2;
                if (elapsedTime1)
                    iconIndex = 1;
                if (elapsedTime2)
                    iconIndex = -1;
                if (policeState == 12 || policeState == 0)
                    iconIndex = 4;
                DrawOutlinedIcon(iconIndex, policeMtx);
            }
            if (hudMapColorStyle >= 5) {
                DrawOutlinedIcon(policeTriColor, policeTriOutlineColor, policeMtx);
            }
        }
    }
}

void mmHudMapFeatureHandler::DrawOpponents() 
{
    auto AIMAP = aiMap::GetInstance();
    auto map = reinterpret_cast<mmHudMap*>(this);

    float outlineSize = map->GetIconScale() * 1.3f;
    auto oldSize = map->GetIconScale();

    for (int i = 0; i < map->GetOpponentCount(); i++) 
    {
        auto iconInfo = map->GetOpponentIcon(i);
        if (iconInfo->Enabled && iconInfo->MatrixPtr != nullptr) 
        {
            auto opponentMtx = *iconInfo->MatrixPtr;

            // check if we're in multiplayer
            if (MMSTATE->unk_EC) 
            {
                DrawOutlinedIcon(i + 2, opponentMtx);
            }
            else  
            {
                auto opponent = AIMAP->Opponent(i);
                auto car = opponent->GetCar();
                auto curDamage = car->GetCarDamage()->GetCurDamage();
                auto maxDamage = car->GetCarDamage()->GetMaxDamage();

                if (curDamage < maxDamage) 
                {
                    switch (hudMapColorStyle)
                    {
                        case 0: 
                        {
                            DrawOutlinedIcon(7, opponentMtx);
                            break;
                        }
                        case 1:
                        {
                            DrawOutlinedIcon(i + 2, opponentMtx);
                            break;
                        }
                        case 2:
                        {
                            DrawOutlinedIcon(3, opponentMtx);
                            break;
                        }
                        case 3:
                        {
                            map->SetIconScale(outlineSize);
                            DrawIcon(0, opponentMtx);
                            map->SetIconScale(oldSize);
                            DrawNfsMwOpponentIcon(opponentMtx);
                            break;
                        }
                        case 4:
                        {
                            DrawOutlinedIcon(6, opponentMtx);
                            break; 
                        }
                        case 5:
                        {
                            DrawOutlinedIcon(opponentTriColor, opponentTriOutlineColor, opponentMtx);
                            break;
                        }
                        default:
                        {
                            DrawOutlinedIcon(7, opponentMtx);
                            break;
                        }
                    }
                }
                else 
                {
                    DrawOutlinedIcon(16, opponentMtx);
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

    InstallCallback("mmHudMap::DrawIcon", "Draw outline on any icon drawn via DrawIcon",
        static_cast<void(mmHudMapFeatureHandler::*)(int, const Matrix34&)>(&DrawOutlinedIcon), {
            cb::jmp(0x42F580),
        }
    );
}

