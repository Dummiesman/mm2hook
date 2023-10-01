#include <common.h>
#include "wpobject.h"

#include <mm2_lua.h>

using namespace MM2;

bool mmWaypointObject::radiusHitLua(Vector3 pos)
{
    return this->RadiusHit(pos) == TRUE;
}

MM2::mmWaypointObject::mmWaypointObject(Vector4 const& positionAndHeading, const char* modelName, int id, float radius, mmWaypointObjectType type, float yOffset)
{
    scoped_vtable x(this);
    hook::Thunk<0x43DE30>::Call<void>(this, &positionAndHeading, modelName, id, radius, type, yOffset);
}

MM2::mmWaypointObject::~mmWaypointObject()
{
    scoped_vtable x(this);
    hook::Thunk<0x43E010>::Call<void>(this);
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
        .addFunction("SetRadius", &SetRadius)
        .addFunction("SetPos", &SetPos)
        .addFunction("SetHeading", &SetHeading)
        .addFunction("SetHeadingType", &SetHeadingType)
        .addFunction("Move", &Move)
        .endClass();
}
