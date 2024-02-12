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

void aiPoliceOfficer::SetState(aiVehiclePhysicsState state)
{
	_vehiclePhysics.ptr(this)->SetState(state);
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

void aiPoliceOfficer::SetPoliceState(aiPoliceState state)
{
	_policeState.set(this, state);
}

void MM2::aiPoliceOfficer::ChooseRandomAppBehavior()
{
	short chosenBehavior = _behaviorList.ptr(this)[(int)(frand() * _behaviorCount.get(this))];
	_apprehendState.set(this, (aiPoliceApprehendState)chosenBehavior);
}

AGE_API void aiPoliceOfficer::Reset()                                { hook::Thunk<0x53DAA0>::Call<void>(this); }
AGE_API bool aiPoliceOfficer::InPersuit() const                      { return hook::Thunk<0x53E400>::Call<bool>(this); }
AGE_API void aiPoliceOfficer::StartSiren()                           { hook::Thunk<0x53DBF0>::Call<void>(this); }
AGE_API void aiPoliceOfficer::StopSiren()                            { hook::Thunk<0x53DC40>::Call<void>(this); }

AGE_API BOOL aiPoliceOfficer::Fov(vehCar* perpCar)
{
	if (!hook::Thunk<0x53E2A0>::Call<BOOL>(this, perpCar))
		return FALSE;

	Vector3 copPosition = this->GetCar()->GetCarSim()->GetICS()->GetPosition();
	Vector3 perpPosition = perpCar->GetCarSim()->GetICS()->GetPosition();

	lvlSegment segment;
	lvlIntersection intersection;

	copPosition.Y += 1.f;
	perpPosition.Y += 1.f;

	segment.Set(copPosition, perpPosition, 0, nullptr);
	return dgPhysManager::Instance->Collide(segment, &intersection, 0, nullptr, (ushort)RoomFlags::SpecialBound, 0) ? FALSE : TRUE;
}

AGE_API BOOL aiPoliceOfficer::Collision(vehCar* perpCar)             { return hook::Thunk<0x53E370>::Call<BOOL>(this, perpCar); }
AGE_API BOOL aiPoliceOfficer::HitMe(vehCar* perpCar)                 { return hook::Thunk<0x53E390>::Call<BOOL>(this, perpCar); }

AGE_API BOOL aiPoliceOfficer::IsPerpACop(vehCar* perpCar)
{
	char* vehName = perpCar->GetCarDamage()->GetName(); // we can't use vehCarSim because the game forces vpcop to vpmustang99...
	return vehCarAudioContainer::IsPolice(vehName) ? TRUE : FALSE;
}

AGE_API BOOL aiPoliceOfficer::OffRoad(vehCar* perpCar)
{
	auto AIMAP = aiMap::GetInstance();
	auto carSim = perpCar->GetCarSim();
	auto carPosition = carSim->GetICS()->GetPosition();

	short outId, outType;
	AIMAP->MapComponent(carPosition, &outId, &outType, perpCar->GetModel()->GetRoomId());

	auto currentComponentType = static_cast<aiMapComponentType>(outType);
	if (currentComponentType == aiMapComponentType::Road)
	{
		auto path = AIMAP->Path(outId);

		float outDistanceFromCenter;
		if (path->IsPosOnRoad(carPosition, 0.f, &outDistanceFromCenter) == 2) // Sidewalk
			return TRUE;
	}
	else if (currentComponentType == aiMapComponentType::None || currentComponentType == aiMapComponentType::Shortcut)
	{
		return TRUE;
	}

	return FALSE;
}

AGE_API BOOL aiPoliceOfficer::WrongWay(vehCar* perpCar)
{
	auto AIMAP = aiMap::GetInstance();
	auto carSim = perpCar->GetCarSim();
	auto carMatrix = carSim->GetICS()->GetMatrix();
	auto carVelocity = carSim->GetICS()->GetVelocity();

	short outId, outType;
	AIMAP->MapComponent(carMatrix.GetRow(3), &outId, &outType, perpCar->GetModel()->GetRoomId());

	if (static_cast<aiMapComponentType>(outType) == aiMapComponentType::Road)
	{
		auto path = AIMAP->Path(outId);
		int vIndex = path->RoadVertice(carMatrix.GetRow(3), 1);

		Vector3 zOri = path->GetForwardDirection(vIndex);
		float ddot = zOri.Dot(carMatrix.GetRow(2)) * -1.0f;
		float vddot = carVelocity.Dot(carMatrix.GetRow(2));

		int reverseSide = AIMAP->GetCityData()->AreAmbientsDriveOnLeft() ? 0 : 1;
		if (!path->IsOneWay())
		{
			Vector3 localPos = carMatrix.GetRow(3) - path->GetCenterVertex(vIndex);
			Vector3 xOri = path->GetSideDirection(vIndex);
			int side = (localPos.Dot(xOri) > 0) ? 1 : 0;
			if (side == reverseSide) ddot *= -1.0f;
		}

		return (ddot > 0 || (ddot <= 0 && vddot > 0.0f && carSim->GetSpeedMPH() >= 10.0f)) ? TRUE : FALSE; // Going backwards, or reversing >10mph
	}

	return FALSE;
}

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

		.addFunction("Reset", &Reset)
		.addFunction("ChaseVehicle", &ChaseVehicle)
		.addFunction("StartSiren", &StartSiren)
		.addFunction("StopSiren", &StopSiren)
		.addFunction("DrawRouteThroughTraffic", &DrawRouteThroughTraffic)
		.addFunction("DrawId", &DrawId)

		.endClass();
}