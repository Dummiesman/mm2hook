#include "aiPoliceOfficer.h"
#include "aiRouteRacer.h"
#include "aiMap.h"
#include "..\vehicle.h"
#include "..\rgl.h"


using namespace MM2;

bool aiPoliceOfficer::s_EnableRubberBanding = true;

aiPoliceOfficer::aiPoliceOfficer(void) : m_VehiclePhysics()
{
}

MM2::aiPoliceOfficer::~aiPoliceOfficer()
{

}

AGE_API void aiPoliceOfficer::Push()
{
	hook::Thunk<0x53E710>::Call<void>(this);
}

AGE_API void aiPoliceOfficer::Block()
{
	hook::Thunk<0x53EA60>::Call<void>(this);
}

AGE_API void MM2::aiPoliceOfficer::Ram()
{
	// normally inlined
	auto vehicleState = m_VehiclePhysics.GetState();
	if (vehicleState == aiVehiclePhysicsState::Forward || vehicleState == aiVehiclePhysicsState::Shortcut)
	{
		aiMap::GetInstance()->CalcRoute(m_VehiclePhysics.GetMatrix(), m_FollowCar->GetICS()->GetPosition(), Vector3::YAXIS,
			m_IntersectionIds, &m_NumIntersectionIds, m_VehiclePhysics.GetCar()->GetModel()->GetRoomId(),
			m_FollowCar->GetModel()->GetRoomId(), true);

		m_VehiclePhysics.RegisterRoute(m_IntersectionIds, m_NumIntersectionIds, 
									   m_FollowCar->GetICS()->GetPosition(), m_FollowCar->GetICS()->GetMatrix().GetRow(2) * -1.0,
									   0, m_FollowCar->GetCarSim()->GetSpeed() + 15.0f, 0.0f, false, true, true , false, true, false,
									   1.0f, 1.0f, 0.7f, 75.0f);
	}
}

AGE_API void aiPoliceOfficer::ApprehendPerpetrator()
{
	if (m_PoliceState != m_LastPoliceState)
	{
		m_LastPoliceState = m_PoliceState;
		m_ApprehendState = m_AllowedBehaviors[irand() % m_BehaviorCount];
	}
	if (m_ApprehendState == aiPoliceApprehendState::Block || m_ApprehendState == aiPoliceApprehendState::BlockWait)
	{
		this->Block();
	}
	else if (m_ApprehendState == aiPoliceApprehendState::PushLeft || m_ApprehendState == aiPoliceApprehendState::PushRight)
	{
		this->Push();
	}
	else 
	{
		this->Ram();
	}
}

void aiPoliceOfficer::FollowPerpetrator() { hook::Thunk<0x53E410>::Call<void>(this); }

void aiPoliceOfficer::CancelPursuit()
{
	if (m_PoliceState == aiPoliceState::Idle)
		return;

	// unregister
	auto policeForce = aiMap::GetInstance()->GetPoliceForce();
	policeForce->UnRegisterCop(GetCar(), GetFollowedCar());

	// stop siren
	auto policeAudio = GetCar()->GetCarAudioContainerPtr()->GetPoliceCarAudioPtr();
	if (policeAudio != nullptr)
	{
		policeAudio->StopSiren();
	}

	// stop car and set our state
	m_PoliceState = aiPoliceState::Idle;
	m_VehiclePhysics.SetState(aiVehiclePhysicsState::Stop);
	m_FollowCar = nullptr;
}

bool aiPoliceOfficer::ChaseVehicle(vehCar* chaseMe)
{
	if (m_PoliceState == aiPoliceState::Incapacitated)
		return false;

	auto policeForce = aiMap::GetInstance()->GetPoliceForce();
	if (m_PoliceState != aiPoliceState::Idle)
	{
		// already chasing this car?
		if (policeForce->IsCopChasingPerp(GetCar(), chaseMe))
		{
			return true;
		}

		// stop chasing the original vehicle first
		this->CancelPursuit();
	}
    
    // chase the new vehicle
    if (policeForce->RegisterPerp(GetCar(), chaseMe))
    {
       m_VehiclePhysics.SetState(aiVehiclePhysicsState::Forward);
	   m_FollowCar = chaseMe;
	   m_FollowCarDistance = GetCar()->GetICS()->GetPosition().Dist(chaseMe->GetICS()->GetPosition());
       
       aiMap::GetInstance()->MapComponent(m_FollowCar->GetICS()->GetPosition(), &m_PerpComponentID, &m_PerpComponentType, -1);
	   
	   m_PoliceState = aiPoliceState::FollowPerp;
       this->FollowPerpetrator();
	   return true;
    }
	return false;
}

