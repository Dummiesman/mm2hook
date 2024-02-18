#pragma once
#include <modules\ai\aiVehicleData.h>
#include <modules\level\inst.h>
#include <modules\vehicle\wheelcheap.h>
#include "aiVehicleSpline.h"

namespace MM2
{
    // Forward declarations
    class aiVehicleActive;
    class aiVehicleManager;
    class aiVehicleInstance;

    // External declarations
    extern class asNode;
    extern class ltLight;

    // Class definitions
    class aiVehicleActive : dgPhysEntity {
    private:
        datCallback* CallBack;
        asParticles Particles;
        asParticles Particles2;
        asBirthRule BirthRule;
        asBirthRule BirthRule2;
        float CurDamage;
        float MaxDamage;
        int field_2D0;
        lvlInstance* Instance;
        phInertialCS ICS;
        phSleep Sleep;
        vehWeelCheap WheelFrontLeft;
        vehWeelCheap WheelFrontRight;
        vehWeelCheap WheelBackLeft;
        vehWeelCheap WheelBackRight;
    public:
        vehWeelCheap* GetWheel(int num);

        //dgPhysEntity overrides
        virtual AGE_API void Update() override;
        virtual AGE_API void PostUpdate() override;
        virtual AGE_API phInertialCS* GetICS() override;
        virtual AGE_API lvlInstance* GetInst() override;
        virtual AGE_API void DetachMe() override;
    };
    ASSERT_SIZEOF(aiVehicleActive, 0xAFC);

    class aiVehicleManager : public asNode {
    private:
        aiVehicleData vehicleDatas[32];
        int numVehicleDatas;
        aiVehicleActive* activeActives[32];
        aiVehicleActive aiVehicleActives[32];
        __int16 attachedCount;
        ltLight* sharedLight;
    public:
        static hook::Type<aiVehicleManager *> Instance;
        static hook::Type<int> SignalClock;

        AGE_API aiVehicleManager(void);
        AGE_API virtual ~aiVehicleManager(void);

        //members
        AGE_API void Init(char *unused);
        AGE_API int AddVehicleDataEntry(LPCSTR name);
        AGE_API void SaveEntry();
        AGE_API void Attach(aiVehicleInstance* instance);
        AGE_API void Detach(aiVehicleActive* active);

        //asNode overrides
        AGE_API void Reset() override;
        AGE_API void Update() override;

        //helpers
        int GetDataCount();
        aiVehicleData *GetData(int num);
        ltLight *GetSharedLight();

        //lua
        static void BindLua(LuaState L);
    };
    ASSERT_SIZEOF(aiVehicleManager, 0x177A4);

    class aiVehicleInstance : public lvlInstance {
    public:
    /*
        Model Index Constants
    */
        static const int SHADOW_GEOM_ID = 1;
        static const int HLIGHT_GEOM_ID = 2;
        static const int TLIGHT_GEOM_ID = 3;
        static const int SLIGHT0_GEOM_ID = 4;
        static const int SLIGHT1_GEOM_ID = 5;
        static const int WHL0_GEOM_ID = 6;
        static const int WHL1_GEOM_ID = 7;
        static const int WHL2_GEOM_ID = 8;
        static const int WHL3_GEOM_ID = 9;
        static const int BREAK0_GEOM_ID = 10;
        static const int BREAK1_GEOM_ID = 11;
        static const int BREAK2_GEOM_ID = 12;
        static const int BREAK3_GEOM_ID = 13;
        static const int HEADLIGHT0_GEOM_ID = 14;
        static const int HEADLIGHT1_GEOM_ID = 15;
        static const int WHL4_GEOM_ID = 16;
        static const int WHL5_GEOM_ID = 17;
        static const int BLIGHT_GEOM_ID = 18;
        static const int PLIGHTON_GEOM_ID = 19;
        static const int PLIGHTOFF_GEOM_ID = 20;
        static const int SWHL0_GEOM_ID = 21;
        static const int SWHL1_GEOM_ID = 22;
        static const int SWHL2_GEOM_ID = 23;
        static const int SWHL3_GEOM_ID = 24;
        static const int SWHL4_GEOM_ID = 25;
        static const int SWHL5_GEOM_ID = 26;
        static const int TSLIGHT0_GEOM_ID = 27;
        static const int TSLIGHT1_GEOM_ID = 28;
    public:
        static int AmbientHeadlightStyle;
    private:
        aiVehicleSpline* Spline;
        short SignalFrequency;
        byte SignalState;
        short LOD;
        short Variant;
        vehBreakableMgr* BreakableMgr;
        Vector3 HeadlightPosition;
        Vector3 VehiclePosition;
    public:
        aiVehicleInstance(void)             DONOTCALL;

        //properties
        aiVehicleSpline* GetSpline();
        int GetVariant() const;

        //overrides
        AGE_API Vector3 const& GetPosition() override;
        AGE_API Matrix34 const& GetMatrix(Matrix34& mtx) override;
        AGE_API void SetMatrix(Matrix34 const& mtx) override;
        AGE_API dgPhysEntity* GetEntity() override;
        AGE_API dgPhysEntity* AttachEntity() override;
        AGE_API void Detach() override;
        AGE_API void Draw(int lod) override;
        AGE_API void DrawShadow() override;
        AGE_API void DrawShadowMap() override;
        AGE_API void DrawGlow() override;
        AGE_API void DrawReflected(float intensity) override;
        AGE_API unsigned int SizeOf() override;
        AGE_API phBound* GetBound(int type) override;

        //members
        AGE_API aiVehicleData* GetData();
        AGE_API void DrawPart(modStatic* model, const Matrix34& matrix, modShader* shaders, int unused);
        AGE_API void DrawPart(int lod, int geomId, const Matrix34& matrix, modShader* shaders);
        void DrawPartReflected(int lod, int geomId, const Matrix34& matrix, modShader* shaders);
        void DrawPart(int lod, int geomId, const Matrix34& matrix, modShader* shaders, bool reflected);

        //lua
        static void BindLua(LuaState L);
    };
    ASSERT_SIZEOF(aiVehicleInstance, 0x3C);
}