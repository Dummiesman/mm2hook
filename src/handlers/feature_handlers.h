#pragma once
#include "common.h"
#include "patch.h"
#include "mm2.h"

#include <vector>

#include "..\events\dispatcher.h"

// Individual handlers
#include "feature_handlers/asCullManagerHandler.h"
#include "feature_handlers/Aud3DObjectManagerHandler.h"
#include "feature_handlers/BridgeFerryHandler.h"
#include "feature_handlers/datCallbackExtensionHandler.h"
#include "feature_handlers/gizFerryHandler.h"
#include "feature_handlers/gizParkedCarMgrHandler.h"
#include "feature_handlers/memSafeHeapHandler.h"
#include "feature_handlers/mmCityListHandler.h"
#include "feature_handlers/mmDashViewHandler.h"
#include "feature_handlers/mmGameMusicDataHandler.h"
#include "feature_handlers/vehBreakableMgrHandler.h"
#include "feature_handlers/vehCarAudioContainerHandler.h"
#include "feature_handlers/vehPoliceCarAudioHandler.h"
#include "feature_handlers/vehTrailerInstanceFeatureHandler.h"
#include "feature_handlers/cityLevelHandler.h"
#include "feature_handlers/cityTimeWeatherLightingHandler.h"
#include "feature_handlers/aiVehicleInstanceFeatureHandler.h"
#include "feature_handlers/aiPoliceOfficerFeatureHandler.h"


class gfxPipelineHandler {
public:
    static bool HandleKeyPress(DWORD vKey);

    static void gfxApplySettings();
    static LRESULT APIENTRY gfxWindowProc(HWND hWnd, UINT uMsg, WPARAM wParam, LPARAM lParam);
    static void gfxWindowMove(bool isOpen);
    static void gfxWindowUpdate(bool isOpen);
    static void SetRes(int width, int height, int cdepth, int zdepth, bool parseArgs);
    static void gfxWindowCreate(LPCSTR lpWindowName);
    static void gfxSetTexReduceSize(int);

    static void Install();
};

class vglHandler {
public:
    static void vglBeginShaded(MM2::gfxDrawMode drawMode, int p1);
    static void vglEndShaded();

    static void Install();
};

class sdlPage16Handler {
public:
    static LPVOID blockPtr;
    static LPVOID attributePtr;

    void Draw(int p1, unsigned int p2);

    // this MUST clean up the stack, hence the stdcall
    static void __stdcall SetAttributePointer(LPVOID lpBlock);
    static void InvalidCmd(LPCSTR, int attr, int subtype);

    static void Install();
};

class lvlHandler {
public:
    void EnumerateSDL(int p1, MM2::SDLIteratorCB iter, void *context);

    static void SetAIRoad(const MM2::lvlSDL * lvlSDL, int road, bool p3);
    static int GetAIRoom(int room);
    static void InvalidCommand(int cmd, void *attrPtr, void *roadPtr);
    static void Install();
};

class mmGameHandler : handler_t {
public:
    void SendChatMessage(char *message);
    void InitWeather(void);
    void UpdateSteeringBrakes(void);
    void UpdateHorn(bool a1);

    static void Install();
};

class mmDirSndHandler {
public:
    static MM2::mmDirSnd* Init(ulong sampleRate, bool enableStero, int p3, int volume, char *deviceName, short p6, short p7);

    static void Install();
};

class mmHudMapFeatureHandler {
public:
    void DrawColoredTri(unsigned int color, const MM2::Matrix34 *a2);
    void DrawWhiteTri(const MM2::Matrix34 *a1);
    void DrawLightOrangeTri(const MM2::Matrix34 *a1);
    void DrawLightGreenTri(const MM2::Matrix34 *a1);
    void DrawIcon(int iconType, const MM2::Matrix34 *matrix);
    void DrawNfsMwPlayerIcon(const MM2::Matrix34 *matrix);
    void DrawNfsMwOpponentIcon(const MM2::Matrix34 *matrix);
    void DrawPlayer();
    void DrawCops();
    void DrawOpponents();

    static void Install();
};

