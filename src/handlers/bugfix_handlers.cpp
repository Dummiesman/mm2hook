#include "bugfix_handlers.h"

using namespace MM2;

static init_handler g_bugfix_handlers[] = {
    CreateHandler<aiPathHandler>("aiPath"),
    CreateHandler<aiPedestrianHandler>("aiPedestrian"),
    CreateHandler<aiPoliceForceHandler>("aiPoliceForce"),
    CreateHandler<aiPoliceOfficerHandler>("aiPoliceOfficer"),
    CreateHandler<aiCTFRacerHandler>("aiCTFRacer"),
    CreateHandler<aiVehicleAmbientHandler>("aiVehicleAmbient"),
    CreateHandler<aiVehicleInstanceHandler>("aiVehicleInstance"),
    CreateHandler<aiGoalAvoidPlayerHandler>("aiGoalAvoidPlayer"),
    CreateHandler<aiRouteRacerHandler>("aiRouteRacer"),

    CreateHandler<asMeshCardInfoHandler>("asMeshCardInfo"),

    CreateHandler<audManagerHandler>("audManager"),

    CreateHandler<gfxImageHandler>("gfxImage"),

    CreateHandler<mmBillInstanceHandler>("mmBillInstance"),

    CreateHandler<mmInterfaceHandler>("mmInterface"),
    CreateHandler<mmPopupHandler>("mmPopupHandler"),

    CreateHandler<vehCarAudioHandler>("vehCarAudio"),
    CreateHandler<vehCarAudioContainerBugfixHandler>("vehCarAudioContainer bugfixes"),
    CreateHandler<vehCarDamageHandler>("vehCarDamage"),
    CreateHandler<vehTrailerHandler>("vehTrailer"),
    CreateHandler<vehPoliceCarAudioBugfixHandler>("vehPoliceCarAudio"),
    CreateHandler<vehSemiCarAudioBugfixHandler>("vehSemiCarAudio"),
    CreateHandler<mmViewMgrBugfixHandler>("mmViewMgr"),
    CreateHandler<memMemoryAllocatorHandler>("memMemoryAllocator"),
    CreateHandler<mmGearIndicatorHandler>("mmGearIndicator"),
    CreateHandler<mmSpeedIndicatorHandler>("mmSpeedIndicator"),
    CreateHandler<mmHudMapHandler>("mmHudMap"),
    CreateHandler<mmCDPlayerHandler>("mmCDPlayer"),
    CreateHandler<mmMirrorHandler>("mmMirror"),
    CreateHandler<lvlSkyHandler>("lvlSkyHandler"),

    CreateHandler<modShaderHandler>("modShader"),

    CreateHandler<cityLevelBugfixHandler>("cityLevelBugfixHandler"),

    CreateHandler<mpConsistencyHandler>("mpConsistencyHandler"),
    CreateHandler<mmMultiCRHandler>("mmMultiCR"),

    CreateHandler<BugfixPatchHandler>("Bugfix patches"),

    CreateHandler<pedAnimationInstanceHandler>("pedAnimationInstance"),
    CreateHandler<fxShardManagerBugfixHandler>("fxShardManager"),
    CreateHandler<mmTimerHandler>("mmTimer")
};

/*
    aiPoliceOfficerHandler
*/

static ConfigValue<bool> cfgPoliceAcademyFunding("PoliceAcademyFunding", true);
static ConfigValue<float> cfgDefaultSpeedLimit("DefaultSpeedLimit", 12.25f);
static ConfigValue<float> cfgSpeedLimitTolerance("SpeedLimitTolerance", 1.125f);
static ConfigValue<int> cfgMaximumCopsLimit("MaximumCopsLimit", 3);
int maximumNumCops = 3;
float burnoutTimer = 0.f;
float soundPlayTime = 0.f;

aiVehicle* findVehicle(vehCar *car) {
    auto AIMAP = aiMap::GetInstance();

    // check players
    for (int i = 0; i < 4; i++) {
        auto player = &AIMAP->players[i];

        if (player->getCar() == car)
            return player;
    }

    // check opponents
    for (int i = 0; i < AIMAP->numOpponents; i++) {
        auto opponent = AIMAP->opponents[i];

        if (opponent->GetCar() == car)
            return opponent;
    }

    // not found :(
    return nullptr;
}

float getSpeedLimit(vehCar *car) {
    auto AIMAP = aiMap::GetInstance();
    auto veh = findVehicle(car);

    if (veh != nullptr) {
        auto roadId = veh->CurrentRoadId();
        return AIMAP->Path(roadId)->GetBaseSpeedLimit();
    }

    return cfgDefaultSpeedLimit;
}