int aiPoliceOfficer::GetId() const
{
	return m_ID;
}

const aiVehiclePhysics* aiPoliceOfficer::GetVehiclePhysics() const
{
	return &m_VehiclePhysics;
}

int aiPoliceOfficer::GetApprehendState() const
{
	return m_ApprehendState;
}

vehCar* aiPoliceOfficer::GetFollowedCar() const
{
	return m_FollowCar;
}

vehCar* aiPoliceOfficer::GetCar() const
{
	return m_VehiclePhysics.GetCar();
}

short aiPoliceOfficer::GetState() const
{
	return m_VehiclePhysics.GetState();
}

void aiPoliceOfficer::SetState(aiVehiclePhysicsState state)
{
	m_VehiclePhysics.SetState(state);
}

int aiPoliceOfficer::GetCurrentLap() const
{
	return m_VehiclePhysics.GetCurrentLap();
}

int aiPoliceOfficer::GetLapCount() const
{
	return m_VehiclePhysics.GetLapCount();
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
	return m_PoliceState;
}

void aiPoliceOfficer::SetPoliceState(aiPoliceState state)
{
	m_PoliceState = state;
}

void MM2::aiPoliceOfficer::Init(int id)
{
	auto raceData = aiMap::GetInstance()->GetRaceData();
	auto ourData = raceData->GetPoliceData(id);
	auto carsim = m_VehiclePhysics.GetCar()->GetCarSim();

	Init(id, ourData->Basename, ourData->Flags);
	
	carsim->SetResetPos(ourData->Position);
	carsim->SetResetRotation(ourData->Rotation);
	ChaseRange = raceData->GetCopChaseRange();
	OpponentDetectionRange = ourData->OppDetectRange;
	OpponentChaseChance = ourData->OppChaseChance;
}

void MM2::aiPoliceOfficer::Init(int id, const char* basename, int flags)
{
	m_ID = id;
	m_VehiclePhysics.Init(id, basename, FALSE, 1);
	GetCar()->GetStuck()->SetTimeThresh(0.75f);

	m_BehaviorCount = 0;
	if (flags & 1) m_AllowedBehaviors[m_BehaviorCount++] = 6; // Block
	//if (flags & 2) m_AllowedBehaviors[m_BehaviorCount++] = 8; // Invalid (potentially was Barricade)
	if (flags & 4) m_AllowedBehaviors[m_BehaviorCount++] = 4; // Push
	if (flags & 8) m_AllowedBehaviors[m_BehaviorCount++] = 3; // Ram?
	
	ChasePlayers = true;
	ChaseOpponents = true;
	ChaseRange = 250.0f;
	OpponentDetectionRange = 50.0f;
	OpponentChaseChance = 0.5f;

	auto policeAudio = GetVehiclePhysics()->GetCar()->GetCarAudioContainerPtr()->GetPoliceCarAudioPtr();
	if (policeAudio)
	{
		policeAudio->SetMaxAttenuationDistance(625.0f);
		policeAudio->SetDropOffs(0.0f, 150.0f);
		policeAudio->SetVolume3D(1.05f);
		policeAudio->SetMinAmpSpeed(5.0f);
	}
}	

AGE_API void aiPoliceOfficer::Reset()  
{
	for (int i = 0; i < 8; i++)
	{
		m_OpponentChaseDenyList[i] = 0;
	}
	
	if (m_PoliceState != aiPoliceState::Idle)
	{
		aiMap::GetInstance()->GetPoliceForce()->UnRegisterCop(m_VehiclePhysics.GetCar(), m_FollowCar);
	}

	m_LastPoliceState = -1;
	m_PoliceState = aiPoliceState::Idle;
	m_ApprehendState = aiPoliceApprehendState::Ram;

	auto policeAudio = m_VehiclePhysics.GetCar()->GetCarAudioContainerPtr()->GetPoliceCarAudioPtr();
	if (policeAudio)
	{
		policeAudio->Reset();
	}	
	this->StopSiren();	

	m_VehiclePhysics.Reset();
	m_NumIntersectionIds = 0;
	m_VehiclePhysics.RegisterRoute(m_IntersectionIds, m_NumIntersectionIds, m_VehiclePhysics.GetCar()->GetCarSim()->GetResetPos(), Vector3::ORIGIN,
								   0, 0.0f, 5.0f, false, true, true, false, true, false,
							       1.0f, 2.0f, 0.7f, 75.0f);
}

