#include "mmSingleRoamHandler.h"

using namespace MM2;

/*
    mmSingleRoamHandler
*/

static ConfigValue<bool> cfgEscapeDeepWater("EscapeDeepWater", true);
static ConfigValue<bool> cfgResetToNearestLocation("ResetToNearestLocation", false);

void mmSingleRoamHandler::ResetToNearestLocation() {
    auto singleRoam = reinterpret_cast<mmSingleRoam*>(this);
    auto player = singleRoam->getPlayer();
    auto car = player->getCar();
    auto carsim = car->getCarSim();
    auto carPos = car->getModel()->GetPosition();

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
        if (intersection->pathCount == 0)
            continue;

        // check roads to see if this is a valid spawn point
        // valid == (!freeway && !alley)
        bool isInvalid = false;
        for (int i = 0; i < intersection->pathCount; i++) {
            auto path = intersection->paths[i];
            ushort pathFlags = *getPtr<ushort>(path, 12);

            if (pathFlags & 4 || pathFlags & 2) {
                isInvalid = true;
            }
        }

        if (isInvalid)
            continue;

        // this is a valid intersection
        float pDist = intersection->center.Dist(carPos);
        if (pDist < shortestDistance) {
            shortestDistance = pDist;
            closestIntersection = is;
        }
    }

    // move player to the closest intersection if we can
    if (closestIntersection >= 0) {
        auto oldResetPos = carsim->getResetPosition();

        // set to closest intersection
        carsim->SetResetPos(&AIMAP->intersections[closestIntersection]->center);

        // reset vehicle
        player->Reset();

        // set back
        carsim->SetResetPos(&oldResetPos);
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
    auto player = singleRoam->getPlayer();
    auto car = player->getCar();
    auto carsim = car->getCarSim();
    auto carPos = car->getModel()->GetPosition();
    auto level = lvlLevel::GetSingleton();

    if (level->GetRoomInfo(car->getModel()->GetRoomId())->Flags & static_cast<int>(RoomFlags::HasWater) &&
        level->GetWaterLevel(car->getModel()->GetRoomId()) > carsim->getWorldMatrix()->m31) {
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

        car->getSplash()->setActive(false);
        player->getCamView()->SetCam(player->getCurrentCameraPtr());

        if (MMSTATE->ShowDash || *getPtr<int>(player, 0x880)) {
            MMSTATE->ViewMode = 1;
            *getPtr<int>(player, 0xE48) = 1;
            player->getCamView()->SetCam(player->getDashCam());
            player->getHUD()->ActivateDash();
        }

        else if (!player->IsPOV()) {
            player->getHUD()->DeactivateDash();
        }

        if (!MMSTATE->ShowDash) {
            player->getDashView()->Deactivate();
        }

        player->SetWideFOV(MMSTATE->UseWideFOV);
    }
}

void mmSingleRoamHandler::HitWaterHandler() {
    auto singleRoam = reinterpret_cast<mmSingleRoam*>(this);
    auto player = singleRoam->getPlayer();

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

