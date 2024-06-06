#pragma once
#include "aiVehicle.h"
#include "aiPath.h"
#include "aiMap.h"


namespace MM2
{
    // Forward declarations
    class aiVehiclePhysics;
    struct aiRouteNode;

    // External declarations
    extern class vehCar;
    extern class aiVehicle;

    // Class definitions
    enum aiVehiclePhysicsState : short
    {
        Forward = 0x0,
        Backup = 0x1,
        Shortcut = 0x2,
        Stop = 0x3,
    };


    struct  aiRouteNode
    {
        int unk_0x00;
        Vector3 Position;
        float Angle;
        float Distance;
        short unk_18;
        short RoadVertexIndex;
        short unk_1C;
        short unk_1E;
        short unk_20;
        short unk_22;
    };

    class aiVehiclePhysics : public aiVehicle {
    private:
        byte _buffer[0x9760];
    private:
        void initLua(int id, const char* basename, bool circuitMode, int audioType);
    protected:
        static hook::Field<0x10, vehCar> _vehCar;
        static hook::Field<0x27C, aiVehiclePhysicsState> _state;
        static hook::Field<0x2D4, aiRouteNode> _routeNodes;
        static hook::Field<0x9514, int> _activeRoute;
        static hook::Field<0x95E0, int> _routeNodeCounts;
        static hook::Field<0x9644, int> _routeCount;
        static hook::Field<0x9674, short*> _intersectionIds;
        static hook::Field<0x9678, short> _numWayPts;
        static hook::Field<0x967A, short> _curWayPt;
        static hook::Field<0x9680, short> _curAiComponentIndex;
        static hook::Field<0x967E, short> _curAiComponentType;

        static hook::Field<0x9682, unsigned short> _currentLap;
        static hook::Field<0x9684, unsigned short> _lapCount;

        static hook::Field<0x9686, short> _damagedOut;

        static hook::Field<0x9690, float> _brakeInput;
        static hook::Field<0x9694, float> _throttleInput;
        static hook::Field<0x9698, float> _steeringInput;
    public:
        aiVehiclePhysics(void);
        ~aiVehiclePhysics();

        vehCar* GetCar() const;
        short GetState() const;
        void SetState(aiVehiclePhysicsState state);
        int GetCurrentWaypoint() const;
        int GetNumWaypoints() const;
        int GetIntersection(int index) const;
        int GetCurrentLap() const;
        int GetLapCount() const;
        aiMapComponentType GetCurrentComponentType() const;
        int GetCurrentComponentIndex() const;
        bool IsDamagedOut() const;
        float GetBrakeInput() const;
        float GetThrottleInput() const;
        float GetSteeringInput() const;

        AGE_API void Init(int id, const char* basename, short circuitMode, int audioType);
        AGE_API void RegisterRoute(short* intersectionIDs, short numIntersections, Vector3 const& endPosition, Vector3 const& endOrientation,
            short numLaps = 0, float targetSpeed = 9999.0f, float finishRadius = 0.0f, bool unkFlag = false, bool avoidTraffic = true, bool avoidProps = true, bool avoidPlayers = true,
            bool avoidOpponents = true, bool weirdPathfinding = false, float maxThrottle = 1.0f, float cornerSpeedMultiplier = 2.0f, float cornerBrakingThreshold = 0.7f, float someDistancePadding = 75.0f);
        AGE_API void DriveRoute();
        AGE_API void Mirror(vehCar* car);

        void Position(Vector3& a1) override;
        float Speed(void) override;
        int CurrentRoadIdx(aiPath** a1, const bool* a2, int* a3);
        int CurrentRdVert(void) override;

        void Reset(void) override;
        int Type(void) override;
        Matrix34 & GetMatrix(void) const override;
        float FrontBumperDistance(void) override;
        float BackBumperDistance(void) override;
        float LSideDistance(void) override;
        float RSideDistance(void) override;
        int CurrentLane(void) override;
        int CurrentRoadId(void) override;
        void DrawId(void) const override;
        void ReplayDebug(void) override;

        void DrawRouteThroughTraffic() const;

        static void BindLua(LuaState L);
    };

    ASSERT_SIZEOF(aiVehiclePhysics, 0x9770);

    // Lua initialization

}