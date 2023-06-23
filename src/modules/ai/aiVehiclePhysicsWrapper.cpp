#include "aiVehiclePhysicsWrapper.h"

using namespace MM2;

vehCar* aiVehiclePhysicsWrapper::GetCar() const
{
    return vehiclePhysics.GetCar();
}

short aiVehiclePhysicsWrapper::GetState() const
{
    return vehiclePhysics.GetState();
}

void aiVehiclePhysicsWrapper::SetState(aiVehiclePhysicsState state)
{
    vehiclePhysics.SetState(state);
}

int aiVehiclePhysicsWrapper::GetCurrentLap() const
{
    return vehiclePhysics.GetCurrentLap();
}

int aiVehiclePhysicsWrapper::GetLapCount() const
{
    return vehiclePhysics.GetLapCount();
}

void aiVehiclePhysicsWrapper::Init(int id, const char* basename, bool canRepair, int audioType)
{
    vehiclePhysics.Init(id, basename, canRepair ? 1 : 0, audioType);
}

void aiVehiclePhysicsWrapper::Reset()
{
    vehiclePhysics.Reset();
}

void aiVehiclePhysicsWrapper::RegisterRoute(LuaRef optsTable)
{
    optsTable.checkTable();

    int numIntersections;

    LuaRef intersectionsTable = optsTable["IntersectionIDs"];
    if (intersectionsTable.isTable())
    {
        numIntersections = min(128, intersectionsTable.len());
        for (int i = 0; i < numIntersections; i++)
        {
            this->intersectionsBuf[i] = (short)(intersectionsTable[i+1].value<int>());
        }
    }
    else
    {
        numIntersections = 0;
        //this->intersectionsBuf[0] = (short)0;
    }

    int numLaps = optsTable.get<int>("NumLaps", 0);
    Vector3 endPosition = optsTable.get<Vector3>("EndPosition", Vector3(0, 0, 0));
    float targetSpeed = optsTable.get<float>("TargetSpeed", 9999.0f);
    float finishRadius = optsTable.get<float>("FinishRadius", 0.0f);
    bool avoidTraffic = optsTable.get<bool>("AvoidTraffic", false);
    bool avoidProps = optsTable.get<bool>("AvoidProps", false);
    bool avoidPlayers = optsTable.get<bool>("AvoidPlayers", false);
    bool avoidOpponents = optsTable.get<bool>("AvoidOpponents", false);
    float maxThrottle = optsTable.get<float>("MaxThrottle", 1.0f);
    float cornerSpeedMultiplier = optsTable.get<float>("CornerSpeedMultiplier", 2.0f);
    float cornerBrakingThreshold = optsTable.get<float>("CornerBrakingThreshold", 0.7f);

    // unknown stuff still
    bool badPathfinding = optsTable.get<bool>("BadPathfinding", false);
    float someDistancePadding = optsTable.get<float>("SomeDistancePadding", 1.0f);

    vehiclePhysics.RegisterRoute(intersectionsBuf, numIntersections, endPosition, Vector3::YAXIS, numLaps, targetSpeed, finishRadius,
        false, avoidTraffic, avoidProps, avoidPlayers, avoidOpponents, badPathfinding,
        maxThrottle, cornerSpeedMultiplier, cornerBrakingThreshold, someDistancePadding);
    
}

void aiVehiclePhysicsWrapper::DriveRoute()
{
    vehiclePhysics.DriveRoute();
}

void aiVehiclePhysicsWrapper::Mirror(vehCar* car)
{
    vehiclePhysics.Mirror(car);
}

void aiVehiclePhysicsWrapper::DrawRouteThroughTraffic()
{
    vehiclePhysics.DrawRouteThroughTraffic();
}

void aiVehiclePhysicsWrapper::Stop()
{
    vehiclePhysics.SetState(aiVehiclePhysicsState::Stop);
}

void aiVehiclePhysicsWrapper::BindLua(LuaState L)
{
    LuaBinding(L).beginExtendClass<aiVehiclePhysicsWrapper, Base>("aiVehiclePhysicsWrapper")
        .addFactory([]() {
            auto object = new aiVehiclePhysicsWrapper();
            MM2Lua::MarkForCleanupOnShutdown(object);
            return object;
        })
        .addPropertyReadOnly("Car", &GetCar)
        .addPropertyReadOnly("CurrentLap", &GetCurrentLap)
        .addPropertyReadOnly("NumLaps", &GetLapCount)
        .addPropertyReadOnly("State", &GetState)
        .addFunction("Init", &Init)
        .addFunction("Reset", &Reset)
        .addFunction("RegisterRoute", &RegisterRoute)
        .addFunction("DriveRoute", &DriveRoute)
        .addFunction("Mirror", &Mirror)
        .addFunction("DrawRouteThroughTraffic", &DrawRouteThroughTraffic)
         .addFunction("Stop", &Stop)
        .endClass();
}