float burnoutTime(vehCar *car) {
    float speed = car->GetCarSim()->GetSpeedMPH();

    for (int i = 0; i < 4; i++)
    {
        auto wheel = car->GetCarSim()->GetWheel(i);

        if (fabs(wheel->GetRotationRate()) > 26.f && speed < 10.f)
            return burnoutTimer += datTimeManager::Seconds;
    }

    return burnoutTimer = 0.f;
}

float hornPlayTime(vehCar *car) {
    auto carAudio = car->GetCarAudioContainerPtr()->GetCarAudioPtr();
    auto hornSound = *getPtr<AudSoundBase*>(carAudio, 0x10C);

    if (hornSound->IsPlaying())
        return soundPlayTime += datTimeManager::Seconds;

    return soundPlayTime = 0.f;
}

BOOL aiPoliceOfficerHandler::OffRoad(vehCar *car) {
    auto AIMAP = aiMap::GetInstance();
    auto veh = findVehicle(car);
    float outVal = 0.f;

    if (veh != nullptr) {
        auto roomId = car->GetModel()->GetRoomId();
        auto roadId = veh->CurrentRoadId();
        auto path = AIMAP->paths[roadId];

        if (path->IsPosOnRoad(car->GetCarSim()->GetICS()->GetPosition(), 0.f, &outVal) > 1 && roomId < 900)
            return TRUE;
    }

    for (int i = 0; i < 4; i++)
    {
        auto wheel = car->GetCarSim()->GetWheel(i);

        if (!strcmp(wheel->GetCurrentPhysicsMaterial()->GetName(), "grass"))
            return TRUE;
    }

    return FALSE;
}

BOOL aiPoliceOfficerHandler::IsPerpDrivingMadly(vehCar *perpCar) {
    char *vehName = perpCar->GetCarDamage()->GetName(); // we can't use vehCarSim because the game forces vpcop to vpmustang99...

    // ignore perp if they're a cop
    if (!hook::StaticThunk<0x4D1A70>::Call<bool>(vehName)) {
        if (vehPoliceCarAudio::iNumCopsPursuingPlayer < maximumNumCops || maximumNumCops <= 0) {
            if (hook::Thunk<0x53E2A0>::Call<BOOL>(this, perpCar))
            {
                float speed = perpCar->GetCarSim()->GetSpeedMPH();
                float speedLimit = getSpeedLimit(perpCar) * 2.857142857142857f;

                if (speed > (speedLimit * cfgSpeedLimitTolerance)) {
                    LogFile::Printf(1, "PERP DETECTED!!! He's doing %.4f over the speed limit (~%.4fmph)!\n", (speed - speedLimit), speedLimit);
                    return TRUE;
                }
                if (hook::Thunk<0x53E370>::Call<BOOL>(this, perpCar)) {
                    LogFile::Printf(1, "PERP IS DOING DAMAGE TO PROPERTY!");
                    return TRUE;
                }
                if (aiPoliceOfficerHandler::OffRoad(perpCar)) {
                    LogFile::Printf(1, "PERP IS GOING OFFROAD!");
                    return TRUE;
                }
            }
            if (hook::Thunk<0x53E390>::Call<BOOL>(this, perpCar)) {
                LogFile::Printf(1, "OFFICER INVOLVED COLLISION WITH PERP!");
                return TRUE;
            }
            if (burnoutTime(perpCar) > 3.f) {
                LogFile::Printf(1, "PERP IS DOING BURNOUTS!");
                return TRUE;
            }
            if (hornPlayTime(perpCar) > 3.f) {
                LogFile::Printf(1, "PERP IS SPAMMING HORN!");
                return TRUE;
            }
        }
    }

    return FALSE;
}

BOOL aiPoliceOfficerHandler::IsOppDrivingMadly(vehCar *perpCar) {
    if (hook::Thunk<0x53E2A0>::Call<BOOL>(this, perpCar))
    {
        float speed = perpCar->GetCarSim()->GetSpeedMPH();
        float speedLimit = getSpeedLimit(perpCar) * 2.857142857142857f;

        if (speed > (speedLimit * cfgSpeedLimitTolerance)) {
            return TRUE;
        }
        if (hook::Thunk<0x53E370>::Call<BOOL>(this, perpCar)) {
            return TRUE;
        }
        if (aiPoliceOfficerHandler::OffRoad(perpCar)) {
            return TRUE;
        }
    }
    if (hook::Thunk<0x53E390>::Call<BOOL>(this, perpCar)) {
        return TRUE;
    }

    return FALSE;
}

