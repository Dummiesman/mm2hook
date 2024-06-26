#pragma once
#include "aiVehicleSpline.h"
#include <modules/rgl/vgl.h>

using namespace MM2;


MM2::aiVehicleSpline::aiVehicleSpline()
{
    hook::Thunk<0x0567DF0>::Call<void>(this);
}

aiVehicleSpline::~aiVehicleSpline()
{
    hook::Thunk<0x567EB0>::Call<void>(this);
}

void aiVehicleSpline::UpdateObstacleMap()               { hook::Thunk<0x568410>::Call<void>(this); }

void aiVehicleSpline::Position(Vector3 &a1)             { hook::Thunk<0x551C40>::Call<void>(this, &a1); }
float aiVehicleSpline::Speed()                          { return hook::Thunk<0x51C70>::Call<float>(this); }
int aiVehicleSpline::CurrentRoadIdx(aiPath **a1, const bool *a2, int *a3)
                                                        { return hook::Thunk<0x5681C0>::Call<int>(this, a1, a2, a3); }
int aiVehicleSpline::CurrentRdVert()                    { return hook::Thunk<0x5683F0>::Call<int>(this); }

void aiVehicleSpline::Update()                          FORWARD_THUNK;
void aiVehicleSpline::Reset()                           FORWARD_THUNK;
int aiVehicleSpline::Type()                             { return hook::Thunk<0x56A700>::Call<int>(this); }
Matrix34 & aiVehicleSpline::GetMatrix() const           { return hook::Thunk<0x551C80>::Call<Matrix34&>(this); }
float aiVehicleSpline::FrontBumperDistance()            { return hook::Thunk<0x551C00>::Call<float>(this); }
float aiVehicleSpline::BackBumperDistance()             { return hook::Thunk<0x551C10>::Call<float>(this); }
float aiVehicleSpline::LSideDistance()                  { return hook::Thunk<0x551C20>::Call<float>(this); }
float aiVehicleSpline::RSideDistance()                  { return hook::Thunk<0x551C30>::Call<float>(this); }
int aiVehicleSpline::CurrentLane()                      { return hook::Thunk<0x551BE0>::Call<int>(this); }
int aiVehicleSpline::CurrentRoadId()                    { return hook::Thunk<0x51BF0>::Call<int>(this); }

void aiVehicleSpline::DrawId() const
{
    Vector3 position = GetMatrix().GetRow(3);
    position.Y += 4.4f;
    vglDrawLabelf(position, "%d", this->GetId());
}

void aiVehicleSpline::Impact(int a1)                     FORWARD_THUNK;
AudImpact* aiVehicleSpline::GetAudImpactPtr()            { return hook::Thunk<0x551C90>::Call<AudImpact*>(this); }
void aiVehicleSpline::PlayHorn(float a1, float a2)       { hook::Thunk<0x551CA0>::Call<void>(this, a1, a2); }
void aiVehicleSpline::StopVoice()                        { hook::Thunk<0x551CB0>::Call<void>(this); }

bool aiVehicleSpline::SolvedStopOrientation() const
{
    return _solvedStopOrientation.get(this) == TRUE;
}

//fields
float aiVehicleSpline::GetSpeed() const
{
    return _curSpeed.get(this);
}

void aiVehicleSpline::SetSpeed(float speed)
{
    _curSpeed.set(this, speed);
}

aiRailSet* aiVehicleSpline::GetRailSet()
{
    return _railSet.ptr(this);
}

aiVehicleInstance * aiVehicleSpline::GetInst() const
{
    return _vehicleInstance.get(this);
}

void aiVehicleSpline::SetMatrix(Matrix34 const& mtx)
{
    _matrix.get(this)->Set(mtx);
}

int MM2::aiVehicleSpline::GetAvoidingPlayerIdx() const
{
    return _avoidingPlayerIdx.get(this);
}

void aiVehicleSpline::IncReactTicks()
{
    _curReactTicks.set(this, _curReactTicks.get(this) + 1);
}

void MM2::aiVehicleSpline::SwitchState(int newState)
{
    _lastState.set(this, _state.get(this));
    _state.set(this, newState);
}

void aiVehicleSpline::SolveStopOrientation()
{
    this->SolvePositionAndOrientation();
    _solvedStopOrientation.set(this, TRUE);
}

void aiVehicleSpline::SolveYPositionAndOrientation()
{
    hook::Thunk<0x5690C0>::Call<void>(this);
}

void MM2::aiVehicleSpline::SolvePositionAndOrientation()
{
    hook::Thunk<0x5686E0>::Call<void>(this);
}

BOOL MM2::aiVehicleSpline::IsThePlayerInFrontOfMe(int playerId) const
{
    return hook::Thunk<0x569CB0>::Call<BOOL>(this, playerId);
}

BOOL aiVehicleSpline::DetectPlayerCollision(int playerId) const
{
    return hook::Thunk<0x569930>::Call<BOOL>(this, playerId);
}

BOOL aiVehicleSpline::IsAmbientBlockingPlayer(int playerId) const
{
    return hook::Thunk<0x569DD0>::Call<BOOL>(this, playerId);
}

//lua
void aiVehicleSpline::BindLua(LuaState L) {
    LuaBinding(L).beginExtendClass<aiVehicleSpline, aiVehicle>("aiVehicleSpline")
        .addPropertyReadOnly("Instance", &GetInst)
        .endClass();
}
    