#pragma once
#include "aiPedestrian.h"
#include "..\rgl.h"

using namespace MM2;

   
AGE_API void aiPedestrian::Update()                                   { hook::Thunk<0x54B9C0>::Call<void>(this); }

void MM2::aiPedestrian::DrawDebug() const
{
	vglBindTexture(nullptr);
	vglBegin(gfxDrawMode::DRAWMODE_LINELIST, 2);

	vglCurrentColor = mkfrgba(1.0f, 0.0f, 1.0f, 1.0f);

	Vector3 lineOffset = Vector3(0.0f, 0.3f, 0.0f);
	vglVertex3f(_matrix.get(this).GetRow(3) + lineOffset);
	vglVertex3f(_targetPoint.get(this) + lineOffset);

	vglEnd();

	vglDrawLabelf(_matrix.get(this).GetRow(3) + Vector3(0.0f, 4.0f, 0.0f), "%d", _id.get(this));
}

aiPedestrianInstance * aiPedestrian::GetInstance() const 
{
	return _instance.get(this);
}

aiPedAudio * aiPedestrian::GetAudio() const 
{
	return _audio.ptr(this);
}

aiPedestrian* MM2::aiPedestrian::GetNext() const
{
	return _next.get(this);
}

Matrix34* aiPedestrian::GetMatrix() const
{
	return _matrix.ptr(this);
}

void aiPedestrian::BindLua(LuaState L) {
	LuaBinding(L).beginClass<aiPedestrian>("aiPedestrian")
		.addPropertyReadOnly("Audio", &GetAudio)
		.addFunction("DrawDebug", &DrawDebug)
		.endClass();
}