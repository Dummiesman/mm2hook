#pragma once
#include "common.h"
#include "patch.h"
#include "mm2.h"
#include "..\modules\effects.h"

// Individual handlers
#include "bugfix_handlers/aiPoliceForceHandler.h"
#include "bugfix_handlers/aiVehicleAmbientHandler.h"
#include "bugfix_handlers/audManagerHandler.h"
#include "bugfix_handlers/gfxImageHandler.h"
#include "bugfix_handlers/lvlSkyHandler.h"
#include "bugfix_handlers/mmBillInstanceHandler.h"
#include "bugfix_handlers/mmHudMapHandler.h"
#include "bugfix_handlers/mmInterfaceHandler.h"
#include "bugfix_handlers/mmPopupHandler.h"
#include "bugfix_handlers/mpConsistencyHandler.h"
#include "bugfix_handlers/modShaderHandler.h"
#include "bugfix_handlers/vehTrailerHandler.h"

static ConfigValue<bool> cfgAirbrakeFix             ("AirbrakeFix",               true);
static ConfigValue<bool> cfgMissingDashboardFix     ("MissingDashboardFix",       true);

class aiPedestrianHandler
{
public:
    void Update();

    static void Install();
};

class aiPoliceOfficerHandler {
public:
    BOOL IsPerpDrivingMadly(MM2::vehCar *perpCar);
    BOOL IsOppDrivingMadly(MM2::vehCar *perpCar);
    BOOL OffRoad(MM2::vehCar *perpCar);
    void PerpEscapes(bool a1);
    void Update();

    static void Install();
};

class aiPathHandler {
public:
    void UpdatePedestrians();

    static void Install();
};

class vehCarAudioHandler {
public:
    bool IsAirBorne();
    void Update();
    void Reset();

    static void Install();
};

class vehCarAudioContainerBugfixHandler : handler_t {
public:
    void StartSiren();
    void StopSiren();

    static void Install(void);
};

class vehCarDamageHandler {
public:
    void Update();
    static void Install();
};

class mmGearIndicatorHandler
{
public:
    void Draw();
    static void Install();
};

class mmSpeedIndicatorHandler
{
public:
    static void Install();
};

class cityLevelBugfixHandler
{
public:
    static bool IsMirrorDrawing;
    static void UpdateRainParticles();
    static MM2::Stream* OpenPvsStream(const char * folder, const char * file, const char * extension, bool a4, bool a5);
    static void Install();
};

class BugfixPatchHandler {
public:
    static void Install();
};

class mmMultiCRHandler {
public:
    bool LoadMusic(char* a1, char* a2);
    static void Install();
};

class pedAnimationInstanceHandler {
public:
    void aiMapUpdate();

    static void PreUpdate(float seconds);

    static void Install();
};

class mmCDPlayerHandler {
public:
    static void Install();
};

class vehPoliceCarAudioBugfixHandler {
public:
    static void Install();
};

class vehSemiCarAudioBugfixHandler {
public:
    void SetNon3DParams();
    void UpdateAirBlow();
    void UpdateReverse();
    void Init(MM2::vehCarSim* carsim, MM2::vehCarDamage* cardamage, char* basename, bool a5, bool a6, bool a7);
    static void Install();
};

class asMeshCardInfoHandler {
public:
    static void Install();
};

class aiVehicleInstanceHandler {
public:
    void Reset();
    static void Install();
};

class aiGoalAvoidPlayerHandler {
public:
    static void Install();
};

class aiRouteRacerHandler {
public:
    void Update();
    void Reset();
    static void Install();
};

class mmMirrorHandler {
public:
    void Cull();
    static void Install();
};

class mmViewMgrBugfixHandler {
public:
    void SetViewSetting(int a1);
    static void Install();
};

class mmPlayerBugfixHandler {
public:
    void Ctor();
    static void Install();
};

class fxShardManagerBugfixHandler {
public:
    static void Install();
};