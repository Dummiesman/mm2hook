#include "bugfix_handlers.h"

using namespace MM2;

static init_handler g_bugfix_handlers[] = {
    CreateHandler<aiPathHandler>("aiPath"),
    CreateHandler<aiPedestrianHandler>("aiPedestrian"),
    CreateHandler<aiPoliceForceHandler>("aiPoliceForce"),
    CreateHandler<aiPoliceOfficerHandler>("aiPoliceOfficer"),
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
    CreateHandler<mmPlayerBugfixHandler>("mmPlayer"),
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
    CreateHandler<fxShardManagerBugfixHandler>("fxShardManager")
};

/*
    aiPathHandler
*/

int numPedUpdateAttempts = 0;

void aiPathHandler::UpdatePedestrians(void) {
    auto path = reinterpret_cast<aiPath*>(this);
    numPedUpdateAttempts = 0;

    path->UpdatePedestrians();
}

void aiPathHandler::Install() {
    InstallCallback("aiPath::UpdatePedestrians", "Limits the number of update attempts for pedestrians.",
        &UpdatePedestrians, {
            cb::call(0x536FE0), // aiMap::Update
        }
    );
}

/*
    aiPedestrianHandler
*/

static ConfigValue<int> cfgMaxPedUpdateAttempts("MaxPedUpdateAttempts", 256);

void aiPedestrianHandler::Update(void) {
    if (numPedUpdateAttempts < cfgMaxPedUpdateAttempts) {
        ++numPedUpdateAttempts;
        $::aiPedestrian::Update(this);
    }
}

void aiPedestrianHandler::Install() {
    InstallCallback("aiPedestrian::Update", "Limits the number of update attempts for a pedestrian.",
        &Update, {
            cb::call(0x544191), // aiPath::UpdatePedestrians
        }
    );
}

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
    auto AIMAP = &aiMap::Instance;

    // check players
    for (int i = 0; i < 4; i++) {
        auto player = &AIMAP->players[i];

        if (player->getCar() == car)
            return player;
    }

    // check opponents
    for (int i = 0; i < AIMAP->numOpponents; i++) {
        auto opponent = AIMAP->opponents[i];

        if (opponent->getCar() == car)
            return opponent;
    }

    // not found :(
    return nullptr;
}

float getSpeedLimit(vehCar *car) {
    auto AIMAP = &aiMap::Instance;
    auto veh = findVehicle(car);

    if (veh != nullptr) {
        auto roadId = veh->CurrentRoadId();
        auto path = AIMAP->paths[roadId];

        return *getPtr<float>(path, 0x18);
    }

    return cfgDefaultSpeedLimit;
}

float burnoutTime(vehCar *car) {
    float speed = car->getCarSim()->getSpeedMPH();

    for (int i = 0; i < 4; i++)
    {
        auto wheel = car->getCarSim()->getWheel(i);

        if (fabs(wheel->getRotationRate()) > 26.f && speed < 10.f)
            return burnoutTimer += datTimeManager::Seconds;
    }

    return burnoutTimer = 0.f;
}

float hornPlayTime(vehCar *car) {
    auto carAudio = car->getAudio()->GetCarAudioPtr();
    auto hornSound = *getPtr<AudSoundBase*>(carAudio, 0x10C);

    if (hornSound->IsPlaying())
        return soundPlayTime += datTimeManager::Seconds;

    return soundPlayTime = 0.f;
}

BOOL aiPoliceOfficerHandler::OffRoad(vehCar *car) {
    auto AIMAP = &aiMap::Instance;
    auto veh = findVehicle(car);
    float outVal = 0.f;

    if (veh != nullptr) {
        auto roomId = car->getModel()->getRoomId();
        auto roadId = veh->CurrentRoadId();
        auto path = AIMAP->paths[roadId];

        if (path->IsPosOnRoad(&car->getCarSim()->getICS()->getPosition(), 0.f, &outVal) > 1 && roomId < 900)
            return TRUE;
    }

    for (int i = 0; i < 4; i++)
    {
        auto wheel = car->getCarSim()->getWheel(i);

        if (!strcmp(wheel->getCurrentPhysicsMaterial()->getName(), "grass"))
            return TRUE;
    }

    return FALSE;
}

