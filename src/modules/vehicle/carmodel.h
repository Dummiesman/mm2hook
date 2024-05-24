#pragma once
#include <modules\level\inst.h>
#include <modules\effects\texeldamage.h>
#include <modules\effects\damage3d.h>

#include "car.h"
#include "breakable.h"
#include "siren.h"
#include "cardamage.h"
#include "bound.h"

namespace MM2
{
    // Forward declarations
    class vehCarModel;

    // External declarations
    extern class dgPhysEntity;
    extern class lvlInstance;
    extern struct fxTexelDamage;
    extern class vehSiren;
    extern class vehCarDamage;

    // Class definitions

    class vehCarModel : public lvlInstance {
    public:
        /*
            Model Index Constants
        */
        static const int SHADOW_GEOM_ID = 1;
        static const int HLIGHT_GEOM_ID = 2;
        static const int TLIGHT_GEOM_ID = 3;
        static const int RLIGHT_GEOM_ID = 4;
        static const int SLIGHT0_GEOM_ID = 5;
        static const int SLIGHT1_GEOM_ID = 6;
        static const int BLIGHT_GEOM_ID = 7;
        static const int BODYDAMAGE_GEOM_ID = 8;
        static const int SIREN0_GEOM_ID = 9;
        static const int SIREN1_GEOM_ID = 10;
        static const int DECAL_GEOM_ID = 11;
        static const int DRIVER_GEOM_ID = 12;
        static const int SHOCK0_GEOM_ID = 13;
        static const int SHOCK1_GEOM_ID = 14;
        static const int SHOCK2_GEOM_ID = 15;
        static const int SHOCK3_GEOM_ID = 16;
        static const int ARM0_GEOM_ID = 17;
        static const int ARM1_GEOM_ID = 18;
        static const int ARM2_GEOM_ID = 19;
        static const int ARM3_GEOM_ID = 20;
        static const int SHAFT2_GEOM_ID = 21;
        static const int SHAFT3_GEOM_ID = 22;
        static const int AXLE0_GEOM_ID = 23;
        static const int AXLE1_GEOM_ID = 24;
        static const int ENGINE_GEOM_ID = 25;
        static const int WHL0_GEOM_ID = 26;
        static const int WHL1_GEOM_ID = 27;
        static const int WHL2_GEOM_ID = 28;
        static const int WHL3_GEOM_ID = 29;
        static const int BREAK0_GEOM_ID = 30;
        static const int BREAK1_GEOM_ID = 31;
        static const int BREAK2_GEOM_ID = 32;
        static const int BREAK3_GEOM_ID = 33;
        static const int BREAK01_GEOM_ID = 34;
        static const int BREAK12_GEOM_ID = 35;
        static const int BREAK23_GEOM_ID = 36;
        static const int BREAK03_GEOM_ID = 37;
        static const int HUB0_GEOM_ID = 38;
        static const int HUB1_GEOM_ID = 39;
        static const int HUB2_GEOM_ID = 40;
        static const int HUB3_GEOM_ID = 41;
        static const int TRAILER_HITCH_GEOM_ID = 42;
        static const int SRN0_GEOM_ID = 43;
        static const int SRN1_GEOM_ID = 44;
        static const int SRN2_GEOM_ID = 45;
        static const int SRN3_GEOM_ID = 46;
        static const int HEADLIGHT0_GEOM_ID = 47;
        static const int HEADLIGHT1_GEOM_ID = 48;
        static const int FNDR0_GEOM_ID = 49;
        static const int FNDR1_GEOM_ID = 50;
        static const int WHL4_GEOM_ID = 51;
        static const int WHL5_GEOM_ID = 52;
        static const int PLIGHTON_GEOM_ID = 53;
        static const int PLIGHTOFF_GEOM_ID = 54;
        static const int SWHL0_GEOM_ID = 55;
        static const int SWHL1_GEOM_ID = 56;
        static const int SWHL2_GEOM_ID = 57;
        static const int SWHL3_GEOM_ID = 58;
        static const int SWHL4_GEOM_ID = 59;
        static const int SWHL5_GEOM_ID = 60;
        static const int HUB4_GEOM_ID = 61;
        static const int HUB5_GEOM_ID = 62;
        static const int FNDR2_GEOM_ID = 63;
        static const int FNDR3_GEOM_ID = 64;
        static const int FNDR4_GEOM_ID = 65;
        static const int FNDR5_GEOM_ID = 66;
        static const int SHUB0_GEOM_ID = 67;
        static const int SHUB1_GEOM_ID = 68;
        static const int SHUB2_GEOM_ID = 69;
        static const int SHUB3_GEOM_ID = 70;
        static const int SHUB4_GEOM_ID = 71;
        static const int SHUB5_GEOM_ID = 72;
        static const int HEADLIGHT2_GEOM_ID = 73;
        static const int HEADLIGHT3_GEOM_ID = 74;
        static const int HEADLIGHT4_GEOM_ID = 75;
        static const int HEADLIGHT5_GEOM_ID = 76;
        static const int HEADLIGHT6_GEOM_ID = 77;
        static const int HEADLIGHT7_GEOM_ID = 78;
        static const int SRN4_GEOM_ID = 79;
        static const int SRN5_GEOM_ID = 80;
        static const int SRN6_GEOM_ID = 81;
        static const int SRN7_GEOM_ID = 82;
        static const int LIGHTBAR0_GEOM_ID = 83;
        static const int LIGHTBAR1_GEOM_ID = 84;
        static const int TSLIGHT0_GEOM_ID = 85;
        static const int TSLIGHT1_GEOM_ID = 86;
        static const int VARIANT_BASE_GEOM_ID = 87;
    public:
        static bool EnableSpinningWheels;
        static bool EnableHeadlightFlashing;
        static bool MWStyleTotaledCar;
        static int SirenType;
        static int HeadlightType;
        static float SirenCycle;
        static float HeadlightFlashingSpeed;
        static bool PartReflections;
        static bool WheelReflections;
        static bool mm1StyleTransmission; //god this is horrible...
        static bool breakableRenderTweak;
        static bool Enable3DShadows;

