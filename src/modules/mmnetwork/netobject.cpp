#include "netobject.h"
#include <modules/vehicle/car.h>

using namespace MM2;

int MM2::mmNetObject::SetPositionDataExtended(DPID sendTo, bool reset)
{
    if (!NETMGR->getInSession())
        return 0;

    // accel compute
    if (_generation.get(this))
    {
        float delta = NETMGR->GetTime() - _lastAccelComputeTime.get(this);
        _accelcompute.ptr(this)->SetLatest(GetCar()->GetICS()->GetVelocity(), delta);
    }
    else
    {
        _accelcompute.ptr(this)->Init(Vector3::ORIGIN, GetCar()->GetICS()->GetVelocity());
    }
    _lastAccelComputeTime.set(this, NETMGR->GetTime());

    // setup data
    auto posData = _posData.ptr(this);
    posData->dword_00 = _setToDw00.get(this);

    auto carsim = GetCar()->GetCarSim();
    posData->steering = static_cast<char>(carsim->GetSteering() * 127.0f);
    posData->throttle = static_cast<char>(carsim->GetThrottle() * 127.0f);
    posData->brake = static_cast<char>(carsim->GetBrake() * 127.0f);
    posData->gear = carsim->GetTransmission()->GetGear();

    auto ics = GetCar()->GetICS();
    posData->angle = ics->GetMatrix().GetEulers();
    posData->position = ics->GetPosition();
    
    Vector3 velocity = ics->GetVelocity();
    posData->velocity_len = velocity.Mag();
    velocity.Scale(1.0f / posData->velocity_len);
    posData->xvel = static_cast<char>(velocity.X * 127.0f);
    posData->yvel = static_cast<char>(velocity.Y * 127.0f);
    posData->zvel = static_cast<char>(velocity.Z * 127.0f);

    velocity = ics->GetVelocity();
    posData->velocity_len_2 = velocity.Mag();
    velocity.Scale(1.0f / posData->velocity_len_2);
    posData->xvel2 = static_cast<char>(velocity.X * 127.0f);
    posData->yvel2 = static_cast<char>(velocity.Y * 127.0f);
    posData->zvel2 = static_cast<char>(velocity.Z * 127.0f);

    Vector3 angvel = ics->GetAngVelocity();
    posData->angvel_scale = angvel.Mag();
    angvel.Scale(1.0f / posData->angvel_scale);
    posData->xangvel = static_cast<char>(angvel.X * 127.0f);
    posData->yangvel = static_cast<char>(angvel.Y * 127.0f);
    posData->zangvel = static_cast<char>(angvel.Z * 127.0f);

    posData->damage = GetCar()->GetCarDamage()->GetCurDamage();
    
    _generation.set(this, _generation.get(this) + 1);
    posData->score = _score.get(this);
    posData->generation = _generation.get(this);

    auto siren = GetCar()->GetSiren();
    posData->flags = 0; 
    if (GetCar()->GetCarAudioContainerPtr()->GetIsHornPlaying()) posData->flags |= 2;
    if (siren->GetHasLights() && siren->IsActive()) posData->flags |= 8;
    if (carsim->GetTransmission()->GetGear() == 0) posData->flags |= 4;
    if (carsim->GetHandbrake() > 0.8f) posData->flags |= 32;
    if (carsim->GetTransmission()->IsAuto()) posData->flags |= 64;
    if (reset) flags |= 256;
    flags |= _someFlagMask.get(this);

    // New packet
    NET_POSDATA2 data2;
    memcpy(&data2, posData, sizeof(NET_POSDATA));
    data2.objectId = this->ID;
    return NETMGR->Send(sendTo, 0x1F5, &data2, sizeof(NET_POSDATA2), FALSE);
}