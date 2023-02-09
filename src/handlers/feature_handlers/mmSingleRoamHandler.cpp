#include "mmSingleRoamHandler.h"

using namespace MM2;

/*
    mmSingleRoamHandler
*/

static ConfigValue<bool> cfgEscapeDeepWater("EscapeDeepWater", true);
static ConfigValue<bool> cfgResetToNearestLocation("ResetToNearestLocation", false);

//TODO: This is a 1:1 copy of out of world handler, lets fix that
void mmSingleRoamHandler::ResetToNearestLocation() {
    auto singleRoam = reinterpret_cast<mmSingleRoam*>(this);
    auto player = singleRoam->GetPlayer();
    auto car = player->GetCar();
    auto carsim = car->GetCarSim();
    auto carPos = car->GetModel()->GetPosition();

    // if the aimap doesn't exist, reset back to spawn
    auto AIMAP = aiMap::GetInstance();
    if (AIMAP == NULL) {
        player->Reset();
        return;
    }

    //search for an intersection to teleport to
    float shortestDistance = 99999;
    int closestIntersection = -1;

    for (int is = 0; is < AIMAP->numIntersections; is++) {
        auto intersection = AIMAP->intersections[is];

        // avoid dummy intersections
        if (intersection->GetPathCount() == 0)
            continue;

        // check roads to see if this is a valid spawn point
        // valid == (!freeway && !alley)
        bool isInvalid = false;
        for (int i = 0; i < intersection->GetPathCount(); i++) {
            auto path = intersection->GetPath(i);
            ushort pathFlags = *getPtr<ushort>(path, 12);

            if (pathFlags & 4 || pathFlags & 2) {
                isInvalid = true;
            }
        }

        if (isInvalid)
            continue;

        // this is a valid intersection
        float pDist = intersection->GetCenter().Dist(carPos);
        if (pDist < shortestDistance) {
            shortestDistance = pDist;
            closestIntersection = is;
        }
    }

    // move player to the closest intersection if we can
    if (closestIntersection >= 0) {
        auto oldResetPos = carsim->GetResetPosition();

        // set to closest intersection
        carsim->SetResetPos(AIMAP->intersections[closestIntersection]->GetCenter());

        // reset vehicle
        player->Reset();

        // set back
        carsim->SetResetPos(oldResetPos);
    }
    else
    {
        // reset vehicle to original spawn
        // no intersection found to teleport to
        player->Reset();
    }
}

void mmSingleRoamHandler::EscapeDeepWater() {
    auto singleRoam = reinterpret_cast<mmSingleRoam*>(this);
    auto player = singleRoam->GetPlayer();
    auto car = player->GetCar();
    auto carsim = car->GetCarSim();
    auto carPos = car->GetModel()->GetPosition();
    auto level = lvlLevel::GetSingleton();

    if (level->GetRoomInfo(car->GetModel()->GetRoomId())->Flags & static_cast<int>(RoomFlags::HasWater) &&
        level->GetWaterLevel(car->GetModel()->GetRoomId()) > carsim->GetWorldMatrix()->m31) {
        if (cfgResetToNearestLocation.Get()) {
            ResetToNearestLocation();
        }
        else {
            player->Reset();
        }
    }
    else {
        //update splash cam
        *getPtr<int>(player, 0x2344) = 0;

        //recheck if player went back into water
        *getPtr<int>(player, 0x2348) = 0;

        //enable camera controls
        *getPtr<byte>(player, 0xE59) = 0;

        car->GetSplash()->setActive(false);
        player->GetCamView()->SetCam(player->GetCurrentCameraPtr());

        if (MMSTATE->ShowDash || *getPtr<int>(player, 0x880)) {
            MMSTATE->ViewMode = 1;
            *getPtr<int>(player, 0xE48) = 1;
            player->GetCamView()->SetCam(player->GetDashCam());
            player->GetHUD()->ActivateDash();
        }

        else if (!player->IsPOV()) {
            player->GetHUD()->DeactivateDash();
        }

        if (!MMSTATE->ShowDash) {
            player->GetDashView()->Deactivate();
        }

        player->SetWideFOV(MMSTATE->UseWideFOV);
    }
}

void mmSingleRoamHandler::HitWaterHandler() {
    auto singleRoam = reinterpret_cast<mmSingleRoam*>(this);
    auto player = singleRoam->GetPlayer();

    if (cfgEscapeDeepWater.Get()) {
        EscapeDeepWater();
    }
    else if (cfgResetToNearestLocation.Get()) {
        ResetToNearestLocation();
    }
    else {
        player->Reset();
    }
}

void mmSingleRoamHandler::Install() {
    InstallVTableHook("mmSingleRoam::HitWaterHandler",
        &HitWaterHandler, {
            0x5B0828
        }
    );
}

