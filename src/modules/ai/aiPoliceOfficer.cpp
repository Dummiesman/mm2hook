#include "aiPoliceOfficer.h"
#include "aiMap.h"
#include "..\vehicle.h"
#include "..\rgl.h"


using namespace MM2;

aiPoliceOfficer::aiPoliceOfficer(void) {};
aiPoliceOfficer::aiPoliceOfficer(const aiPoliceOfficer&&) {};

void aiPoliceOfficer::FollowPerpetrator() { hook::Thunk<0x53E410>::Call<void>(this); }

void aiPoliceOfficer::ChaseVehicle(vehCar* chaseMe)
{
  // already chasing this car?
  if (this->GetFollowedCar() == chaseMe)
  {
    return;
  }
  
  auto policeForce = aiMap::GetInstance()->GetPoliceForce();
  
  // stop chasing the original vehicle first
  if (this->GetFollowedCar() != nullptr)
  {
    policeForce->UnRegisterCop(this->GetCar(), this->GetFollowedCar());
  }
  
  // chase the new vehicle
  if (policeForce->RegisterPerp(this->GetCar(), chaseMe))
  {
    this->GetVehiclePhysics()->SetState(aiVehiclePhysicsState::Forward);
    _followedCar.set(this, chaseMe);
  
    auto myPosition = this->GetCar()->GetICS()->GetPosition();
    auto otherPosition = chaseMe->GetICS()->GetPosition();
    _followedCarDistance.set(this, myPosition.Dist(otherPosition));
  
    short cmpIdx, cmpType;
    aiMap::GetInstance()->MapComponent(otherPosition, &cmpIdx, &cmpType, -1);
    _perpMapCompIndex.set(this, cmpIdx);
    _perpMapCompType.set(this, cmpType);
  
    _policeState.set(this, aiPoliceState::FollowPerp);
    this->FollowPerpetrator();
  }
}

int aiPoliceOfficer::GetId() const
{
	return _id.get(this);
}

aiVehiclePhysics* aiPoliceOfficer::GetVehiclePhysics() const
{
	return _vehiclePhysics.ptr(this);
}

int aiPoliceOfficer::GetApprehendState() const
{
	return _apprehendState.get(this);
}

vehCar* aiPoliceOfficer::GetFollowedCar() const
{
	return _followedCar.get(this);
}

vehCar* aiPoliceOfficer::GetCar() const
{
	return _vehiclePhysics.ptr(this)->GetCar();
}

short aiPoliceOfficer::GetState() const
{
	return _vehiclePhysics.ptr(this)->GetState();
}

int aiPoliceOfficer::GetCurrentLap() const
{
	return _vehiclePhysics.ptr(this)->GetCurrentLap();
}

int aiPoliceOfficer::GetLapCount() const
{
	return _vehiclePhysics.ptr(this)->GetLapCount();
}

void aiPoliceOfficer::DrawRouteThroughTraffic() const
{
	this->GetVehiclePhysics()->DrawRouteThroughTraffic();
}

void MM2::aiPoliceOfficer::DrawId() const
{
	this->GetVehiclePhysics()->DrawId();
}

/// <summary>
/// The state from aiPoliceForce::State
/// </summary>        
int aiPoliceOfficer::GetPoliceState() const
{
	return _policeState.get(this);
}

void MM2::aiPoliceOfficer::ChooseRandomAppBehavior()
{
	short chosenBehavior = _behaviorList.ptr(this)[(int)(frand() * _behaviorCount.get(this))];
	_apprehendState.set(this, (aiPoliceApprehendState)chosenBehavior);
}

AGE_API bool aiPoliceOfficer::InPersuit() const					     { return hook::Thunk<0x53E400>::Call<bool>(this); }
AGE_API void aiPoliceOfficer::StartSiren()                           { hook::Thunk<0x53DBF0>::Call<void>(this); }
AGE_API void aiPoliceOfficer::StopSiren()                            { hook::Thunk<0x53DC40>::Call<void>(this); }
void AGE_API aiPoliceOfficer::DetectPerpetrator()                    { hook::Thunk<0x53DFD0>::Call<void>(this); }
void AGE_API aiPoliceOfficer::PerpEscapes()                          { hook::Thunk<0x53F170>::Call<void>(this); }

void aiPoliceOfficer::BindLua(LuaState L) {
	LuaBinding(L).beginClass<aiPoliceOfficer>("aiPoliceOfficer")
		.addPropertyReadOnly("FollowedCar", &GetFollowedCar)
		.addPropertyReadOnly("PoliceState", &GetPoliceState)
		.addPropertyReadOnly("ApprehendState", &GetApprehendState)
		.addPropertyReadOnly("ID", &GetId)
		.addPropertyReadOnly("CurrentLap", &GetCurrentLap)
		.addPropertyReadOnly("NumLaps", &GetLapCount)
		.addPropertyReadOnly("InPursuit", &InPersuit)
		.addPropertyReadOnly("InPersuit", &InPersuit)

		.addPropertyReadOnly("Car", &GetCar)
		.addPropertyReadOnly("State", &GetState)

		.addFunction("ChaseVehicle", &ChaseVehicle)
		.addFunction("StartSiren", &StartSiren)
		.addFunction("StopSiren", &StopSiren)
		.addFunction("DrawRouteThroughTraffic", &DrawRouteThroughTraffic)
		.addFunction("DrawId", &DrawId)

		.endClass();
}