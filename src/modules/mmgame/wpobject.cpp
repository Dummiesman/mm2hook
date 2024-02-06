#include <common.h>
#include "wpobject.h"

#include <mm2_lua.h>

using namespace MM2;

bool mmWaypointObject::radiusHitLua(Vector3 pos)
{
    return this->RadiusHit(pos) == TRUE;
}

bool mmWaypointObject::planeHitLua(Matrix34 vehMatrix, Vector2 linePtA, Vector2 linePtB, Vector3 vehSize)
{
    return this->PlaneHit(vehMatrix, linePtA, linePtB, vehSize) == TRUE;
}

void mmWaypointObject::setHitFlagLua(bool flag)
{
    this->SetHitFlag(flag ? TRUE : FALSE);
}

mmWaypointObject::mmWaypointObject(Vector4 const& positionAndHeading, const char* modelName, int id, float radius, mmWaypointObjectType type, float yOffset)
{
    scoped_vtable x(this);
    hook::Thunk<0x43DE30>::Call<void>(this, &positionAndHeading, modelName, id, radius, type, yOffset);
}

mmWaypointObject::~mmWaypointObject()
{
    asNode::~asNode();
}

AGE_API void mmWaypointObject::Update()
{
    hook::Thunk<0x43E060>::Call<void>(this);
}

AGE_API void mmWaypointObject::Reset()
{
    hook::Thunk<0x43E020>::Call<void>(this);
}

AGE_API void MM2::mmWaypointObject::Activate()
{
    hook::Thunk<0x43E0C0>::Call<void>(this);
}

AGE_API void MM2::mmWaypointObject::Deactivate()
{
    hook::Thunk<0x43E0D0>::Call<void>(this);
}

AGE_API BOOL mmWaypointObject::RadiusHit(Vector3 pos)
{
    return hook::Thunk<0x43E3B0>::Call<BOOL>(this, pos);
}

AGE_API BOOL MM2::mmWaypointObject::PlaneHit(Matrix34 vehMatrix, Vector2 linePtA, Vector2 linePtB, Vector3 vehSize)
{
    return hook::Thunk<0x43E210>::Call<BOOL>(this, vehMatrix, linePtA, linePtB, vehSize);
}

AGE_API void MM2::mmWaypointObject::SetHitFlag(BOOL flag)
{
    hook::Thunk<0x43E6D0>::Call<void>(this, flag);
}

AGE_API void mmWaypointObject::SetRadius(float radius)
{
    hook::Thunk<0x43E170>::Call<void>(this, radius);
}

AGE_API void mmWaypointObject::SetPos(Vector3 pos)
{
    hook::Thunk<0x43E140>::Call<void>(this, pos);
}

AGE_API void MM2::mmWaypointObject::SetHeading(float heading)
{
    hook::Thunk<0x43E0E0>::Call<void>(this, heading);
}

AGE_API void MM2::mmWaypointObject::SetHeadingType(int type)
{
    hook::Thunk<0x43E160>::Call<void>(this, type);
}

AGE_API void mmWaypointObject::Move()
{
    hook::Thunk<0x43E5F0>::Call<void>(this);
}

AGE_API void MM2::mmWaypointObject::CalculateGatePoints()
{
    hook::Thunk<0x43E190>::Call<void>(this);
}

void MM2::mmWaypointObject::Clear()
{
    m_Cleared = TRUE;
}

bool mmWaypointObject::Cleared() const
{
    return this->m_Cleared == TRUE;
}

int mmWaypointObject::GetHitIDMask() const
{
    return HitID;
}

void mmWaypointObject::BindLua(LuaState L)
{
    LuaBinding(L).beginExtendClass<mmWaypointObject, asNode>("mmWaypointObject")
        .addFactory([](Vector4 positionAndHeading, LPCSTR modelName, int id, 
                       float radius, mmWaypointObjectType type, float yOffset) {
            auto object = new mmWaypointObject(positionAndHeading, modelName, id, radius, type, yOffset);
            MM2Lua::MarkForCleanupOnShutdown(object);
            return object;
        })
        .addFunction("Activate", &Activate)
        .addFunction("Deactivate", &Deactivate)
        .addFunction("RadiusHit", &radiusHitLua)
        .addFunction("PlaneHit", &planeHitLua)
        .addFunction("SetRadius", &SetRadius)
        .addFunction("SetPos", &SetPos)
        .addFunction("SetPosition", &SetPos)
        .addFunction("SetHeading", &SetHeading)
        .addFunction("SetHeadingType", &SetHeadingType)
        .addFunction("SetHitFlag", &setHitFlagLua)
        .addFunction("Move", &Move)
        .addFunction("CalculateGatePoints", &CalculateGatePoints)
        .endClass();
}