void aiPoliceOfficerHandler::PerpEscapes(bool a1) 
{
    auto carAudioContainer = *getPtr<vehCarAudioContainer*>(this, 0x268);
    auto policeCarAudio = carAudioContainer->GetPoliceCarAudioPtr();
    auto AIMAP = aiMap::GetInstance();
    auto policeOfficer = reinterpret_cast<aiPoliceOfficer*>(this);

    policeOfficer->aiPoliceOfficer::StopSiren();

    if (policeCarAudio != nullptr && a1)
        policeCarAudio->PlayExplosion();

    AIMAP->policeForce->UnRegisterCop(policeOfficer->GetCar(), policeOfficer->GetFollowedCar());
    *getPtr<WORD>(this, 0x977A) = 0;
    *getPtr<WORD>(this, 0x280) = 3;
}

void aiPoliceOfficerHandler::Update() {
    auto policeOfficer = reinterpret_cast<aiPoliceOfficer*>(this);
    auto car = policeOfficer->GetCar();
    auto carsim = car->GetCarSim();
    auto carPos = car->GetModel()->GetPosition();
    auto level = lvlLevel::GetSingleton();

    if (*getPtr<WORD>(this, 0x977A) != 12) {
        if (level->GetRoomInfo(car->GetModel()->GetRoomId())->Flags & static_cast<int>(RoomFlags::HasWater)) {
            if (level->GetWaterLevel(car->GetModel()->GetRoomId()) > carsim->GetWorldMatrix()->m31) {
                PerpEscapes(0);
                *getPtr<WORD>(this, 0x977A) = 12;
            }
        }
    }

    if (*getPtr<WORD>(this, 0x977A) == 0)
        *getPtr<WORD>(this, 0x280) = 3;

    //call original
    hook::Thunk<0x53DC70>::Call<void>(this);
}

static ConfigValue<bool> cfgFlyingCopFix("FlyingCopFix", true);

void aiPoliceOfficerHandler::Install() {
    maximumNumCops = cfgMaximumCopsLimit.Get();
    if (cfgPoliceAcademyFunding) {
        InstallCallback("aiPoliceOfficer::DetectPerpetrator", "Experimenting with making cops a little smarter about chasing people.",
            &IsPerpDrivingMadly, {
                cb::call(0x53E057), // aiPoliceOfficer::Fov
            }
        );

        InstallCallback("aiPoliceOfficer::DetectPerpetrator", "Experimenting with making cops a little smarter about chasing opponents.",
            &IsOppDrivingMadly, {
                cb::call(0x53E11A), // aiPoliceOfficer::Fov
            }
        );
    }

    InstallCallback("aiPoliceOfficer::PerpEscapes", "Fixes infinite explosion sounds.",
        &PerpEscapes, {
            cb::call(0x53DE83),
            cb::call(0x53DE9F),
            cb::call(0x53DEC1),
        }
    );

    InstallCallback("aiPoliceOfficer::Update", "Fixes ai cops being disabled when they jump over water areas.",
        &Update, {
            cb::call(0x537265),
        }
    );

    // fix aiPoliceOfficer::Collision
    InstallPatch({ 0x8B, 0x91, 0xF4, 0x0, 0x0, 0x0 }, {
        0x53E37E,
    });

    // remove Angels water check
    InstallPatch({ 0xEB }, {
        0x53DD19
    });

    // fix cops being freezed when they're away from the player
    /*
    InstallPatch({ 0x75 }, {
        0x53DF4A
    });
    */

    // fix the physics bug that causes cops to rapidly accelerate while in the air
    if (cfgFlyingCopFix.Get()) {
        InstallPatch({ 0xEB }, {
            0x53DCA7
        });
    }
}

/*
    vehCarAudioHandler
*/
static ConfigValue<float> cfgAirborneTimerThresh("AirborneTimerThreshold", 1.1);
static ConfigValue<float> cfgAirborneSpeedThresh("AirborneSpeedThreshold", 45.0);
static ConfigValue<bool> cfgEnableAirborneCheck("TweakableAirborneCheck", false);

float carAirborneTimer = 0.0f;
float carAirborneThreshold = 1.1f;
float carAirborneSpeedThreshold = 45.0;

bool vehCarAudioHandler::IsAirBorne() {
    return carAirborneTimer > carAirborneThreshold;
}

