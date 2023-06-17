#pragma once
#include "car.h"

using namespace MM2;

void vehCar::setDrivable(bool drivable, int mode)
{
	this->SetDrivable(drivable ? TRUE : FALSE, mode);
}
		
AGE_API vehCar::vehCar(BOOL a1)                             { hook::Thunk<0x42BAB0>::Call<void>(this, a1); }
AGE_API vehCar::~vehCar()                                   { hook::Thunk<0x42BCC0>::Call<void>(this); }

declfield(vehCar::sm_DrawHeadlights)(0x627518);

vehCarDamage * vehCar::GetCarDamage(void) const {
	return _damage.get(this);
};

vehCarSim * vehCar::GetCarSim() const {
	return _sim.get(this);
}

vehCarModel * vehCar::GetModel() const {
	return _model.get(this);
}

vehSplash * vehCar::GetSplash() const {
	return _splash.get(this);
}

vehCarAudioContainer * vehCar::GetCarAudioContainerPtr() const {
	return _audio.get(this);
}

vehTrailer * vehCar::GetTrailer() const {
	return _trailer.get(this);
}

vehSiren * vehCar::GetSiren() const {
	return _siren.get(this);
}

vehWheelPtx * vehCar::GetWheelPtx() const {
	return _wheelPtx.get(this);
}

vehGyro * vehCar::GetGyro() const {
	return _gyro.get(this);
}

vehStuck * vehCar::GetStuck() const {
	return _stuck.get(this);
}

AGE_API void vehCar::Reset()                                { hook::Thunk<0x42C330>::Call<void>(this); }

AGE_API void vehCar::ClearDamage()                          { hook::Thunk<0x42C450>::Call<void>(this); }
AGE_API bool vehCar::IsPlayer()                             { return hook::Thunk<0x42C890>::Call<bool>(this); }
AGE_API void vehCar::Init(char const *basename, int variant, int colliderID, bool useFullBound, bool hasTrailer)
															{ hook::Thunk<0x42BE10>::Call<void>(this, basename, variant, colliderID, useFullBound, hasTrailer); }
AGE_API void vehCar::InitAudio(char const *basename, int audioType)      
															{ hook::Thunk<0x42C1F0>::Call<void>(this, basename, audioType); }
AGE_API void vehCar::SetDrivable(BOOL drivable, int mode)   { hook::Thunk<0x42C2C0>::Call<void>(this, drivable, mode); }

AGE_API void MM2::vehCar::SetColliderID(int id)
{
	hook::Thunk<0x42C1D0>::Call<void>(this, id);
}

/*
	dgPhysEntity virtuals
*/

AGE_API bool vehCar::RequiresTerrainCollision()
												   { return hook::Thunk<0x42CA90>::Call<bool>(this); }
AGE_API lvlInstance* vehCar::GetInst()             { return hook::Thunk<0x42CA80>::Call<lvlInstance *>(this); }
AGE_API phInertialCS* vehCar::GetICS()             { return hook::Thunk<0x42CA70>::Call<phInertialCS *>(this); }
AGE_API void vehCar::PostUpdate()                  { hook::Thunk<0x42C8B0>::Call<void>(this); }
AGE_API void vehCar::Update()                      { hook::Thunk<0x42C690>::Call<void>(this); }
AGE_API void vehCar::PreUpdate()                   { hook::Thunk<0x42C480>::Call<void>(this); }

void vehCar::BindLua(LuaState L) {
	LuaBinding(L).beginExtendClass<vehCar, dgPhysEntity>("vehCar")
		.addFactory([]() {
			auto object = new vehCar(TRUE);
			MM2Lua::MarkForCleanupOnShutdown(object);
			return object;
		})

		//properties
		.addPropertyReadOnly("CarDamage", &GetCarDamage)
		.addPropertyReadOnly("CarSim", &GetCarSim)
		.addPropertyReadOnly("Splash", &GetSplash)
		.addPropertyReadOnly("Audio", &GetCarAudioContainerPtr)
		.addPropertyReadOnly("Trailer", &GetTrailer)
		.addPropertyReadOnly("Siren", &GetSiren)
		.addPropertyReadOnly("WheelPtx", &GetWheelPtx)
		.addPropertyReadOnly("Gyro", &GetGyro)
		.addPropertyReadOnly("Stuck", &GetStuck)

		.addPropertyReadOnly("Instance", &GetModel)

		//functions
		.addFunction("Init", &Init)
		.addFunction("InitAudio", &InitAudio)
		.addFunction("Reset", &Reset)
		.addFunction("ClearDamage", &ClearDamage)
		.addFunction("SetDrivable", &setDrivable, LUA_ARGS(bool, _def<int, 3>))
		.addFunction("IsPlayer", &IsPlayer)
		.addFunction("SetColliderID", &SetColliderID)
	.endClass();
}