AGE_API void MM2::aiPoliceOfficer::Update()
{
	auto AIMAP = aiMap::GetInstance();
	if (m_PoliceState != aiPoliceState::Incapacitated)
	{
		if (m_PoliceState != aiPoliceState::Idle)
		{
			AIMAP->MapComponent(m_FollowCar->GetICS()->GetPosition(), &m_PerpComponentID, &m_PerpComponentType, m_FollowCar->GetModel()->GetRoomId());
			m_FollowCarDistance = m_FollowCar->GetICS()->GetPosition().FlatDist(m_VehiclePhysics.GetCar()->GetICS()->GetPosition());
			m_PoliceState = AIMAP->GetPoliceForce()->State(m_VehiclePhysics.GetCar(), m_FollowCar, m_FollowCarDistance);

			if (m_FollowCar->GetCarSim()->GetTransmission()->GetGear() == 0 || m_VehiclePhysics.GetState() == aiVehiclePhysicsState::Backup
				|| m_BehaviorCount == 0 || m_FollowCar->GetCarSim()->GetSpeed() < 10.0)
			{
				m_PoliceState = aiPoliceState::FollowPerp;
			}

			if (m_PoliceState == aiPoliceState::FollowPerp)
			{
				this->FollowPerpetrator();
			}
			else 
			{
				this->ApprehendPerpetrator();
			}

			if (m_FollowCarDistance > ChaseRange)
			{
				this->PerpEscapes(false);
				return;
			}

			if (s_EnableRubberBanding && m_VehiclePhysics.GetThrottleInput() == 1.0f && m_VehiclePhysics.Speed() < 50.0f)
			{
				auto ics = m_VehiclePhysics.GetCar()->GetICS();
				ics->SetVelocity(ics->GetVelocity() * 1.03f);
			}
		}
		else
		{
			this->DetectPerpetrator();
		}

		// did we damage out?
		if (m_VehiclePhysics.IsDamagedOut())
		{
			this->PerpEscapes(true);
			m_PoliceState = aiPoliceState::Incapacitated;
		}

		// did we fall in the water?
		if (m_VehiclePhysics.GetCar()->GetSplash()->isActive())
		{
			this->PerpEscapes(false);
			m_PoliceState = aiPoliceState::Incapacitated;
		}
	}

	// update audio
	auto policeAudio = m_VehiclePhysics.GetCar()->GetCarAudioContainerPtr()->GetPoliceCarAudioPtr();
	if (policeAudio)
	{
		policeAudio->Update();
	}

	// drive route
	if (m_PoliceState == aiPoliceState::Apprehend && m_ApprehendState == aiPoliceApprehendState::BlockWait)
	{
		m_VehiclePhysics.Mirror(m_FollowCar);
	}
	else
	{
		m_VehiclePhysics.DriveRoute();
	}

	// simulate physics if close enough to the perp, or if a player is close
	if (this->InPersuit())
	{
		if (m_FollowCarDistance <= 200.0f || !dgPhysManager::sm_OpponentOptimization)
			dgPhysManager::Instance->DeclareMover(m_VehiclePhysics.GetCar()->GetModel(), 2, 0x1B);
		else
			dgPhysManager::Instance->DeclareMover(m_VehiclePhysics.GetCar()->GetModel(), 2, 0x13);
	}
	else
	{
		float closestPlayerDist2 = 9999999.0f;
		auto ourPosition = m_VehiclePhysics.GetCar()->GetICS()->GetPosition();
		for (int i = 0; i < AIMAP->GetPlayerCount(); i++)
		{
			auto player = AIMAP->Player(i);
			float dist2 = (player->GetCar()->GetICS()->GetPosition() - ourPosition).Mag2();
			closestPlayerDist2 = fminf(dist2, closestPlayerDist2);
		}

		float closestPlayerDist = sqrtf(closestPlayerDist2);
		if (closestPlayerDist <= 250.0f)
		{
			if (closestPlayerDist <= 200.0f || !dgPhysManager::sm_OpponentOptimization)
				dgPhysManager::Instance->DeclareMover(m_VehiclePhysics.GetCar()->GetModel(), 2, 0x1B);
			else
				dgPhysManager::Instance->DeclareMover(m_VehiclePhysics.GetCar()->GetModel(), 2, 0x13);
		}
	}
	
	// check if we've fallen and can't get up
	if (this->m_VehiclePhysics.GetCar()->GetICS()->GetPosition().Y < -200.0)
	{
		Warningf("Police Officer #%d, Has fallen through the geometry.", m_ID);
		this->Reset();
	}
}