void vehCarAudioHandler::Update() {
    float elapsedTime = datTimeManager::Seconds;
    auto carAudio = reinterpret_cast<vehCarAudio*>(this);
    auto carSim = carAudio->getCarSim();
    
    // grab only forward/sideways velocity, ignore vertical
    Vector3 vehicleVelo = carSim->GetInstance()->GetVelocity();
    float vehicleMph = sqrtf((vehicleVelo.X * vehicleVelo.X) + (vehicleVelo.Z * vehicleVelo.Z)) * 2.23694f;

    //update timer
    if (carSim->OnGround() || carSim->BottomedOut() || vehicleMph < carAirborneSpeedThreshold) 
    {
        carAirborneTimer = 0.0f;
    }
    else  
    {
        carAirborneTimer += elapsedTime;
    }

    // call original
    hook::Thunk<0x4DC320>::Call<void>(this);
}

void vehCarAudioHandler::Reset() {
    carAirborneTimer = 0.0f;

    // call original
    hook::Thunk<0x4DBE30>::Call<void>(this);
}

void vehCarAudioHandler::Install() {
    if (!cfgEnableAirborneCheck.Get())
        return;

    carAirborneSpeedThreshold = cfgAirborneSpeedThresh.Get();
    carAirborneThreshold = cfgAirborneTimerThresh.Get();

    InstallCallback("vehCarAudio::IsAirBorne", "Better method of vehicle airborne checking.",
        &IsAirBorne, {
            cb::jmp(0x4D16F9),
            cb::jmp(0x4D1705),
            cb::jmp(0x4D1711),
            cb::jmp(0x4D171F),
        }
    );

    InstallCallback("vehCarAudio::Reset", "Better method of vehicle airborne checking.",
        &Reset, {
            cb::call(0x4D19CA),
        }
    );

    InstallVTableHook("vehCarAudio::Update",
        &Update, {
            0x5B319C
        }
    );
}


/*
    vehCarDamage
*/

static ConfigValue<bool> cfgBrokenWheels("PhysicalBrokenWheels", true);

void vehCarDamageHandler::Update() {
    auto carDamage = reinterpret_cast<vehCarDamage*>(this);
    auto car = carDamage->getCar();
    auto carsim = car->GetCarSim();
    auto model = car->GetModel();

    for (int i = 0; i < 4; i++)
    {
        auto wheel = carsim->GetWheel(i);

        int wheelStatusFlag = 1 << (i * 3);

        if ((model->GetWheelBrokenStatus() & wheelStatusFlag) != 0) {
            if (wheel->GetRadius() < 0.f) { // Check if the wheel radius is < 0, if so, it's probably done by us, and we need to reset it
                string_buf<16> buffer("whl%d", i);
                Matrix34 outMatrix;
                if (GetPivot(outMatrix, carDamage->GetName(), buffer)) {
                    float halfHeight = (outMatrix.m11 - outMatrix.m01) * 0.5f;
                    wheel->SetRadius(fabs(halfHeight));
                }
            }
        }
        else {
            wheel->SetRadius(-1.f);
            car->GetStuck()->setStuckTime(0.f);
        }
    }

    //call original
    hook::Thunk<0x4CAA40>::Call<void>(this);
}

void vehCarDamageHandler::Install() {
    if (!cfgBrokenWheels.Get())
        return;

    InstallVTableHook("vehCarDamage::Update",
        &Update, {
            0x5B2C30,
        }
    );
}

/*
    mmGearIndicatorHandler
*/

void mmGearIndicatorHandler::Draw() {
    int *gearIndicator = *getPtr<int*>(this, 0x3C);
    int *vehCarPtr = *getPtr<int*>(gearIndicator, 0x284);
    int reverse = *getPtr<int>(vehCarPtr, 0x304);
    int autoTransmission = *getPtr<int>(vehCarPtr, 0x300);
    int gearID = -1;
    float speedMPH = *getPtr<float>(vehCarPtr, 0x24C);
    float throttle = *getPtr<float>(vehCarPtr, 0x2BC);

    if (autoTransmission) {
        if (throttle > 0.f || speedMPH >= 1.f) {
            if (!reverse)
                gearID = 9;
            else if (reverse == 1)
                gearID = 10;
            else
                gearID = 11;
        }
        else {
            gearID = 0;
        }
    }
    if (!autoTransmission) {
        if (!reverse)
            gearID = 9;
        else if (reverse == 1)
            gearID = 10;
        else
            gearID = reverse - 1;
    }

    // get gfxPipeline::CopyBitmap stuff
    int v1 = *getPtr<int>(this, 0x30);
    int v2 = *getPtr<int>(this, 0x34);
    int *v3 = *getPtr<int*>(this, 0x38);
    int v4 = *getPtr<int>(v3, 0x20);
    int v5 = *getPtr<int>(v3, 0x24);
    auto *bitmap = *getPtr<gfxBitmap*>(this, gearID * 4);
    int width = bitmap->Width;
    int height = bitmap ->Height;
    
    gfxPipeline::CopyBitmap(
        v1 + v4,     // destX
        v2 + v5,     // destY
        bitmap,      // bitmap
        0,           // srcX
        0,           // srcY
        width,       // width
        height,      // height
        true);       // srcColorKey
}

