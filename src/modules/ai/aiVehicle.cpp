#include "aiVehicle.h"
#include "aiMap.h"
#include  <modules\gfx\rstate.h>

namespace MM2
{
    /*
        aiObstacle
    */
    BOOL aiObstacle::InAccident(void)                                    { return hook::Thunk<0x53F5D0>::Call<BOOL>(this); }
    float aiObstacle::BreakThreshold(void)                               { return hook::Thunk<0x53F5E0>::Call<float>(this); }
    BOOL aiObstacle::Drivable(void)                                      { return hook::Thunk<0x53F5F0>::Call<BOOL>(this); }

    //lua
    void aiObstacle::BindLua(LuaState L) {
        LuaBinding(L).beginClass<aiObstacle>("aiObstacle")
            .addPropertyReadOnly("BreakThreshold", &BreakThreshold)
            .addPropertyReadOnly("Speed", &Speed)
            .addPropertyReadOnly("InAccident", &luaInAccident)
            .addPropertyReadOnly("Drivable", &luaDrivable)
            .addFunction("IsBlockingTarget", &IsBlockingTarget)
            .addFunction("GetPosition", &luaPosition)
            .endClass();
    }

    /*
        aiVehicle
    */
    void aiVehicle::PreAvoid(const Vector3& a1, const Vector3& a2, float a3, Vector3& a4, Vector3& a5)
        FORWARD_THUNK;

    int aiVehicle::GetId() const
    {
        return this->Id;
    }

    float aiVehicle::IsBlockingTarget(const Vector3& a1, const Vector3& a2, float a3, float a4)
    {  return hook::Thunk<0x556850>::Call<float>(this, &a1, &a2, a3, a4); }

    void aiVehicle::Update(void) { return hook::Thunk<0x556230>::Call<void>(this); };
    void aiVehicle::Reset(void) { return hook::Thunk<0x556210>::Call<void>(this); };
    void aiVehicle::ReplayDebug(void) { return hook::Thunk<0x556D00>::Call<void>(this); }

    //lua
    void aiVehicle::BindLua(LuaState L) {
        LuaBinding(L).beginExtendClass<aiVehicle, aiObstacle>("aiVehicle")
            .addFunction("GetMatrix", &GetMatrix)
            .addFunction("DrawId", &DrawId)
            .addPropertyReadOnly("ID", &GetId)
            .addPropertyReadOnly("Type", &Type)
            .addPropertyReadOnly("FrontBumperDistance", &FrontBumperDistance)
            .addPropertyReadOnly("BackBumperDistance", &BackBumperDistance)
            .addPropertyReadOnly("BackBumperDistance", &BackBumperDistance)
            .addPropertyReadOnly("LSideDistance", &LSideDistance)
            .addPropertyReadOnly("RSideDistance", &RSideDistance)
            .addPropertyReadOnly("CurrentLane", &CurrentLane)
            .addPropertyReadOnly("CurrentRoadId", &CurrentRoadId)
            .endClass();
    }
}