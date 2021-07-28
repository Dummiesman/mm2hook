#pragma once
#include "common.h"
#include "patch.h"
#include "mm2.h"

// Individual handlers
#include "bugfix_handlers/aiPoliceForceHandler.h"
#include "bugfix_handlers/aiVehicleAmbientHandler.h"
#include "bugfix_handlers/asMeshCardInfoHandler.h"
#include "bugfix_handlers/audManagerHandler.h"
#include "bugfix_handlers/fxShardManagerBugfixHandler.h"
#include "bugfix_handlers/gfxImageHandler.h"
#include "bugfix_handlers/lvlSkyHandler.h"
#include "bugfix_handlers/mmBillInstanceHandler.h"
#include "bugfix_handlers/mmCDPlayerHandler.h"
#include "bugfix_handlers/mmHudMapHandler.h"
#include "bugfix_handlers/mmInterfaceHandler.h"
#include "bugfix_handlers/mmMultiCRHandler.h"
#include "bugfix_handlers/mmPopupHandler.h"
#include "bugfix_handlers/mmSpeedIndicatorHandler.h"
#include "bugfix_handlers/mmViewMgrBugfixHandler.h"
#include "bugfix_handlers/mpConsistencyHandler.h"
#include "bugfix_handlers/modShaderHandler.h"
#include "bugfix_handlers/vehCarAudioContainerBugfixHandler.h"
#include "bugfix_handlers/vehTrailerHandler.h"
#include "bugfix_handlers/vehSemiCarAudioBugfixHandler.h"

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

class pedAnimationInstanceHandler {
public:
    void aiMapUpdate();

    static void PreUpdate(float seconds);

    static void Install();
};

class vehPoliceCarAudioBugfixHandler {
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

class mmPlayerBugfixHandler {
public:
    void Ctor();
    static void Install();
};