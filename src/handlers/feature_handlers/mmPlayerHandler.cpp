#include "mmPlayerHandler.h"

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
    auto car = player->GetCar();
    auto carPos = car->GetModel()->GetPosition();

    // tell the player "That didn't happen!"
    player->GetHUD()->SetMessage(AngelReadString(29), 3.f, 0);

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
    auto car = player->GetCar();
    float vehicleVelocity = car->GetModel()->GetVelocity().Mag();

    //trigger ColliderId 22 with velocity of vehicleMph
    auto impactAud = car->GetCarAudioContainerPtr()->GetAudImpactPtr();
    impactAud->Play(vehicleVelocity, 22);
}

void mmPlayerHandler::PlayExplosion() {
    auto player = reinterpret_cast<mmPlayer*>(this);
    auto car = player->GetCar();
    auto policeAudio = car->GetCarAudioContainerPtr()->GetPoliceCarAudioPtr();
    auto explosionSound = *getPtr<AudSoundBase*>(policeAudio, 0x138);
    if (explosionSound != nullptr) {
        if (!explosionSound->IsPlaying())
            explosionSound->PlayOnce(-1.f, -1.f);
    }
}

void mmPlayerHandler::Update() {
    auto player = reinterpret_cast<mmPlayer*>(this);
    auto car = player->GetCar();
    auto audio = car->GetCarAudioContainerPtr();
    auto siren = car->GetSiren();
    auto carsim = car->GetCarSim();
    auto engine = carsim->GetEngine();
    auto basename = player->GetCar()->GetCarDamage()->GetName();
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
                player->GetHUD()->DeactivateDash();
                player->GetCamView()->SetCam(player->GetPovCam());
            }
        }
    }

    if (carsim->GetWorldMatrix()->m11 <= 0.f)
        car->GetStuck()->SetStuckTime(0.f);

    //call original
    hook::Thunk<0x405760>::Call<void>(this);
}

void mmPlayerHandler::Reset() {
    // deactivate signal lights if they're active
    vehCarModel::HazardLightsState = false;
    vehCarModel::LeftSignalLightState = false;
    vehCarModel::RightSignalLightState = false;

    // call original
    hook::Thunk<0x404A60>::Call<void>(this);
}

void mmPlayerHandler::Install() {
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

