#pragma once
#include "common.h"
#include "patch.h"
#include "mm2.h"

// Individual handlers
#include "bugfix_handlers/aiCTFRacerHandler.h"
#include "bugfix_handlers/aiPathHandler.h"
#include "bugfix_handlers/aiPedestrianHandler.h"
#include "bugfix_handlers/aiPoliceForceHandler.h"
#include "bugfix_handlers/aiVehicleAmbientHandler.h"
#include "bugfix_handlers/aiVehicleInstanceHandler.h"
#include "bugfix_handlers/aiVehiclePhysicsHandler.h"
#include "bugfix_handlers/asMeshCardInfoHandler.h"
#include "bugfix_handlers/audManagerHandler.h"
#include "bugfix_handlers/cityLevelBugfixHandler.h"
#include "bugfix_handlers/CityListHandler.h"
#include "bugfix_handlers/fxShardManagerBugfixHandler.h"
#include "bugfix_handlers/gfxImageHandler.h"
#include "bugfix_handlers/lvlSkyHandler.h"
#include "bugfix_handlers/memMemoryAllocatorHandler.h"
#include "bugfix_handlers/mmWaypointInstanceHandler.h"
#include "bugfix_handlers/mmCDPlayerHandler.h"
#include "bugfix_handlers/mmHudMapHandler.h"
#include "bugfix_handlers/mmInterfaceHandler.h"
#include "bugfix_handlers/mmMirrorHandler.h"
#include "bugfix_handlers/mmMultiCRHandler.h"
#include "bugfix_handlers/mmPopupHandler.h"
#include "bugfix_handlers/mmSpeedIndicatorHandler.h"
#include "bugfix_handlers/mmTimerHandler.h"
#include "bugfix_handlers/mmViewMgrBugfixHandler.h"
#include "bugfix_handlers/mpConsistencyHandler.h"
#include "bugfix_handlers/modShaderHandler.h"
#include "bugfix_handlers/NetSelectMenuHandler.h"
#include "bugfix_handlers/vehBreakableMgrBugfixHandler.h"
#include "bugfix_handlers/vehCarAudioHandler.h"
#include "bugfix_handlers/vehCarAudioContainerBugfixHandler.h"
#include "bugfix_handlers/vehSuspensionHandler.h"
#include "bugfix_handlers/vehTrailerHandler.h"
#include "bugfix_handlers/vehSemiCarAudioBugfixHandler.h"
#include "bugfix_handlers/vehSplashHandler.h"

class aiPoliceOfficerHandler {
public:
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

class aiGoalAvoidPlayerHandler {
public:
    static void Install();
};

class aiRouteRacerHandler {
public:
    void Update();
    static void Install();
};