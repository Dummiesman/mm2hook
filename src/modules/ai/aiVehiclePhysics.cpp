#pragma once
#include "aiVehiclePhysics.h"
#include "..\rgl.h"
#include <modules\vehicle\car.h>

using namespace MM2;

void aiVehiclePhysics::initLua(int id, const char* basename, bool circuitMode, int audioType)
{
	this->Init(id, basename, (circuitMode) ? 1 : 0, audioType);
}

aiVehiclePhysics::aiVehiclePhysics(void) 
{
	scoped_vtable x(this);
	hook::Thunk<0x5592A0>::Call<void>(this);
}

MM2::aiVehiclePhysics::~aiVehiclePhysics()
{
	scoped_vtable x(this);
	hook::Thunk<0x559380>::Call<void>(this);
}

vehCar * aiVehiclePhysics::GetCar() const
{
	return _vehCar.ptr(this);
}

short aiVehiclePhysics::GetState() const
{
	return _state.get(this);
}

void aiVehiclePhysics::SetState(aiVehiclePhysicsState state)
{
	_state.set(this, state);
}

int MM2::aiVehiclePhysics::GetCurrentWaypoint() const
{
	return _curWayPt.get(this);
}

int MM2::aiVehiclePhysics::GetNumWaypoints() const
{
	return _numWayPts.get(this);
}

int MM2::aiVehiclePhysics::GetIntersection(int index) const
{
	auto intersections = _intersectionIds.get(this);
	if (intersections == nullptr || index >= this->GetNumWaypoints() || index < 0)
		return -1;
	return intersections[index];
}

int aiVehiclePhysics::GetCurrentLap() const 
{
	return _currentLap.get(this);
}

int aiVehiclePhysics::GetLapCount() const
{
	return _lapCount.get(this);
}

aiMapComponentType MM2::aiVehiclePhysics::GetCurrentComponentType() const
{
	return static_cast<aiMapComponentType>(_curAiComponentType.get(this));
}

int MM2::aiVehiclePhysics::GetCurrentComponentIndex() const
{
	return _curAiComponentIndex.get(this);
}

bool MM2::aiVehiclePhysics::IsDamagedOut() const
{
	return _damagedOut.get(this) != 0;
}

float MM2::aiVehiclePhysics::GetBrakeInput() const
{
	return _brakeInput.get(this);
}

float MM2::aiVehiclePhysics::GetThrottleInput() const
{
	return _throttleInput.get(this);
}

float MM2::aiVehiclePhysics::GetSteeringInput() const
{
	return _steeringInput.get(this);
}

AGE_API void aiVehiclePhysics::Init(int id, const char* basename, short canRepair, int audioType)
															 { hook::Thunk<0x5593E0>::Call<void>(this, id, basename, canRepair, audioType); }

AGE_API void MM2::aiVehiclePhysics::RegisterRoute(short* intersectionIDs, short numIntersections, Vector3 const& endPosition, Vector3 const& endOrientation, short numLaps, float targetSpeed, float finishRadius, bool unkFlag, bool avoidTraffic, bool avoidProps, bool avoidPlayers, bool avoidOpponents, bool weirdPathfinding, float maxThrottle, float cornerSpeedMultiplier, float cornerBrakingThreshold, float someDistancePadding)
{
	hook::Thunk<0x5598A0>::Call<void>(this, intersectionIDs, numIntersections, &endPosition, &endOrientation,
		numLaps, targetSpeed, finishRadius, unkFlag, avoidTraffic, avoidProps, avoidPlayers,
		avoidOpponents, weirdPathfinding, maxThrottle, cornerSpeedMultiplier, cornerBrakingThreshold, someDistancePadding);
}

AGE_API void MM2::aiVehiclePhysics::DriveRoute()
{
	hook::Thunk<0x55A8F0>::Call<void>(this, 0);
}