AGE_API bool aiPoliceOfficer::InPersuit() const
{
	return m_PoliceState != aiPoliceState::Idle && m_PoliceState != aiPoliceState::Incapacitated;
}

AGE_API void aiPoliceOfficer::StartSiren()                           { hook::Thunk<0x53DBF0>::Call<void>(this); }
AGE_API void aiPoliceOfficer::StopSiren()                            { hook::Thunk<0x53DC40>::Call<void>(this); }

AGE_API bool aiPoliceOfficer::Fov(vehCar* perpCar)
{
	auto ourMatrix = m_VehiclePhysics.GetCar()->GetICS()->GetMatrix();
	Vector3 dirToCar = perpCar->GetICS()->GetPosition() - m_VehiclePhysics.GetCar()->GetICS()->GetPosition();

	float sideDist = ourMatrix.GetRow(0).Dot(dirToCar);
	float forwardDist = ourMatrix.GetRow(2).Dot(dirToCar * -1.0f);
	float angle = atan2f(sideDist, forwardDist);

	return angle > -1.57f && angle < 1.57f;
}

AGE_API BOOL aiPoliceOfficer::Collision(vehCar* perpCar)             { return hook::Thunk<0x53E370>::Call<BOOL>(this, perpCar); }
AGE_API BOOL aiPoliceOfficer::HitMe(vehCar* perpCar)                 { return hook::Thunk<0x53E390>::Call<BOOL>(this, perpCar); }

AGE_API bool aiPoliceOfficer::IsPerpACop(vehCar* perpCar)
{
	char* vehName = perpCar->GetCarDamage()->GetName(); // we can't use vehCarSim because the game forces vpcop to vpmustang99...
	return vehCarAudioContainer::IsPolice(vehName);
}

bool aiPoliceOfficer::OffRoad(vehCar* perpCar)
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
			return true;
	}
	else if (currentComponentType == aiMapComponentType::None || currentComponentType == aiMapComponentType::Shortcut)
	{
		return true;
	}

	return false;
}

AGE_API bool aiPoliceOfficer::WrongWay(vehCar* perpCar)
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

	return false;
}

AGE_API bool aiPoliceOfficer::Speeding(vehCar* perpCar)
{
	auto AIMAP = aiMap::GetInstance();
	short cmpId, cmpType;
	AIMAP->MapComponent(perpCar->GetICS()->GetPosition(), &cmpId, &cmpType, perpCar->GetModel()->GetRoomId());

	if (static_cast<aiMapComponentType>(cmpType) == aiMapComponentType::Road || static_cast<aiMapComponentType>(cmpType) == aiMapComponentType::Shortcut)
	{
		auto  path = aiMap::GetInstance()->Path(cmpId);
		float speedLimit = 4.0f + path->GetBaseSpeedLimit() + 1.0f; // Max AI ExheedLimit + Speed Limit + Lenieance so 0.01mph over isn't speeding
		if (path->GetFlags() & aiPath::FLAG_FREEWAY)
		{
			speedLimit += (max(path->GetLaneCount(0), path->GetLaneCount(1)) - 1) * 5.0f; // Maximum freeway limit
		}
		return perpCar->GetCarSim()->GetSpeed() > speedLimit;
	}
	return false;
}

