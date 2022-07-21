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

void mmPlayerHandler::Zoink() {
    Warningf("Player is out of the world, teleporting!");

    //get required vars
    auto player = reinterpret_cast<mmPlayer*>(this);
    auto car = player->getCar();
    auto carPos = car->GetModel()->GetPosition();

    // tell the player "That didn't happen!"
    player->getHUD()->SetMessage(AngelReadString(29), 3.f, 0);

    // if we're in CNR, drop the gold!
    if (dgStatePack::Instance->GameMode == dgGameMode::CnR) {
        auto game = mmGameManager::Instance->getGame();
        hook::Thunk<0x425460>::ThisCall<void>(game); // mmMultiCR::DropThruCityHandler
    }

    // early exit
    auto AIMAP = aiMap::GetInstance();
    if (AIMAP == nullptr || AIMAP->GetIntersectionCount() == 0) {
        car->Reset();
        return;
    }

    // search for an intersection to teleport to
    float shortestDistance = 99999;
    int closestIntersection = -1;

    for (int is = 0; is < AIMAP->numIntersections; is++) {
        auto intersection = AIMAP->intersections[is];

        // avoid dummy intersections
        if (intersection->GetPathCount() == 0)
            continue;

        // check roads to see if this is a valid spawn point
        // valid == (!freeway && !alley)
        bool isValid = true;
        for (int i = 0; i < intersection->GetPathCount(); i++) {
            auto path = intersection->GetPath(i);
            ushort pathFlags = *getPtr<ushort>(path, 12);

            if (pathFlags & 4 || pathFlags & 2) {
                isValid = false;
                break;
            }
        }

        if (isValid) {
            float pDist = intersection->GetCenter().Dist(carPos);
            if (pDist < shortestDistance) {
                shortestDistance = pDist;
                closestIntersection = is;
            }
        }
    }

    // move player to the closest intersection if we can
    car->Reset();
    if (closestIntersection >= 0) {
        auto ics = car->GetICS();
        ics->SetPosition(AIMAP->Intersection(closestIntersection)->GetCenter());
    }
}

bool prevSplashState = false;
void mmPlayerHandler::Splash() {
    auto player = reinterpret_cast<mmPlayer*>(this);
    auto car = player->getCar();
    float vehicleMph = car->GetModel()->GetVelocity().Mag() * 2.23694f;

    //trigger ColliderId 22 with velocity of vehicleMph
    auto impactAud = car->GetCarAudioContainerPtr()->GetAudImpactPtr();
    impactAud->Play(vehicleMph, 22);
}

void mmPlayerHandler::PlayExplosion() {
    auto player = reinterpret_cast<mmPlayer*>(this);
    auto car = player->getCar();
    auto policeAudio = car->GetCarAudioContainerPtr()->GetPoliceCarAudioPtr();
    auto explosionSound = *getPtr<AudSoundBase*>(policeAudio, 0x138);
    if (explosionSound != nullptr) {
        if (!explosionSound->IsPlaying())
            explosionSound->PlayOnce(-1.f, -1.f);
    }
}