void mmGearIndicatorHandler::Install() {
    if (vehCarModel::mm1StyleTransmission) {
        InstallCallback("mmGearIndicatorHandler::Draw", "Adds the unused P gear indicator to the HUD.",
            &Draw, {
                cb::call(0x431B26),
            }
        );
    }

    InstallPatch("Replace N letter with P", {
        0xB8, 0xD4, 0x52, 0x5C, 0x00,    // mov eax, offset aDigitac_gear_p
        0x75, 0x05,                      // jnz short loc_43F132
        0xB8, 0xE8, 0x52, 0x5C, 0x00,    // mov eax, offset aDigitac_gear_0
    }, {
        0x43F126 // mmGearIndicator::Init
    });

    InstallPatch("Replace P letter with N", {
        0xB8, 0x64, 0x52, 0x5C, 0x00,    // mov eax, offset aDigitac_gear_n
        0x75, 0x05,                      // jnz short loc_43F1AF
        0xB8, 0x78, 0x52, 0x5C, 0x00,    // mov eax, offset aDigitac_gear_3
    }, {
        0x43F1A3 // mmGearIndicator::Init
    });
}

/*
    BugfixPatchHandler
*/

void BugfixPatchHandler::Install() {
    InstallPatch("Fixes a bug where the HUD becomes permanently disabled after switching from a POV cam.", {
        0xEB, 0x3A, // jmp short loc_414959
        0x90,       // nop
    }, {
        0x41491D,   // mmGame::UpdateGameInput
    });

    InstallPatch("Fixes a bug where an access violation may occur during Cops 'n Robbers gameplay.", {
        0x51,       // push ecx
        0x89, 0xE0, // mov eax, esp
        0xDB, 0x00, // fild [eax]
        0x90, 0x90, // nop(2)
    }, {
        0x424982,   // mmMultiCR::ImpactCallback
    });
}

/*
    pedAnimationInstanceHandler
*/

hook::Type<float> FrameFraction      = 0x6B4724;
hook::Type<int> FrameDelta           = 0x6B4720;

static float pedAnimFrameRate   = 30.0f;

void pedAnimationInstanceHandler::PreUpdate(float seconds) {
    FrameFraction += (seconds * pedAnimFrameRate);

    float delta = floorf(FrameFraction);
    
    FrameDelta = (int)delta;
    FrameFraction -= delta;
}

void pedAnimationInstanceHandler::aiMapUpdate()
{
    //call preupdate
    pedAnimationInstanceHandler::PreUpdate(datTimeManager::ActualSeconds);

    //call aimap update
    hook::Thunk<0x536E50>::Call<void>(this);
}

void pedAnimationInstanceHandler::Install() {
    InstallCallback("aiMap::Update (Debug Hook)", "Call from mmGame",
        &aiMapUpdate, {
            cb::call(0x4141CF),
        }
    );

    InstallVTableHook("aiMap::Update (Debug Hook)",
        &aiMapUpdate, {
            0x5B5468
        });

    //nop out original call, that got called each frame
    InstallPatch({ 0x90, 0x90, 0x90, 0x90, 0x90, 0x90, 0x90, 0x90, 0x90 }, {
        0x54BF66,
        });
}

/*
    vehPoliceCarAudioBugfixHandler
*/

void vehPoliceCarAudioBugfixHandler::Install() {
    // fixes infinite explosion sounds
    /*InstallPatch({ 0x90, 0x90, 0x90, 0x90, 0x90, 0x90 }, {
        0x4D4C28,
    });*/
}

/*
    aiGoalAvoidPlayerHandler
*/

void aiGoalAvoidPlayerHandler::Install() {
    // fixes traffic reactions when they avoid the player
    InstallPatch({ 0x90, 0x90, 0x90 }, {
        0x56B235,
    });
}

/*
    aiRouteRacerHandler
*/

void aiRouteRacerHandler::Update() {
    auto opponent = reinterpret_cast<aiRouteRacer*>(this);

    if (opponent->Finished())
        *getPtr<int>(this, 0x27C) = 3;

    //call original
    hook::Thunk<0x53D3B0>::Call<void>(this);
}

void aiRouteRacerHandler::Install() {
    InstallCallback("aiRouteRacer::Update", "Fixes opponents fight each other for their spots at the finish line.",
        &Update, {
            cb::call(0x53705B), // aiMap::Update
        }
    );
}