void AGE_API aiPoliceOfficer::DetectPerpetrator() 
{
	if (m_LastPoliceState != m_PoliceState)
	{
		m_LastPoliceState = m_PoliceState;
	}

	auto AIMAP = aiMap::GetInstance();
	auto policeForce = AIMAP->GetPoliceForce();

	// look for players
	if (ChasePlayers)
	{
		for (int i = 0; i < AIMAP->GetPlayerCount(); i++)
		{
			auto player = AIMAP->Player(i);
			auto playerCar = player->GetCar();
			auto ourCar = m_VehiclePhysics.GetCar();
			float dist2 = (playerCar->GetICS()->GetPosition() - ourCar->GetICS()->GetPosition()).Mag2();

			if (dist2 < 5625.0f && this->Fov(playerCar) && this->IsPerpBreakingTheLaw(playerCar))
			{
				if (this->ChaseVehicle(playerCar))
				{
					return;
				}
			}
		}
	}

	// look for opponents
	if (ChaseOpponents)
	{
		for (int i = 0; i < AIMAP->GetOpponentCount(); i++)
		{
			if (!m_OpponentChaseDenyList[i])
			{
				auto opponent = AIMAP->Opponent(i);
				auto opponentCar = opponent->GetCar();
				auto ourCar = m_VehiclePhysics.GetCar();
				float dist2 = (opponentCar->GetICS()->GetPosition() - ourCar->GetICS()->GetPosition()).Mag2();

				if (dist2 < 5625.0f && dist2 < (OpponentDetectionRange * OpponentDetectionRange) 
					&& this->Fov(opponentCar) && this->IsPerpBreakingTheLaw(opponentCar))
				{
					float chance = frand();
					if (chance <= OpponentChaseChance)
					{
						if (this->ChaseVehicle(opponentCar))
						{
							return;
						}
					}
					else
					{
						m_OpponentChaseDenyList[i] = TRUE;
					}
				}
			}
		}
	}
}

void AGE_API aiPoliceOfficer::PerpEscapes(bool playExplosion)        { hook::Thunk<0x53F170>::Call<void>(this, playExplosion); }

bool MM2::aiPoliceOfficer::IsPerpBreakingTheLaw(vehCar* perpCar)
{
	if (aiMap::GetInstance()->GetPoliceForce()->GetNumChasers(perpCar))
	{
		return true;
	}
	return !this->IsPerpACop(perpCar) && (this->OffRoad(perpCar) || this->Speeding(perpCar));
}

void aiPoliceOfficer::BindLua(LuaState L) {
	LuaBinding(L).beginClass<aiPoliceOfficer>("aiPoliceOfficer")
		.addFactory([]() {
			auto object = new aiPoliceOfficer();
			//MM2Lua::MarkForCleanupOnShutdown(object);
			return object;
		})
		.addPropertyReadOnly("FollowedCar", &GetFollowedCar)
		.addPropertyReadOnly("PoliceState", &GetPoliceState)
		.addPropertyReadOnly("ApprehendState", &GetApprehendState)
		.addPropertyReadOnly("ID", &GetId)
		.addPropertyReadOnly("CurrentLap", &GetCurrentLap)
		.addPropertyReadOnly("NumLaps", &GetLapCount)
		.addPropertyReadOnly("InPursuit", &InPersuit)
		.addPropertyReadOnly("InPersuit", &InPersuit)

		.addVariable("ChaseRange", &aiPoliceOfficer::ChaseRange)
		.addVariable("ChasePlayers", &aiPoliceOfficer::ChasePlayers)
		.addVariable("ChaseOpponents", &aiPoliceOfficer::ChaseOpponents)
		.addVariable("OpponentChaseChance", &aiPoliceOfficer::OpponentChaseChance)
		.addVariable("OpponentDetectionRange", &aiPoliceOfficer::OpponentDetectionRange)

		.addPropertyReadOnly("Car", &GetCar)
		.addPropertyReadOnly("State", &GetState)

		.addFunction("Init", static_cast<void (aiPoliceOfficer::*)(int, const char*, int)>(&Init))
		.addFunction("Reset", &Reset)
		.addFunction("Update", &Update)
		.addFunction("CancelPursuit", &CancelPursuit)
		.addFunction("ChaseVehicle", &ChaseVehicle)
		.addFunction("StartSiren", &StartSiren)
		.addFunction("StopSiren", &StopSiren)
		.addFunction("DrawRouteThroughTraffic", &DrawRouteThroughTraffic)
		.addFunction("DrawId", &DrawId)

		.endClass();
}