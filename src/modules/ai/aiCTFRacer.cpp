#pragma once
#include "aiCTFRacer.h"
#include <modules\ai\aiMap.h>
#include <modules\vehicle\car.h>
#include <modules\rgl.h>

using namespace MM2;

Vector3 aiCTFRacer::GetFlagPosition() const { return FlagPosition; }
void aiCTFRacer::SetFlagPosition(Vector3 pos) 
{ 
    FlagPosition = pos; 
    FlagRoom = lvlLevel::GetSingleton()->FindRoomId(FlagPosition, FlagRoom);
}

Vector3 aiCTFRacer::GetDeliverPosition() const { return DeliverPosition; }
void aiCTFRacer::SetDeliverPosition(Vector3 pos) 
{
    DeliverPosition = pos; 
    DeliverRoom = lvlLevel::GetSingleton()->FindRoomId(DeliverPosition, DeliverRoom);
}

void aiCTFRacer::EnterAcquireMode() { CTFState = 1; }
void aiCTFRacer::EnterDeliverMode() { CTFState = 3; }

const aiVehiclePhysics* aiCTFRacer::GetVehiclePhysics() const
{
    return &this->VehiclePhysics;
}

vehCar* aiCTFRacer::GetCar() const
{
    return GetVehiclePhysics()->GetCar();
}

int aiCTFRacer::GetId() const
{
    return this->ID;
}

short aiCTFRacer::GetState() const
{
    return GetVehiclePhysics()->GetState();
}

inline bool aiCTFRacer::CanRepairDamage() const 
{
    return VehiclePhysics.CanRepairDamage(); 
}

inline void aiCTFRacer::SetCanRepairDamage(bool value) 
{
    VehiclePhysics.SetCanRepairDamage(value); 
}

AGE_API void aiCTFRacer::Init(int id, const char* basename) 
{
    hook::Thunk<0x554470>::Call<void>(this, id, basename); 
}

/*
* NOTE: Removed in Midnight Club and otherwise unused in MM2
*/
void aiCTFRacer::PreAquireFlag()
{
    hook::Thunk<0x554640>::Call<void>(this);
}

void aiCTFRacer::AquireFlag()
{
    LastCTFState = CTFState;
    
    auto vehPhysState = VehiclePhysics.GetState();
    if (vehPhysState == aiVehiclePhysicsState::Forward || vehPhysState == aiVehiclePhysicsState::Shortcut)
    {
        float elapsedTime = (Timer::Ticks() - this->TimerTicks) * Timer::TicksToSeconds.get();
        float flagDistance = LastFlagPosition.FlatDist(FlagPosition);

        if (flagDistance > 0.1f || (elapsedTime > 0.5f && VehiclePhysics.GetCurrentComponentType() == aiMapComponentType::Road))
        {
            auto sourceMatrix = this->GetCar()->GetICS()->GetMatrix();
            int sourceRoom = this->GetCar()->GetInst()->GetRoomId();
            Vector3 routeOrientation = Vector3::YAXIS;

            Vector3 destPosition = GetFlagPosition();
            int destRoom = FlagRoom;

            auto aiMap = aiMap::GetInstance();
            aiMap->CalcRoute(sourceMatrix, destPosition, routeOrientation, this->RouteIntersections, &this->NumRouteIntersections, sourceRoom, FlagRoom, true);
            VehiclePhysics.RegisterRoute(RouteIntersections, NumRouteIntersections, destPosition, routeOrientation, 0, 9999.0f, 0.0f, false, true, true, false, true, false, 1.1f, 1.5f);
            
            LastFlagPosition = FlagPosition;
            TimerTicks = Timer::Ticks();
        }
    }

    VehiclePhysics.DriveRoute();
}

/*
* NOTE: Removed in Midnight Club and otherwise unused in MM2
*/
void aiCTFRacer::PreDeliverFlag()
{
    hook::Thunk<0x554990>::Call<void>(this);
}

void aiCTFRacer::DeliverFlag()
{
    bool changedState = (CTFState != LastCTFState);
    LastCTFState = CTFState;

    float elapsedTime = (Timer::Ticks() - this->TimerTicks) * Timer::TicksToSeconds.get();
    if (changedState || (elapsedTime > 0.5f && VehiclePhysics.GetCurrentComponentType() == aiMapComponentType::Road))
    {
        auto sourceMatrix = this->GetCar()->GetICS()->GetMatrix();
        int sourceRoom = this->GetCar()->GetInst()->GetRoomId();
        Vector3 routeOrientation = Vector3::YAXIS;

        Vector3 destPosition = GetDeliverPosition();
        int destRoom = DeliverRoom;

        auto aiMap = aiMap::GetInstance();
        aiMap->CalcRoute(sourceMatrix, destPosition, routeOrientation, this->RouteIntersections, &this->NumRouteIntersections, sourceRoom, FlagRoom, true);
        VehiclePhysics.RegisterRoute(RouteIntersections, NumRouteIntersections, destPosition, routeOrientation, 0, 9999.0f, 0.0f, false, true, true, false, true, false, 1.0f, 1.5f);

        TimerTicks = Timer::Ticks();
    }
    
    VehiclePhysics.DriveRoute();
}
        
AGE_API void aiCTFRacer::Reset() 
{
    hook::Thunk<0x554510>::Call<void>(this); 
    TimerTicks = Timer::Ticks();
}

