#include "feature_handlers.h"
#include "lua_drawable.h"
#include <imgui\impl\imgui_impl_age.h>
#include <imgui\impl\imgui_impl_win32.h>

using namespace MM2;

static init_handler g_feature_handlers[] = {
    /*
        Initialize the really important handlers
    */

    CreateHandler<gfxPipelineHandler>("gfxPipeline"),
    CreateHandler<memSafeHeapHandler>("memSafeHeap"),

    CreateHandler<datCallbackExtensionHandler>("datCallback Extensions"),

    /*
        Initialize the rest of the handlers
        Order doesn't really matter, just whatever looks neat
    */

    CreateHandler<asCullManagerHandler>("asCullManager"),

    CreateHandler<cityLevelHandler>("cityLevel"),
    CreateHandler<cityTimeWeatherLightingHandler>("cityTimeWeatherLighting"),

    CreateHandler<BridgeFerryHandler>("gizBridge/gizFerry"),

    CreateHandler<gizFerryHandler>("gizFerry"),
    CreateHandler<gizParkedCarMgrHandler>("gizParkedCarMgr"),

    CreateHandler<mmHudMapFeatureHandler>("mmHudMapFeatureHandler"),
    CreateHandler<mmIconsHandler>("mmIconsHandler"),
    CreateHandler<mmDashViewHandler>("mmDashView"),
    CreateHandler<mmExternalViewHandler>("mmExternalView"),
    CreateHandler<mmDirSndHandler>("mmDirSnd"),
    CreateHandler<mmPlayerHandler>("mmPlayer"),
    CreateHandler<mmGameHandler>("mmGame"),
    CreateHandler<mmGameMusicDataHandler>("mmGameMusicData"),
    CreateHandler<Aud3DObjectManagerHandler>("Aud3DObjectManagerHandler"),
    CreateHandler<mmArrowHandler>("mmArrowHandler"),
    CreateHandler<mmSingleRaceHandler>("mmSingleRace"),
    CreateHandler<mmSingleBlitzHandler>("mmSingleBlitz"),
    CreateHandler<mmSingleCircuitHandler>("mmSingleCircuit"),
    CreateHandler<mmSingleStuntHandler>("mmSingleStunt"),
    CreateHandler<mmSingleRoamHandler>("mmSingleRoam"),

    CreateHandler<dgBangerInstanceHandler>("dgBangerInstance"),
    CreateHandler<ltLensFlareHandler>("ltLensFlare"),

    CreateHandler<vehCarHandler>("vehCar"),
    CreateHandler<vehCarAudioContainerHandler>("vehCarAudioContainer"),
    CreateHandler<vehPoliceCarAudioHandler>("vehPoliceCarAudio"),
    CreateHandler<vehBreakableMgrHandler>("vehBreakableMgr"),
    CreateHandler<vehCarModelFeatureHandler>("vehCarModel"),
    CreateHandler<vehCarSimHandler>("vehCarSim"),
    CreateHandler<vehWheelHandler>("vehWheel"),
    CreateHandler<vehTrailerFeatureHandler>("vehTrailer"),
    CreateHandler<vehTrailerInstanceFeatureHandler>("vehTrailerInstance"),
    CreateHandler<vehCableCarInstanceHandler>("vehCableCarInstance"),
    CreateHandler<vehSirenHandler>("vehSiren"),
    CreateHandler<vehEngineFeatureHandler>("vehEngine"),

    CreateHandler<Dialog_NewPlayerHandler>("New player dialog"),

    CreateHandler<lvlHandler>("Propulator"),
    CreateHandler<sdlPage16Handler>("sdlPage16"),
    CreateHandler<vglHandler>("VGL drawing"),

    CreateHandler<StreamHandler>("Stream"),
    CreateHandler<TextureVariantHandler>("Texture variants"),

    CreateHandler<mmCityListHandler>("mmCityList"),

    CreateHandler<PUMainHandler>("PUMain"),

    CreateHandler<fxTexelDamageHandler>("fxTexelDamage"),

    CreateHandler<pedestrianInstanceHandler>("pedestrianInstanceHandler"),

    CreateHandler<aiVehicleInstanceFeatureHandler>("aiVehicleInstance"),
    CreateHandler<aiPoliceOfficerFeatureHandler>("aiPoliceOfficer"),

    CreateHandler<luaDrawableHandler>("luaDrawableHandler")
};

#ifndef FEATURES_DECLARED
#define FEATURES_DECLARED
#endif
