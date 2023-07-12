#include "aiVehicle.h"
#include "aiMap.h"
#include  <modules\gfx\rstate.h>

namespace MM2
{
    /*
        aiVehicleInstance
    */

    // Properties
    aiVehicleSpline* aiVehicleInstance::GetSpline()
    {
        return _spline.get(this);
    }
        
    //overrides
    AGE_API Vector3 const& aiVehicleInstance::GetPosition()              { return hook::Thunk<0x553030>::Call<Vector3 const&>(this); };
    AGE_API Matrix34 const& aiVehicleInstance::GetMatrix(Matrix34* a1)   { return hook::Thunk<0x553020>::Call<Matrix34 const&>(this, a1); };
    AGE_API void aiVehicleInstance::SetMatrix(Matrix34 const & a1)       { hook::Thunk<0x553010>::Call<void>(this, a1); }
    AGE_API dgPhysEntity* aiVehicleInstance::GetEntity()                 { return hook::Thunk<0x52F50>::Call<dgPhysEntity*>(this); };
    AGE_API dgPhysEntity* aiVehicleInstance::AttachEntity()              { return hook::Thunk<0x552FBD>::Call<dgPhysEntity*>(this); };
    AGE_API void aiVehicleInstance::Detach()                             { hook::Thunk<0x552F80>::Call<void>(this); }
    AGE_API void aiVehicleInstance::DrawShadow()                         { hook::Thunk<0x552CC0>::Call<void>(this); }
    AGE_API void aiVehicleInstance::DrawShadowMap()                      { hook::Thunk<0x552F30>::Call<void>(this); }
        
    AGE_API void aiVehicleInstance::Draw(int a1)                                 { hook::Thunk<0x552160>::Call<void>(this, a1); }

    AGE_API void aiVehicleInstance::DrawReflected(float a1)              { hook::Thunk<0x552CB0>::Call<void>(this, a1); }
    AGE_API unsigned int aiVehicleInstance::SizeOf()                     { return hook::Thunk<0x553060>::Call<unsigned int>(this); };
    AGE_API phBound* aiVehicleInstance::GetBound(int a1)                 { return hook::Thunk<0x552F40>::Call<phBound*>(this, a1); };
        
    //members
    aiVehicleData* aiVehicleInstance::GetData()                           { return hook::Thunk<0x553F80>::Call<aiVehicleData*>(this); }
    AGE_API void aiVehicleInstance::DrawPart(modStatic* a1, const Matrix34* a2, modShader* a3, int a4)
                                                                          { hook::Thunk<0x552870>::Call<void>(this, a1, a2, a3, a4); }

    //lua
    void aiVehicleInstance::BindLua(LuaState L) {
        LuaBinding(L).beginExtendClass<aiVehicleInstance, lvlInstance>("aiVehicleInstance")
            //members
            .addFunction("GetData", &GetData)
            .endClass();
    }

    /*
        aiObstacle
    */
    BOOL aiObstacle::InAccident(void)                                    { return hook::Thunk<0x53F5D0>::Call<BOOL>(this); }
    float aiObstacle::BreakThreshold(void)                               { return hook::Thunk<0x53F5E0>::Call<float>(this); }
    BOOL aiObstacle::Drivable(void)                                      { return hook::Thunk<0x53F5F0>::Call<BOOL>(this); }

    //lua
    void aiObstacle::BindLua(LuaState L) {
        LuaBinding(L).beginClass<aiObstacle>("aiObstacle")
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
            .endClass();
    }
}