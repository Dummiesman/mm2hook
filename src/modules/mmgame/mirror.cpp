#include "mirror.h"
using namespace MM2;

/*
	mmMirror
*/

Vector3 mmMirror::GetPosition() const { return this->matrix.GetRow(3); }
void mmMirror::SetPosition(const Vector3& position) { this->matrix.SetRow(3, position); }

Vector2 mmMirror::GetSize() const { return this->Size; }
void mmMirror::SetSize(const Vector2& size) { this->Size.Set(size); }

float mmMirror::GetFov() const { return this->Fov; }
void mmMirror::SetFov(float fov) { this->Fov = fov; }

float mmMirror::GetAspect() const { return this->Aspect; }
void mmMirror::SetAspect(float aspect) { this->Aspect = aspect; }

float mmMirror::GetNearClip() const { return this->NearClip; }
float mmMirror::GetFarClip() const { return this->FarClip; }

void mmMirror::SetNearClip(float clip) { this->NearClip = clip; }
void mmMirror::SetFarClip(float clip) { this->FarClip = clip; }

void mmMirror::BindLua(LuaState L)
{
	LuaBinding(L).beginExtendClass<mmMirror, asNode>("mmMirror")
		//functions
		.addProperty("Position", &GetPosition, &SetPosition)
		.addProperty("Size", &GetSize, &SetSize)
		.addProperty("FOV", &GetFov, &SetFov)
		.addProperty("NearClip", &GetNearClip, &SetNearClip)
		.addProperty("FarClip", &GetFarClip, &SetFarClip)
		.addProperty("Aspect", &GetAspect, &SetAspect)
		.endClass();
}