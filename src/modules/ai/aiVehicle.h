#pragma once
#include <mm2_common.h>
#include <modules\level\inst.h>
#include "aiRailSet.h"

namespace MM2
{
    // Forward declarations
    class aiObstacle;
    class aiObstacleLuaIterator;
    class aiVehicle;
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
    class aiObstacle {
    private:
        aiObstacle* Next;
    private:
        bool luaDrivable() { return Drivable() == TRUE; }
        bool luaInAccident() { return InAccident() == TRUE; }
        Vector3 luaPosition() { Vector3 p = Vector3::ORIGIN; this->Position(p); return p; }
    public:
        aiObstacle(void)                                    {};
        aiObstacle(const aiObstacle&&)                      {};

        aiObstacle* GetNext()                               { return this->Next; }

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

    class aiObstacleLuaIterator : public CppFunctor
    {
    private:
        aiObstacle* obstacle;
    public:
        aiObstacleLuaIterator(aiObstacle* obstacle)
        {
            this->obstacle = obstacle;
        }

        virtual ~aiObstacleLuaIterator()
        {
        }

        virtual int run(lua_State* L) override
        {
            if (this->obstacle)
            {
                LuaState(L).push(this->obstacle);
                this->obstacle = this->obstacle->GetNext();
                return 1;
            }
            else
            {
                return 0;
            }
        }
    };

    class aiVehicle : public aiObstacle {
    private:
        __int16 Id;
        __int16 FID;
        aiVehicle* NextAtStopSign;
    public:
        aiVehicle(void)                                     {};
        aiVehicle(const aiVehicle &&)                       {};

        int GetId() const;

        void PreAvoid(const Vector3& a1, const Vector3& a2, float a3, Vector3& a4, Vector3& a5) override;
        float IsBlockingTarget(const Vector3& a1, const Vector3& a2, float a3, float a4) override;
        
        virtual void Update(void);
        virtual void Reset(void);
        virtual int Type(void)                              PURE;
        virtual Matrix34 & GetMatrix(void) const            PURE;
        virtual float FrontBumperDistance(void)             PURE;
        virtual float BackBumperDistance(void)              PURE;
        virtual float LSideDistance(void)                   PURE;
        virtual float RSideDistance(void)                   PURE;
        virtual int CurrentLane(void)                       PURE;
        virtual int CurrentRoadId(void)                     PURE;
        virtual void DrawId(void) const                     PURE;
        virtual void ReplayDebug(void);

        //lua
        static void BindLua(LuaState L);
    };

    // Lua initialization

}