BOOL aiPoliceOfficerHandler::IsPerpDrivingMadly(vehCar *perpCar) {
    char *vehName = perpCar->getCarDamage()->GetName(); // we can't use vehCarSim because the game forces vpcop to vpmustang99...

    // ignore perp if they're a cop
    if (!hook::StaticThunk<0x4D1A70>::Call<bool>(vehName)) {
        if (vehPoliceCarAudio::iNumCopsPursuingPlayer < maximumNumCops || maximumNumCops <= 0) {
            if (hook::Thunk<0x53E2A0>::Call<BOOL>(this, perpCar))
            {
                float speed = perpCar->getCarSim()->getSpeedMPH();
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
        float speed = perpCar->getCarSim()->getSpeedMPH();
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

void aiPoliceOfficerHandler::PerpEscapes(bool a1) {
    auto carAudioContainer = *getPtr<vehCarAudioContainer*>(this, 0x268);
    auto policeCarAudio = carAudioContainer->GetPoliceCarAudioPtr();
    auto AIMAP = &aiMap::Instance;

    $::aiPoliceOfficer::StopSiren(this);

    if (policeCarAudio != nullptr && a1)
        policeCarAudio->PlayExplosion();

    AIMAP->policeForce->UnRegisterCop(*getPtr<vehCar*>(this, 0x14), *getPtr<vehCar*>(this, 0x9774));
    *getPtr<WORD>(this, 0x977A) = 0;
    *getPtr<WORD>(this, 0x280) = 3;
}

void aiPoliceOfficerHandler::Update() {
    auto policeOfficer = reinterpret_cast<aiPoliceOfficer*>(this);
    auto vehiclePhysics = policeOfficer->getVehiclePhysics();
    auto car = vehiclePhysics->getCar();
    auto carsim = car->getCarSim();
    auto carPos = car->getModel()->GetPosition();
    auto singleton = *lvlLevel::Singleton;

    if (*getPtr<WORD>(this, 0x977A) != 12) {
        if (**(BYTE**)(*getPtr<int>(lvlLevel::Singleton, 8) + 4 * car->getModel()->getRoomId()) & 4) {
            if (singleton->GetWaterLevel(0) > carsim->getWorldMatrix()->m31) {
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
    InstallPatch({ 0x75 }, {
        0x53DF4A
    });

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
    auto vehicleVelo = carSim->getInstance()->GetVelocity();
    float vehicleMph = sqrtf((vehicleVelo->X * vehicleVelo->X) + (vehicleVelo->Z * vehicleVelo->Z)) * 2.23694f;

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
    vehCarAudioContainerBugfixHandler
*/

void vehCarAudioContainerBugfixHandler::StartSiren() {
    auto policeAudio = get<vehCarAudioContainer>()->GetPoliceCarAudioPtr();

    if (policeAudio != nullptr) {
        // vehPoliceCarAudio::StartSiren
        hook::Thunk<0x4D4B20>::ThisCall<void>(policeAudio, 0);
    }
}

void vehCarAudioContainerBugfixHandler::StopSiren() {
    auto policeAudio = get<vehCarAudioContainer>()->GetPoliceCarAudioPtr();

    if (policeAudio != nullptr) {
        // vehPoliceCarAudio::StopSiren
        hook::Thunk<0x4D4C20>::ThisCall<void>(policeAudio);
    }
}

void vehCarAudioContainerBugfixHandler::Install() {
    InstallCallback("vehCarAudioContainer::StartSiren", "Fixes a crash caused by activating sirens on a vehicle with missing audio." ,
        &StartSiren, {
            cb::call(0x4145FB), // mmGame::UpdateHorn
            cb::call(0x43D533), // mmNetObject::PositionUpdate
        }
    );

    InstallCallback("vehCarAudioContainer::StopSiren", "Fixes a crash caused by deactivating sirens on a vehicle with missing audio." ,
        &StopSiren, {
            cb::call(0x41460C), // mmGame::UpdateHorn
            cb::call(0x43D562), // mmNetObject::PositionUpdate
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
    auto carsim = car->getCarSim();
    auto model = car->getModel();

    for (int i = 0; i < 4; i++)
    {
        auto wheel = carsim->getWheel(i);

        int wheelStatusFlag = 1 << (i * 3);

        if ((model->getWheelBrokenStatus() & wheelStatusFlag) != 0) {
            if (wheel->getRadius() < 0.f) { // Check if the wheel radius is < 0, if so, it's probably done by us, and we need to reset it
                string_buf<16> buffer("whl%d", i);
                Matrix34 outMatrix;
                if (GetPivot(outMatrix, carDamage->GetName(), buffer)) {
                    float halfHeight = (outMatrix.m11 - outMatrix.m01) * 0.5f;
                    wheel->setRadius(fabs(halfHeight));
                }
            }
        }
        else {
            wheel->setRadius(-1.f);
            car->getStuck()->setStuckTime(0.f);
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
    cityLevelBugfixHandler
*/

hook::Type<asParticles*> sm_RainParticles(0x62770C);
hook::Type<bool> sm_EnablePVS(0x62B070);
bool cityLevelBugfixHandler::IsMirrorDrawing = false;

Stream* cityLevelBugfixHandler::OpenPvsStream(const char * folder, const char * file, const char * extension, bool a4, bool a5) {
    //open stream
    auto stream = hook::StaticThunk<0x4C58C0>::Call<Stream*>(folder, file, extension, a4, a5);
    
    //stream will be NULL if the PVS doesn't exist
    if (!stream) {
        sm_EnablePVS = false;
    }

    //return original stream
    return stream;
}

void cityLevelBugfixHandler::UpdateRainParticles() {
    asParticles* rainParticles = (asParticles*)sm_RainParticles;

    // set position if appropriate
    if (!IsMirrorDrawing) {
        Vector4 dotWith = Vector4(0.0, 10.0, -10.0, 1.0);
        
        Vector4 newParticlePosition = Vector4(0, 0, 0, 0);
        newParticlePosition.Dot(dotWith, *(Matrix44*)gfxRenderState::sm_Camera);

        rainParticles->pBirthRule->Position.X = newParticlePosition.X;
        rainParticles->pBirthRule->Position.Y = newParticlePosition.Y;
        rainParticles->pBirthRule->Position.Z = newParticlePosition.Z;
    }
    
    // render particles
    rainParticles->Cull();
}

void cityLevelBugfixHandler::Install() {
    InstallCallback("cityLevel::Load", "Disables PVS when it doesn't exist.",
        &OpenPvsStream, {
            cb::call(0x4440E8), // cityLevel::Load
        }
    );

    InstallCallback("lvlLevel::Draw", "Allows for control over when to update rain particle position.",
        &UpdateRainParticles, {
            cb::call(0x4462B7),
        }
    );
    mem::nop(0x4462BA + 0x02, 76); // nop out the rest of the rain update, since we're replacing it
}

/*
    mmMirrorHandler
*/

void mmMirrorHandler::Cull() {
    cityLevelBugfixHandler::IsMirrorDrawing = true;
    hook::Thunk<0x42B8C0>::Call<void>(this); // call original
    cityLevelBugfixHandler::IsMirrorDrawing = false;
}

void mmMirrorHandler::Install() {
    InstallVTableHook("mmMirror::Cull",
        &Cull, {
            0x5B0B80 ,
        }
    );
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
    if (ioKeyboard::GetKeyState(DIK_0) == 0)
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
    asMeshCardInfoHandler
*/

void asMeshCardInfoHandler::Install()
{
    InstallCallback("asMeshCardInfo::Draw", "Scales particles correctly based on current cull mode.",
        &asMeshCardInfo::Draw, {
            cb::jmp(0x461770),
        }
    );
}

/*
    aiVehicleInstanceHandler
*/

void aiVehicleInstanceHandler::Reset()
{
    *getPtr<byte>(this, 0x1A) = 0;
    hook::Thunk<0x552100>::Call<void>(this); // call original
}

void aiVehicleInstanceHandler::Install()
{
    // fixes four ways persisting after level reset
    InstallVTableHook("aiVehicleInstance::DrawGlow",
        &Reset, {
            0x5B590C,
        }
    );
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

static ConfigValue<int> cfgBustedTarget("BustedTarget", 3);
static ConfigValue<float> cfgBustedMaxSpeed("BustedMaxSpeed", 20.f);
static ConfigValue<float> cfgBustedTimeout("BustedTimeout", 4.f);
int aiOppBustedTarget = 3;
float aiOppBustedMaxSpeed = 20.f;
float aiOppBustedTimeout = 4.f;
float aiOppBustedTimer = 0.f;

void aiRouteRacerHandler::Update() {
    auto opponent = reinterpret_cast<aiRouteRacer*>(this);

    if (opponent->Finished())
        *getPtr<int>(this, 0x27C) = 3;

    if (aiOppBustedTarget >= 2) {
        auto car = opponent->getCar();
        auto carsim = car->getCarSim();
        auto AIMAP = &aiMap::Instance;

        for (int i = 0; i < AIMAP->numCops; i++)
        {
            auto police = AIMAP->Police(i);
            auto copCar = police->getVehiclePhysics()->getCar();
            auto curDamage = car->getCarDamage()->getCurDamage();
            auto maxDamage = car->getCarDamage()->getMaxDamage();
            auto opponentPos = car->getModel()->GetPosition();
            auto policePos = copCar->getModel()->GetPosition();
            auto policeAud = copCar->getAudio()->GetPoliceCarAudioPtr();

            if (*getPtr<int>(car, 0xEC) != 0 && curDamage < maxDamage)
                continue;

            if (*getPtr<WORD>(police, 0x977A) != 0 && *getPtr<WORD>(police, 0x977A) != 12) {
                if (*getPtr<vehCar*>(police, 0x9774) == opponent->getCar()) {
                    if (*getPtr<int>(this, 0x27C) != 3) {
                        if (opponentPos.Dist(policePos) <= 12.5f) {
                            if (carsim->getSpeedMPH() <= aiOppBustedMaxSpeed) {
                                aiOppBustedTimer += datTimeManager::Seconds;
                                if (aiOppBustedTimer > aiOppBustedTimeout) {
                                    *getPtr<int>(this, 0x27C) = 3;
                                }
                            }
                            else {
                                aiOppBustedTimer = 0.f;
                            }
                        }
                    }
                    if (*getPtr<int>(this, 0x27C) == 3) {
                        if (opponent->Finished()) {
                            police->StopSiren();
                        }
                        else {
                            if (policeAud != nullptr)
                                policeAud->StopSiren();
                        }
                        AIMAP->policeForce->UnRegisterCop(*getPtr<vehCar*>(police, 0x14), *getPtr<vehCar*>(police, 0x9774));
                        *getPtr<WORD>(police, 0x977A) = 0;
                        *getPtr<WORD>(police, 0x280) = 3;
                    }
                }
            }
        }
    }

    //call original
    hook::Thunk<0x53D3B0>::Call<void>(this);
}

void aiRouteRacerHandler::Reset() {
    //reset busted timer
    aiOppBustedTimer = 0.f;

    //call original
    hook::Thunk<0x53D390>::Call<void>(this);
}

void aiRouteRacerHandler::Install() {
    aiOppBustedTarget = cfgBustedTarget.Get();
    aiOppBustedMaxSpeed = cfgBustedMaxSpeed.Get();
    aiOppBustedTimeout = cfgBustedTimeout.Get();
    InstallCallback("aiRouteRacer::Update", "Fixes opponents fight each other for their spots at the finish line.",
        &Update, {
            cb::call(0x53705B), // aiMap::Update
        }
    );

    InstallCallback("aiRouteRacer::Reset", "Resets opponent busted timer upon reset.",
        &Reset, {
            cb::call(0x536B4D), // aiMap::Reset
        }
    );
}

/*
    mmPlayerBugfixHandler
*/

void mmPlayerBugfixHandler::Ctor()
{
    //clean up our memory
    memset(this, 0x00, 0x23A4);

    //call ctor original
    hook::Thunk<0x4033D0>::Call<void>(this);
}

void mmPlayerBugfixHandler::Install()
{
    InstallCallback("mmPlayer::mmPlayer", "Clean up player memory on ctor.",
        &Ctor, {
            cb::call(0x415D79),
            cb::call(0x41AE89),
            cb::call(0x41C6E9),
            cb::call(0x41E169),
            cb::call(0x41FAD9),
            cb::call(0x420169),
            cb::call(0x421E09),
            cb::call(0x423A2E),
            cb::call(0x427739),
            cb::call(0x428469),
        }
    );
}

/*
    fxShardManagerBugfixHandler
*/
void fxShardManagerBugfixHandler::Install()
{
    InstallCallback("fxShardManager::Drraw", "Fix crashes with cars with low material count.",
        &fxShardManager::Draw, {
            cb::jmp(0x4602D0)
        }
    );
}