struct OppIconInfo {
    int color;
    char buffer[0x24];
};

ASSERT_SIZEOF(OppIconInfo, 0x28);

class mmIconsHandler {
public:
    void RegisterOpponents(OppIconInfo *icons, int count, void *a3);
    void RegisterOpponents_Blitz(OppIconInfo *icons, int count, void *a3);

    static void Install();
};

class StreamHandler {
public:
    static MM2::Stream * Open(const char *filename, bool readOnly);

    static void Install();
};

class TextureVariantHandler {
public:
    static void InitVariantData();
    static void Reset();

    static MM2::gfxImage * LoadTextureVariant(const char *textureName, bool mipmaps);
    static MM2::gfxImage * PrepareTextureVariant(MM2::gfxImage* image, const char *textureName, bool mipmaps);

    static void InstallTextureVariantHandler();

    static void Install();
};

class PUMainHandler {
public:
    static void Install();
};

class mmPlayerHandler {
public:
    void Splash();
    void Zoink();
    void Update();
    void Reset();
    void PlayExplosion();
    void BustPerp();
    void BustOpp();
    static void Install();
};

class Dialog_NewPlayerHandler {
public:
    static void Install();
};

class ltLensFlareHandler {
public:
    static void Install();
};

class mmSingleRaceHandler {
public:
    void QueueCopVoice(float a1);
    void SetPriority(int a1);
    static void Install();
};

class mmSingleBlitzHandler {
public:
    void SetPriority(int a1);
    static void Install();
};

class mmSingleCircuitHandler {
public:
    void SetPriority(int a1);
    static void Install();
};

class mmSingleStuntHandler {
public:
    void SetPriority(int a1);
    static void Install();
};

class mmSingleRoamHandler {
public:
    void ResetToNearestLocation();
    void EscapeDeepWater();
    void HitWaterHandler();
    void SetPriority(int a1);
    static void Install();
};

class dgBangerInstanceHandler {
public:
    void DrawGlow();
    static void Reset();
    static void Install();
};

class vehCarHandler : handler_t {
public:
    void InitCar(LPCSTR vehName, int a2, int a3, bool a4, bool a5);
    void InitCarAudio(LPCSTR a1, int a2);
    const MM2::phBound * GetModelBound(int a1);

    void Mm1StyleTransmission();
    void Update();

    static void Install(void);
};

class vehCarModelFeatureHandler {
public:
    void Draw(int a1);
    void DrawGlow();
    void ModStaticDraw(MM2::modShader * a1);
    void EjectOneShot();
    static void Install();
};

class vehWheelHandler {
public:
    float GetBumpDisplacement(float a1);
    static void Install();
};

class fxTexelDamageHandler {
public:
    static void Install();
};

class pedestrianInstanceHandler {
public:
    void DrawRagdoll();
    void Draw(int a1);
    bool IsCollidable();
    MM2::phBound * GetBound(int a1);

    void aiMapClean();
    void aiMapInit(char* a1, char* a2, char* a3, const MM2::dgStatePack* a4, int a5, MM2::vehCar* a6, bool a7);

    static void Install();
};


class vehTrailerFeatureHandler {
public:
    static void Install();
};

struct MultiTireTrackContainer {
public:
    MM2::gfxTexture* TrackTexture;
    MM2::gfxTexture* OffroadTrackTexture;
};

class multiTireTrackHandler {
    void UpdateTrack(void* lvlTrackManagerPtr, void* wheel);
    static void Install();
};

class vehCarSimHandler {
public:
    static void Install();
};

class vehCableCarInstanceHandler {
public:
    void DrawShadow();
    void DrawGlow();
    bool BeginGeom(const char* a1, const char* a2, int a3);
    static void Install();
};

class mmArrowHandler {
public:
    void SetShape(LPCSTR modelName, LPCSTR dirName, bool useLVertex, MM2::Vector3* a4);
    static void Install();
};

class vehSirenHandler {
public:
    void Update();
    void Reset();
    void SizeOf();
    static void Install();
};


class mmExternalViewHandler {
public:
    static void Install();
};