        //light states
        static bool ShowHeadlights;
        static bool HazardLightsState;
        static bool LeftSignalLightState;
        static bool RightSignalLightState;

        //3d damage feature
        static bool Enable3DDamage;
    private:
        bool enabledElectrics[4]; //0/1 are back, 2/3 are front
        vehCar* car;
        vehCarSim* carSim;
        fxTexelDamage* texelDamage;
        byte variant;
        byte char_25;
        byte char_26;
        byte char_27;
        int dword_28;
        float dummyWheelOffset;
        Vector3 fndr0offset;
        Vector3 fndr1offset;
        Vector3 trailerHitchPosition;
        int dword_54;
        int dword_58;
        int dword_5c;
        int dword_60;
        int dword_64;
        int dword_68;
        int dword_6c;
        int dword_70;
        int dword_74;
        int dword_78;
        int dword_7c;
        int dword_80;
        int dword_84;
        int dword_88;
        int dword_8c;
        Vector3 weirdOffsetPos;
        int dword_9c;
        vehBreakableMgr* genBreakableMgr;
        bool hasEjectedOneshot;
        char pad[3];
        int wheelBrokenStatus;
        vehBreakableMgr* wheelBreakableMgr;
        ltLight* headlights;
        Vector3 headlightPositions[2];
        //EXTRA FIELDS. The hook expands on this class, this is only possible because it's only used like a pointer in the original MM code
        //These are the position differences from (FNDR2-WHL2) and (FNDR3-WHL3) / (FNDR4-WHL4) and (FNDR5-WHL5)
        Vector3 fndr2offset;
        Vector3 fndr3offset;
        Vector3 fndr4offset;
        Vector3 fndr5offset;
        ltLight* extraHeadlights[6]; //HEADLIGHT2-7
        Vector3 extraHeadlightPositions[6];
        fxDamage3D* damage3D;
    public:
        AGE_API vehCarModel();
        AGE_API ~vehCarModel();

        vehBreakableMgr* GetGenBreakableMgr();
        vehBreakableMgr* GetMechBreakableMgr();
        vehCar* GetCar();
        int GetVariant();
        ltLight* GetHeadlight(int index);
        int GetWheelBrokenStatus();
        Vector3 GetTrailerHitchOffset();
        fxTexelDamage* GetTexelDamage();
        fxDamage3D* GetDamage3D();

        AGE_API void GetSurfaceColor(modStatic* model, Vector3* outVector);
        AGE_API void InitBreakable(vehBreakableMgr* manager, const char* basename, const char* breakableName, int geomId, int someId);
        AGE_API void InitSirenLight(const char* basename, const char* mtxName, int geomId);
        void InitSirenLights(const char* basename);
        void InitHeadlights(const char* basename);
        AGE_API void BreakElectrics(Vector3* localImpactPos);
        AGE_API void ClearDamage();
        AGE_API void EjectOneshot();
        AGE_API bool GetVisible();
        AGE_API void SetVisible(bool visible);
        AGE_API void DrawHeadlights(bool rotate);
        AGE_API void DrawExtraHeadlights(bool rotate);
        AGE_API void DrawPart(modStatic* model, const Matrix34* matrix, modShader* shaders);
        AGE_API void DrawPart(int lod, int geomId, const Matrix34* matrix, modShader* shaders);
        void DrawPartReflected(int lod, int geomId, const Matrix34* matrix, modShader* shaders);
        void DrawPart(int lod, int geomId, const Matrix34* matrix, modShader* shaders, bool reflected);
        void Init(vehCar* car, const char* basename, int variant);
        
        /*
            lvlInstance virtuals
        */
        AGE_API void Reset()  override;
        AGE_API const Vector3& GetPosition() override;
        AGE_API const Matrix34& GetMatrix(Matrix34* a1);
        AGE_API void SetMatrix(const Matrix34& a1) override;
        AGE_API void SetVariant(int variant) override;
        AGE_API dgPhysEntity* GetEntity() override;
        AGE_API dgPhysEntity* AttachEntity() override;
        AGE_API const Vector3& GetVelocity() override;
        AGE_API void Draw(int lod) override;
        AGE_API void DrawShadow() override;
        AGE_API void DrawShadowMap() override;
        AGE_API void DrawGlow() override;
        AGE_API void DrawReflected(float a1) override;
        AGE_API unsigned int SizeOf() override;

        static void BindLua(LuaState L);
    };
}