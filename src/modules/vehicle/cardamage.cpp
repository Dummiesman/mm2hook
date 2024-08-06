#pragma once
#include "cardamage.h"

using namespace MM2;

/*
	vehDamageImpactInfo
*/
void vehDamageImpactInfo::BindLua(LuaState L) {
	LuaBinding(L).beginClass<vehDamageImpactInfo>("vehDamageImpactInfo")
		.addVariable("LocalPosition", &vehDamageImpactInfo::LocalPosition, false)
		.addVariable("WorldPosition", &vehDamageImpactInfo::WorldPosition, false)
		.addVariable("Normal", &vehDamageImpactInfo::Normal, false)
		.addVariable("Impulse", &vehDamageImpactInfo::Impulse, false)
		.addVariable("Damage", &vehDamageImpactInfo::Damage, false)
		.addVariable("TotalDamage", &vehDamageImpactInfo::TotalDamage, false)
		.addVariable("RelaxTime", &vehDamageImpactInfo::RelaxTime, false)
		.addVariableRef("OtherCollider", &vehDamageImpactInfo::OtherCollider, false)
		.endClass();
}
		
/*
	vehCarDamage
*/
   
declfield(vehCarDamage::EngineSmokeRule)(0x6AFED4);

void vehCarDamage::setGameCallbackLua(LuaRef fn)
{
	GameCallback = datCallback::CreateLuaCallback<vehDamageImpactInfo*>(fn, this);
}
    
AGE_API vehCarDamage::vehCarDamage()                              { hook::Thunk<0x4CA380>::Call<void>(this); }
AGE_API vehCarDamage::~vehCarDamage()                             { hook::Thunk<0x4CA530>::Call<void>(this); }

AGE_API void vehCarDamage::AddDamage(float amount)                { hook::Thunk<0x4CAEC0>::Call<void>(this, amount); }
AGE_API void vehCarDamage::ClearDamage()                          { hook::Thunk<0x4CAE80>::Call<void>(this); }

/*
	vehCarDamage virtuals
*/

AGE_API float vehCarDamage::GetDamageModifier(phCollider* a1)
													{ return hook::Thunk<0x4CB650>::Call<float>(this, a1); }
        
/*
	asNode virtuals
*/

AGE_API void vehCarDamage::Update()			                      { hook::Thunk<0x5B2C30>::Call<void>(this); }
AGE_API void vehCarDamage::Reset()		                          { hook::Thunk<0x4CAE60>::Call<void>(this); }
AGE_API void vehCarDamage::FileIO(datParser &parser)		      { hook::Thunk<0x4CB400>::Call<void>(this); }
AGE_API char* vehCarDamage::GetClassName()                        { return hook::Thunk<0x4CB640>::Call<char*>(this); }
AGE_API char const* vehCarDamage::GetDirName()                    { return hook::Thunk<0x4CA5F0>::Call<char const*>(this); }
        
/*
	vehCarDamage
*/
float vehCarDamage::GetCurDamage() const { return this->CurrentDamage; }
float vehCarDamage::GetMedDamage() const { return this->MedDamage; }
float vehCarDamage::GetMaxDamage() const { return this->MaxDamage; }
float vehCarDamage::vehCarDamage::GetImpactThreshold() const { return this->ImpactThreshold; }
void vehCarDamage::SetImpactThreshold(float threshold) { this->ImpactThreshold = threshold; }
vehCar* vehCarDamage::GetCar() const { return this->vehCarPtr; }
asParticles* vehCarDamage::GetEngineSmokePtx() const { return this->Particles; }
asLineSparks* vehCarDamage::GetSparkomatic() const { return this->Sparks; }

// TEMP DEBUG STUFF
int vehCarDamage::GetNumActiveImpacts() const
{
	for (int i = 0; i < 12; i++)
	{
		if (this->ImpactsTable[i].OtherCollider == nullptr)
			return i;
	}
	return 12;
}

const vehDamageImpactInfo* vehCarDamage::GetImpact(int id) const
{
	auto tablePtr = &this->ImpactsTable;
	return tablePtr[id];
}

void vehCarDamage::BindLua(LuaState L) {
	LuaBinding(L).beginExtendClass<vehCarDamage, asNode>("vehCarDamage")
		.addPropertyReadOnly("NumActiveImpacts", &GetNumActiveImpacts)
		.addFunction("GetImpact", &GetImpact)

		.addFunction("Reset", &Reset)
		.addFunction("AddDamage", &AddDamage)
		.addFunction("ClearDamage", &ClearDamage)
		.addFunction("SetGameCallback", &setGameCallbackLua)

		.addVariable("DamageAmount", &vehCarDamage::CurrentDamage)

		.addVariable("Enabled", &vehCarDamage::EnableDamage)
		.addVariable("MedDamage", &vehCarDamage::MedDamage)
		.addVariable("MaxDamage", &vehCarDamage::MaxDamage)
		.addVariable("ImpactThreshold", &vehCarDamage::ImpactThreshold)
		.addVariable("RegenerateRate", &vehCarDamage::RegenerateRate)
		.addVariable("SmokeOffset", &vehCarDamage::SmokeOffset)
		.addVariable("SmokeOffset2", &vehCarDamage::SmokeOffset2)
		.addVariable("TextelDamageRadius", &vehCarDamage::TextelDamageRadius)
		.addVariable("DoublePivot", &vehCarDamage::DoublePivot)
		.addVariable("MirrorPivot", &vehCarDamage::MirrorPivot)

		.addVariable("SparkMultiplier", &vehCarDamage::SparkMultiplier)

		.addPropertyReadOnly("Car", &GetCar)
		.addPropertyReadOnly("EngineSmokeParticles", &GetEngineSmokePtx)
		.addPropertyReadOnly("Sparks", &GetSparkomatic)
		.addPropertyReadOnly("Sparkomatic", &GetSparkomatic)
		.addStaticProperty("EngineSmokeRule", [] { return EngineSmokeRule.get(); })
		.endClass();
}