void mmPlayerHandler::BustPerp() {
    auto player = reinterpret_cast<mmPlayer*>(this);
    auto carsim = player->getCar()->GetCarSim();
    auto AIMAP = aiMap::GetInstance();

    if (Wanted_Common::enableBustedTimer)
        Wanted_Common::bustedTimer += datTimeManager::Seconds;

    for (int i = 0; i < AIMAP->numCops; i++)
    {
        auto police = AIMAP->Police(i);
        auto car = police->GetCar();
        auto curDamage = car->GetCarDamage()->getCurDamage();
        auto maxDamage = car->GetCarDamage()->getMaxDamage();
        auto copCarSim = car->GetCarSim();
        auto policePos = car->GetModel()->GetPosition();
        auto policeAud = car->GetCarAudioContainerPtr()->GetPoliceCarAudioPtr();
        auto playerPos = player->getCar()->GetModel()->GetPosition();

        if (vehPoliceCarAudio::iNumCopsPursuingPlayer == 0) {
            if (lvlLevel::GetSingleton()->GetRoomInfo(car->GetModel()->GetRoomId())->Flags & static_cast<int>(RoomFlags::HasWater)) {
                if (lvlLevel::GetSingleton()->GetWaterLevel(car->GetModel()->GetRoomId()) > copCarSim->GetWorldMatrix()->m31) {
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
                    if (carsim->GetSpeedMPH() <= Wanted_Common::bustedMaxSpeed && copCarSim->GetSpeed() <= Wanted_Common::bustedMaxSpeed) {
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
    auto audio = car->GetCarAudioContainerPtr();
    auto siren = car->GetSiren();
    auto AIMAP = aiMap::GetInstance();

    if (Wanted_Common::enableOppBustedTimer)
        Wanted_Common::oppBustedTimer += datTimeManager::Seconds;

    for (int i = 0; i < AIMAP->numOpponents; i++)
    {
        auto opponent = AIMAP->Opponent(i);
        auto oppCar = opponent->GetCar();
        auto carsim = oppCar->GetCarSim();
        auto curDamage = oppCar->GetCarDamage()->getCurDamage();
        auto maxDamage = oppCar->GetCarDamage()->getMaxDamage();
        auto opponentPos = oppCar->GetModel()->GetPosition();
        auto playerPos = car->GetModel()->GetPosition();

        if (*getPtr<int>(oppCar, 0xEC) != 0 && curDamage < maxDamage)
            continue;

        if (*getPtr<int>(opponent, 0x27C) != 3) {
            if (opponentPos.Dist(playerPos) <= 12.5f) {
                if (carsim->GetSpeedMPH() <= Wanted_Common::bustedMaxSpeed) {
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
    auto audio = car->GetCarAudioContainerPtr();
    auto siren = car->GetSiren();
    auto carsim = car->GetCarSim();
    auto engine = carsim->GetEngine();
    auto basename = player->getCar()->GetCarDamage()->GetName();
    auto flagsId = VehicleListPtr->GetVehicleInfo(basename)->GetFlags();
    auto AIMAP = aiMap::GetInstance();

    //check if we're out of the level
    int playerRoom = car->GetInst()->GetRoomId();
    if (playerRoom == 0 && cfgEnableOutOfMapFix.Get()) {
        Zoink();
    }

    //play splash sound if we just hit the water
    if (cfgEnableWaterSplashSound.Get()) {
        bool splashState = car->GetSplash()->isActive();
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
            engine->SetCurrentTorque(0.f);
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
                    carsim->SetBrake(1.f);
                    engine->SetThrottleInput(0.f);
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
                        game->GetPopup()->ProcessEscape(0);
                        player->getHUD()->StopTimers();
                        Wanted_Common::enableResetTimer = false;
                        Wanted_Common::resetTimer = 0.f;
                    }
                }
            }
        }
    }

    if (carsim->GetWorldMatrix()->m11 <= 0.f)
        car->GetStuck()->setStuckTime(0.f);

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

Matrix34 leftMatrix;
Matrix34 rightMatrix;

void mmPlayerHandler::SetHeadPtr(Matrix34* ptr, int a3) 
{
    Matrix34 camMatrix = *ptr;

    Vector3 left = camMatrix.Transform(Vector3(-1, 0, 0));
    Vector3 right = camMatrix.Transform(Vector3(1, 0, 0));
    

    leftMatrix.Set(&camMatrix);
    rightMatrix.Set(&camMatrix);
    leftMatrix.SetRow(3, left);
    rightMatrix.SetRow(3, right);

    hook::Thunk<0x510020>::Call<void>(this, &leftMatrix, 0);
    hook::Thunk<0x510040>::Call<void>(this, &rightMatrix, 0);
}

void mmPlayerHandler::Install() {
    static ConfigValue<int> cfgBustedTarget("BustedTarget", 3);
    static ConfigValue<float> cfgBustedMaxSpeed("BustedMaxSpeed", 20.f);
    static ConfigValue<float> cfgBustedTimeout("BustedTimeout", 4.f);

    Wanted_Common::bustedTarget = cfgBustedTarget.Get();
    Wanted_Common::bustedMaxSpeed = cfgBustedMaxSpeed.Get();
    Wanted_Common::bustedTimeout = cfgBustedTimeout.Get();

    //InstallCallback("Double head OWO", "3D audio test.",
    //    &SetHeadPtr, {
    //        cb::call(0x4057A0),
    //    }
    //);

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