AGE_API void aiCTFRacer::Update()
{
    switch (CTFState)
    {
    case 0:
        PreAquireFlag();
        break;
    case 1:
        AquireFlag();
        break;
    case 2:
        PreDeliverFlag();
        break;
    case 3:
        DeliverFlag();
        break;
    }

    // Declare mover, updated with opponent optimization as done in MC1
    auto aiMap = aiMap::GetInstance();
    int closestPlayer = -1;
    float closestPlayerDist2 = 9999999.0f;

    for (int i = 0; i < aiMap->GetPlayerCount(); i++)
    {
        auto player = aiMap->Player(i);
        auto playerMtx = player->GetCar()->GetICS()->GetMatrix();
        auto ourMtx = this->GetCar()->GetICS()->GetMatrix();

        float dist2 = (ourMtx.GetRow(3) - playerMtx.GetRow(3)).Mag2();
        if (dist2 < closestPlayerDist2)
        {
            closestPlayer = i;
            closestPlayerDist2 = dist2;
        }
    }
    
    if (closestPlayerDist2 < 40000.0f)
    {
        dgPhysManager::Instance.get()->DeclareMover(this->GetCar()->GetInst(), 3, 0x1B);
    }
    else
    {
        if(dgPhysManager::sm_OpponentOptimization.get())
            dgPhysManager::Instance.get()->DeclareMover(this->GetCar()->GetInst(), 2, 0x13);
        else
            dgPhysManager::Instance.get()->DeclareMover(this->GetCar()->GetInst(), 2, 0x1B);
    }

    // check if we've fallen through the world
    if (this->GetCar()->GetICS()->GetPosition().Y < -200.0f)
    {
        Warningf("CTF Opponent #%d, Has fallen through the geometry.", this->ID);
        this->Reset();
    }
}

void MM2::aiCTFRacer::DrawRouteThroughTraffic()
{
    auto vehiclePhysics = GetVehiclePhysics();

    rglPushMatrix();
    rglWorldIdentity();

    vehiclePhysics->DrawId();
    vehiclePhysics->DrawRouteThroughTraffic();

    // start drawing intersections
    vglBindTexture(nullptr);
    

    if (vehiclePhysics->GetNumWaypoints() > 0)
    {
        auto aiMap = aiMap::GetInstance();
        Vector3 drawStartPos = vehiclePhysics->GetCar()->GetICS()->GetPosition();

        vglCurrentColor = mkfrgba(1.0f, 0.0f, 0.0f, 1.0f);
        vglBegin(MM2::gfxDrawMode::DRAWMODE_LINESTRIP, vehiclePhysics->GetNumWaypoints());

        for (int i = 0; i < vehiclePhysics->GetNumWaypoints(); i++)
        {
            vglCurrentColor = mkfrgba(1.0f - (i / (float)vehiclePhysics->GetNumWaypoints()), 0.0f, i / (float)vehiclePhysics->GetNumWaypoints(), 1.0f);

            auto intersection = aiMap->Intersection(vehiclePhysics->GetIntersection(i));
            if (intersection != nullptr)
            {
                Vector3 position = intersection->GetCenter();
                position.Y += 1.0f;
                vglVertex3f(position);
            }
        }

        vglEnd();

        if (vehiclePhysics->GetCurrentWaypoint() < vehiclePhysics->GetNumWaypoints())
        {
            vglCurrentColor = mkfrgba(1.0f, 1.0f, 1.0f, 1.0f);
            vglBegin(MM2::gfxDrawMode::DRAWMODE_LINESTRIP, 2);

            vglVertex3f(drawStartPos);
            auto intersection = aiMap->Intersection(vehiclePhysics->GetIntersection(vehiclePhysics->GetCurrentWaypoint()));

            if (intersection == nullptr)
            {
                vglVertex3f(drawStartPos);
            }
            else {
                Vector3 position = intersection->GetCenter();
                position.Y += 1.0f;
                vglVertex3f(position);
            }

            vglEnd();
        }

        for (int i = 0; i < vehiclePhysics->GetNumWaypoints(); i++)
        {
            auto intersection = aiMap->Intersection(vehiclePhysics->GetIntersection(i));
            if (intersection != nullptr)
            {
                intersection->DrawId();
            }
        }
    }
    

    vglDrawLabelf(GetCar()->GetICS()->GetPosition() + Vector3(0.0f, 5.0f, 0.0f), "CurCompType %i", VehiclePhysics.GetCurrentComponentType());

    rglPopMatrix();
}

void aiCTFRacer::BindLua(LuaState L) {
    LuaBinding(L).beginExtendClass<aiCTFRacer, Base>("aiCTFRacer")
        .addPropertyReadOnly("Car", &GetCar)
        .addPropertyReadOnly("State", &GetState)
        .addPropertyReadOnly("ID", &GetId)
        .addProperty("FlagPosition", &GetFlagPosition, &SetFlagPosition)
        .addProperty("DeliverPosition", &GetDeliverPosition, &SetDeliverPosition)
        .addFunction("Acquire", &EnterAcquireMode)
        .addFunction("Deliver", &EnterDeliverMode)
        .addFunction("Reset", &Reset)
        .addFunction("DrawRouteThroughTraffic", &DrawRouteThroughTraffic)
        .endClass();
}