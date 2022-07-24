#pragma once
#include <mm2_common.h>
#include <modules\level\inst.h>

namespace MM2
{
    // Forward declarations
    class aiObstacle;
    class aiVehicle;
    class aiRailSet;
    class aiVehicleSpline;
    class aiVehicleInstance;

    // External declarations
    extern class aiPath;
    extern class aiMap;
    extern class aiVehicleInstance;
    extern class dgPhysEntity;
    extern class phBound;
    extern class aiVehicleData;

    // Class definitions
    class aiRailSet {
    protected:
        static hook::Field<0x98, float> _speed;
        static hook::Field<0x44, float> _accelFactor;
    public:
        float GetSpeed();
        float GetAccelFactor();
    };

    class aiVehicleInstance : public lvlInstance {
    public:
        static const int HLIGHT_GEOM_ID = 2;
        static const int TLIGHT_GEOM_ID = 3;
        static const int SLIGHT0_GEOM_ID = 4;
        static const int SLIGHT1_GEOM_ID = 5;
        static const int BLIGHT_GEOM_ID = 18;
        static const int PLIGHTON_GEOM_ID = 19;
        static const int PLIGHTOFF_GEOM_ID = 20;
        static const int TSLIGHT0_GEOM_ID = 21;
        static const int TSLIGHT1_GEOM_ID = 22;
    public:
        static int AmbientHeadlightStyle;
    protected:
        static hook::Field<0x14, aiVehicleSpline*> _spline;
    public:
        aiVehicleInstance(void)             DONOTCALL;

        //properties
        aiVehicleSpline* GetSpline();
        
        //overrides
        AGE_API Vector3 const& GetPosition() override;
        AGE_API Matrix34 const& GetMatrix(Matrix34* a1) override;
        AGE_API void SetMatrix(Matrix34 const& a1) override;
        AGE_API dgPhysEntity* GetEntity() override;
        AGE_API dgPhysEntity* AttachEntity() override;
        AGE_API void Detach() override;
        AGE_API void DrawShadow() override;
        AGE_API void DrawShadowMap() override;
        
        AGE_API void Draw(int a1) override;

        AGE_API void DrawReflected(float a1) override;
        AGE_API unsigned int SizeOf() override;
        AGE_API phBound* GetBound(int a1) override;
        
        //members
        aiVehicleData* GetData();
        AGE_API void DrawPart(modStatic* a1, const Matrix34* a2, modShader* a3, int a4);

        //lua
        static void BindLua(LuaState L);
    };

    class aiObstacle {
    public:
        aiObstacle(void)                                    DONOTCALL;
        aiObstacle(const aiObstacle&&)                      DONOTCALL;

        virtual BOOL InAccident(void);
        virtual void Position(Vector3& a1)                  PURE;
        virtual float Speed(void)                           PURE;
        virtual float BreakThreshold(void);
        virtual BOOL Drivable(void);
        virtual int CurrentRoadIdx(aiPath** a1, const bool* a2, int* a3)    
                                                            PURE;
        virtual int CurrentRdVert(void)                     
                                                            PURE;
        virtual void PreAvoid(const Vector3& a1, const Vector3& a2, float a3, Vector3& a4, Vector3& a5)
                                                            PURE;
        virtual float IsBlockingTarget(const Vector3& a1, const Vector3& a2, float a3, float a4)  
                                                            PURE;

        //lua
        static void BindLua(LuaState L);
    };

    class aiVehicle : public aiObstacle {
    public:
        aiVehicle(void)                                     DONOTCALL;
        aiVehicle(const aiVehicle &&)                       DONOTCALL;

        void PreAvoid(const Vector3& a1, const Vector3& a2, float a3, Vector3& a4, Vector3& a5) override;
        float IsBlockingTarget(const Vector3& a1, const Vector3& a2, float a3, float a4) override;
        
        virtual void Update(void);
        virtual void Reset(void);
        virtual int Type(void)                              PURE;
        virtual Matrix34 & GetMatrix(void)                  PURE;
        virtual float FrontBumperDistance(void)             PURE;
        virtual float BackBumperDistance(void)              PURE;
        virtual float LSideDistance(void)                   PURE;
        virtual float RSideDistance(void)                   PURE;
        virtual int CurrentLane(void)                       PURE;
        virtual int CurrentRoadId(void)                     PURE;
        virtual void DrawId(void)                           PURE;
        virtual void ReplayDebug(void);

        //lua
        static void BindLua(LuaState L);
    };

    // Lua initialization

}