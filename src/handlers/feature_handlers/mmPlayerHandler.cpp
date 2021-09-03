#include "mmPlayerHandler.h"
#include "Wanted_Common.h"

using namespace MM2;

static ConfigValue<bool> cfgAmbientSoundsWithMusic("AmbientSoundsWithMusic", true);
static ConfigValue<bool> cfgEnableModelVisibility("ModelVisibility", false);
static ConfigValue<bool> cfgEnableOutOfMapFix("OutOfMapFix", true);
static ConfigValue<bool> cfgEnableWaterSplashSound("WaterSplashSound", true);
static ConfigValue<bool> cfgEnableExplosionSound("ExplosionSound", true);
static ConfigValue<bool> cfgEnableMissingDashboardFix("MissingDashboardFix", true);

/*
    mmPlayerHandler
*/

bool mmPlayerHandler::playerCanFly = false;

void mmPlayerHandler::Zoink() {
    Warningf("Player is out of the world, teleporting!");

    //get required vars
    auto player = reinterpret_cast<mmPlayer*>(this);
    auto car = player->getCar();
    auto carPos = car->getModel()->GetPosition();

    // tell the player "That didn't happen!"
    player->getHUD()->SetMessage(AngelReadString(29), 3.f, 0);

    //if we're in CNR, drop the gold!
    if (dgStatePack::Instance->GameMode == dgGameMode::CnR) {
        auto game = mmGameManager::Instance->getGame();
        hook::Thunk<0x425460>::ThisCall<void>(game); // mmMultiCR::DropThruCityHandler
    }

    // if the aimap doesn't exist, reset back to spawn
    auto AIMAP = &aiMap::Instance;
    if (AIMAP == NULL) {
        car->Reset();
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
    auto carsim = car->getCarSim();
    if (closestIntersection >= 0) {
        auto oldResetPos = carsim->getResetPosition();

        // set to closest intersection
        carsim->SetResetPos(&AIMAP->intersections[closestIntersection]->center);

        // reset vehicle
        car->Reset();

        // set back
        carsim->SetResetPos(&oldResetPos);
    }
    else
    {
        // reset vehicle to original spawn
        // no intersection found to teleport to
        car->Reset();
    }

}

bool prevSplashState = false;
void mmPlayerHandler::Splash() {
    auto player = reinterpret_cast<mmPlayer*>(this);
    auto car = player->getCar();
    float vehicleMph = car->getModel()->GetVelocity()->Mag() * 2.23694f;

    //trigger ColliderId 22 with velocity of vehicleMph
    auto impactAud = car->getAudio()->GetAudImpactPtr();
    impactAud->Play(vehicleMph, 22);
}

void mmPlayerHandler::PlayExplosion() {
    auto player = reinterpret_cast<mmPlayer*>(this);
    auto car = player->getCar();
    auto policeAudio = car->getAudio()->GetPoliceCarAudioPtr();
    auto explosionSound = *getPtr<AudSoundBase*>(policeAudio, 0x138);
    if (explosionSound != nullptr) {
        if (!explosionSound->IsPlaying())
            explosionSound->PlayOnce(-1.f, -1.f);
    }
}

void mmPlayerHandler::BustPerp() {
    auto player = reinterpret_cast<mmPlayer*>(this);
    auto carsim = player->getCar()->getCarSim();
    auto AIMAP = &aiMap::Instance;

    if (Wanted_Common::enableBustedTimer)
        Wanted_Common::bustedTimer += datTimeManager::Seconds;

    for (int i = 0; i < AIMAP->numCops; i++)
    {
        auto police = AIMAP->Police(i);
        auto car = police->getVehiclePhysics()->getCar();
        auto curDamage = car->getCarDamage()->getCurDamage();
        auto maxDamage = car->getCarDamage()->getMaxDamage();
        auto copCarSim = car->getCarSim();
        auto policePos = car->getModel()->GetPosition();
        auto policeAud = car->getAudio()->GetPoliceCarAudioPtr();
        auto playerPos = player->getCar()->getModel()->GetPosition();

        if (vehPoliceCarAudio::iNumCopsPursuingPlayer == 0) {
            if (**(BYTE**)(*getPtr<int>(lvlLevel::Singleton, 8) + 4 * car->getModel()->getRoomId()) & 4) {
                if (lvlLevel::Singleton->GetWaterLevel(0) > copCarSim->getWorldMatrix()->m31) {
                    Wanted_Common::enableBustedTimer = false;
                    Wanted_Common::bustedTimer = 0.f;
                    Wanted_Common::enableResetTimer = false;
                    Wanted_Common::resetTimer = 0.f;
                }
            }
            if (curDamage >= maxDamage) {
                Wanted_Common::enableBustedTimer = false;
                Wanted_Common::bustedTimer = 0.f;
                Wanted_Common::enableResetTimer = false;
                Wanted_Common::resetTimer = 0.f;
            }
        }

        if (*getPtr<int>(player->getCar(), 0xEC) != 0 && !player->IsMaxDamaged())
            continue;

        if (*getPtr<WORD>(police, 0x977A) != 0 && *getPtr<WORD>(police, 0x977A) != 12) {
            if (*getPtr<vehCar*>(police, 0x9774) == player->getCar()) {
                if (playerPos.Dist(policePos) <= 12.5f) {
                    if (carsim->getSpeedMPH() <= Wanted_Common::bustedMaxSpeed && copCarSim->getSpeed() <= Wanted_Common::bustedMaxSpeed) {
                        Wanted_Common::enableBustedTimer = true;
                    }
                    else {
                        Wanted_Common::enableBustedTimer = false;
                        Wanted_Common::bustedTimer = 0.f;
                    }
                }
                if (Wanted_Common::bustedTimer > Wanted_Common::bustedTimeout) {
                    mmGameManager *mgr = mmGameManager::Instance;
                    auto game = mgr->getGame();
                    auto soundBase = *getPtr<AudSoundBase*>(game, 0x8C);
                    if (!soundBase->IsPlaying()) {
                        int i = rand() % 20 + 1;
                        string_buf<80> buffer("ACOPAPP%02d%s", i, MMSTATE->CityName);
                        if (soundBase->Load(buffer, i + 7, false)) {
                            soundBase->SetSoundHandleIndex(i + 7);
                            soundBase->PlayOnce(-1.f, -1.f);
                        }
                    }
                    if (policeAud != nullptr) {
                        policeAud->StopSiren();
                    }
                    player->getHUD()->SetMessage("Busted!", 4.f, 0);
                    AIMAP->policeForce->UnRegisterCop(*getPtr<vehCar*>(police, 0x14), *getPtr<vehCar*>(police, 0x9774));
                    *getPtr<WORD>(police, 0x977A) = 12;
                    *getPtr<WORD>(police, 0x280) = 3;
                    Wanted_Common::enableBustedTimer = false;
                    Wanted_Common::enableResetTimer = true;
                }
                if (MMSTATE->GameMode != 6) {
                    if (*getPtr<int>(player, 0x2258)) {
                        police->StopSiren();
                        AIMAP->policeForce->UnRegisterCop(*getPtr<vehCar*>(police, 0x14), *getPtr<vehCar*>(police, 0x9774));
                        *getPtr<WORD>(police, 0x977A) = 0;
                        *getPtr<WORD>(police, 0x280) = 3;
                    }
                }
            }
        }
    }
}

void mmPlayerHandler::BustOpp() {
    auto player = reinterpret_cast<mmPlayer*>(this);
    auto car = player->getCar();
    auto audio = car->getAudio();
    auto siren = car->getSiren();
    auto AIMAP = &aiMap::Instance;

    if (Wanted_Common::enableOppBustedTimer)
        Wanted_Common::oppBustedTimer += datTimeManager::Seconds;

    for (int i = 0; i < AIMAP->numOpponents; i++)
    {
        auto opponent = AIMAP->Opponent(i);
        auto oppCar = opponent->getCar();
        auto carsim = oppCar->getCarSim();
        auto curDamage = oppCar->getCarDamage()->getCurDamage();
        auto maxDamage = oppCar->getCarDamage()->getMaxDamage();
        auto opponentPos = oppCar->getModel()->GetPosition();
        auto playerPos = car->getModel()->GetPosition();

        if (*getPtr<int>(oppCar, 0xEC) != 0 && curDamage < maxDamage)
            continue;

        if (*getPtr<int>(opponent, 0x27C) != 3) {
            if (opponentPos.Dist(playerPos) <= 12.5f) {
                if (carsim->getSpeedMPH() <= Wanted_Common::bustedMaxSpeed) {
                    Wanted_Common::enableOppBustedTimer = true;
                    if (Wanted_Common::oppBustedTimer > Wanted_Common::bustedTimeout) {
                        *getPtr<int>(opponent, 0x27C) = 3;
                        siren->Active = false;
                        audio->StopSiren();
                        Wanted_Common::enableOppBustedTimer = false;
                        Wanted_Common::oppBustedTimer = 0.f;
                    }
                }
                else {
                    Wanted_Common::enableOppBustedTimer = false;
                    Wanted_Common::oppBustedTimer = 0.f;
                }
            }
        }
    }
}

void mmPlayerHandler::Update() {
    auto player = reinterpret_cast<mmPlayer*>(this);
    auto car = player->getCar();
    auto audio = car->getAudio();
    auto siren = car->getSiren();
    auto carsim = car->getCarSim();
    auto engine = carsim->getEngine();
    auto basename = player->getCar()->getCarDamage()->GetName();
    auto flagsId = VehicleListPtr->GetVehicleInfo(basename)->GetFlags();
    auto AIMAP = &aiMap::Instance;

    //check if we're out of the level
    int playerRoom = car->GetInst()->getRoomId();
    if (playerRoom == 0 && cfgEnableOutOfMapFix.Get()) {
        Zoink();
    }

    //play splash sound if we just hit the water
    if (cfgEnableWaterSplashSound.Get()) {
        bool splashState = car->getSplash()->isActive();
        if (splashState && splashState != prevSplashState) {
            Splash();
        }
        prevSplashState = splashState;
    }

    //check if we're damaged out
    if (cfgEnableExplosionSound.Get()) {
        if (player->IsMaxDamaged()) {
            //turn off engine
            audio->SilenceEngine(1);
            engine->setCurrentTorque(0.f);
            //play explosion sound if siren is activated
            if (siren != nullptr && siren->Active) {
                siren->Active = false;
                audio->StopSiren();
                PlayExplosion();
            }
        }
        if (!player->IsMaxDamaged()) {
            audio->SilenceEngine(0);
        }
    }

    //check if dashboard model is missing
    if (cfgEnableMissingDashboardFix.Get()) {
        string_buf<80> buffer("%s_dash", basename);
        if (!datAssetManager::Exists("geometry", buffer, "pkg")) {
            if (MMSTATE->ShowDash) {
                player->getHUD()->DeactivateDash();
                player->getCamView()->SetCam(player->getPovCam());
            }
        }
    }

    if (Wanted_Common::bustedTarget != 0) {
        if (audio->IsPolice(basename) && flagsId == 8) {
            if (siren != nullptr && siren->Active)
                BustOpp();
        }

        if (Wanted_Common::bustedTarget == 1 || Wanted_Common::bustedTarget >= 3) {
            if (!audio->IsPolice(basename)) {
                BustPerp();
                if (Wanted_Common::bustedTimer > Wanted_Common::bustedTimeout) {
                    carsim->setBrake(1.f);
                    engine->setThrottleInput(0.f);
                }
            }

            if (Wanted_Common::enableResetTimer) {
                Wanted_Common::resetTimer += datTimeManager::Seconds;
                if (Wanted_Common::resetTimer > 4.f) {
                    mmGameManager *mgr = mmGameManager::Instance;
                    auto game = mgr->getGame();
                    auto soundBase = *getPtr<AudSoundBase*>(game, 0x8C);
                    if (MMSTATE->GameMode == 0) {
                        *getPtr<byte>(mmReplayManager::Instance, 0x19) = 1;
                        soundBase->SetSoundHandleIndex(1);
                        soundBase->PlayOnce(-1.f, -1.f);
                    }
                    else {
                        if (MMSTATE->GameMode == 1)
                            soundBase->SetSoundHandleIndex(6);
                        if (MMSTATE->GameMode == 4 || MMSTATE->GameMode == 6) {
                            soundBase->SetSoundHandleIndex(7);
                            player->getTimer()->Stop();
                        }
                        if (MMSTATE->GameMode == 3)
                            soundBase->SetSoundHandleIndex(5);
                        soundBase->PlayOnce(-1.f, -1.f);
                        game->getPopup()->ProcessEscape(0);
                        player->getHUD()->StopTimers();
                        Wanted_Common::enableResetTimer = false;
                        Wanted_Common::resetTimer = 0.f;
                    }
                }
            }
        }
    }

    if (mmPlayerHandler::playerCanFly) {
        auto ics = getPtr<Vector3>(carsim->getICS(), 0x3C);
        if (engine->getThrottleInput() > 0.f && carsim->getSpeed() < 50.f && !player->IsMaxDamaged()) {
            ics->X *= 1.03f;
            ics->Y *= 1.03f;
            ics->Z *= 1.03f;
        }
    }

    if (carsim->getWorldMatrix()->m11 <= 0.f)
        car->getStuck()->setStuckTime(0.f);

    //call original
    hook::Thunk<0x405760>::Call<void>(this);
}

void mmPlayerHandler::Reset() {
    // deactivate signal lights if they're active
    vehCarModel::HazardLightsState = false;
    vehCarModel::LeftSignalLightState = false;
    vehCarModel::RightSignalLightState = false;

    // disable and reset timers
    Wanted_Common::enableBustedTimer = false;
    Wanted_Common::enableOppBustedTimer = false;
    Wanted_Common::enableResetTimer = false;
    Wanted_Common::bustedTimer = 0.f;
    Wanted_Common::oppBustedTimer = 0.f;
    Wanted_Common::resetTimer = 0.f;

    // call original
    hook::Thunk<0x404A60>::Call<void>(this);
}

void mmPlayerHandler::Install() {
    static ConfigValue<int> cfgBustedTarget("BustedTarget", 3);
    static ConfigValue<float> cfgBustedMaxSpeed("BustedMaxSpeed", 20.f);
    static ConfigValue<float> cfgBustedTimeout("BustedTimeout", 4.f);

    Wanted_Common::bustedTarget = cfgBustedTarget.Get();
    Wanted_Common::bustedMaxSpeed = cfgBustedMaxSpeed.Get();
    Wanted_Common::bustedTimeout = cfgBustedTimeout.Get();

    InstallVTableHook("mmPlayer::Update",
        &Update, {
            0x5B03BC
    });

    if (cfgAmbientSoundsWithMusic) {
        InstallPatch("Enables positional ambient sounds with music.", { 0x90, 0x90 }, {
            0x404044,
        });
    }

    InstallVTableHook("mmPlayer::Reset",
        &Reset, {
            0x5B03C0,
        }
    );

    if (cfgEnableModelVisibility.Get()) {
        InstallPatch("Enables vehicle's body visibility from the dashboard and bumper camera.", { 0x90, 0x90, 0x90 }, {
            0x405ED4,
        });
    }

    //fix collision detection
    InstallPatch({ 0x8B, 0x81, 0xF4, 0x0, 0x0, 0x0 }, {
        0x40493F, // mmPlayer::UpdateHOG
    });
}