AGE_API void MM2::aiVehiclePhysics::Mirror(vehCar* car)
{
	hook::Thunk<0x55A570>::Call<void>(this, car);
}

void aiVehiclePhysics::Position(Vector3 &a1)                 { hook::Thunk<0x567C10>::Call<void>(this, &a1); }
float aiVehiclePhysics::Speed(void)                          { return hook::Thunk<0x567C00>::Call<float>(this); }
int aiVehiclePhysics::CurrentRoadIdx(aiPath **a1, const bool *a2, int *a3)
															 { return hook::Thunk<0x55AAD0>::Call<int>(this, a1, a2, a3); }
int aiVehiclePhysics::CurrentRdVert(void)                    { return hook::Thunk<0x567BB0>::Call<int>(this); }

void aiVehiclePhysics::Reset(void)                           { hook::Thunk<0x5597A0>::Call<void>(this); }
int aiVehiclePhysics::Type(void)                             { return hook::Thunk<0x567B90>::Call<int>(this); }
Matrix34 & aiVehiclePhysics::GetMatrix(void) const           { return hook::Thunk<0x567C40>::Call<Matrix34&>(this); };
float aiVehiclePhysics::FrontBumperDistance(void)            { return hook::Thunk<0x567BC0>::Call<float>(this); }
float aiVehiclePhysics::BackBumperDistance(void)             { return hook::Thunk<0x567BD0>::Call<float>(this); }
float aiVehiclePhysics::LSideDistance(void)                  { return hook::Thunk<0x567BE0>::Call<float>(this); }
float aiVehiclePhysics::RSideDistance(void)                  { return hook::Thunk<0x567BF0>::Call<float>(this); }
int aiVehiclePhysics::CurrentLane(void)                      { return hook::Thunk<0x567BA0>::Call<int>(this); }
int aiVehiclePhysics::CurrentRoadId(void)                    { return hook::Thunk<0x567C50>::Call<int>(this); }
void aiVehiclePhysics::ReplayDebug(void)                     { hook::Thunk<0x5679D0>::Call<void>(this); }

void aiVehiclePhysics::DrawId(void)  const
{
	Vector3 position = this->GetCar()->GetICS()->GetPosition();
	position.Y += 4.4f;
	vglDrawLabelf(position, "%d", this->GetId());
}

void aiVehiclePhysics::DrawRouteThroughTraffic() const
{
   int routeCount = _routeCount.get(this);
   int currentRoute = _activeRoute.get(this);
   
   rglPushMatrix();
   rglWorldIdentity();
   vglBindTexture(nullptr);
   
   for (int i = 0; i < routeCount; i++)
   {
	   int  routeNodeCount = _routeNodeCounts.ptr(this)[i];
	   
	   vglBegin(gfxDrawMode::DRAWMODE_LINESTRIP, routeNodeCount);
	   vglCurrentColor = mkfrgba(1.0f, (i == currentRoute) ? 1.0f : 0.0f, (i == currentRoute) ? 0.0f : 1.0f, 1.0f);
   
	   for (int j = 0; j < routeNodeCount; j++)
	   {
		   int routeNodeIndex = (40 * i) + j;
		   auto routeNode = _routeNodes.ptr(this)[routeNodeIndex];
		   vglVertex3f(routeNode.Position);
	   }
   
	   vglEnd();
   }
   
   rglPopMatrix();
}

void aiVehiclePhysics::BindLua(LuaState L) {
	LuaBinding(L).beginExtendClass<aiVehiclePhysics, aiVehicle>("aiVehiclePhysics")
		.addPropertyReadOnly("Car", &GetCar)
		.addPropertyReadOnly("State", &GetState)
		.addPropertyReadOnly("CurrentLap", &GetCurrentLap)
		.addPropertyReadOnly("NumLaps", &GetLapCount)
		.addFunction("Init", &initLua)
		.addFunction("DrawRouteThroughTraffic", &DrawRouteThroughTraffic)
		.endClass();
}