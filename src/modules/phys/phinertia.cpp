#pragma once
#include "phinertia.h"

namespace MM2
{
    AGE_API phInertialCS::phInertialCS() {
        scoped_vtable x(this);
        hook::Thunk<0x475DE0>::Call<void>(this);
    }

    //props
    float phInertialCS::GetMass() const {
        return _mass.get(this);
    }

    void phInertialCS::SetMass(float mass) {
        _mass.set(this, mass);
    }

    float phInertialCS::GetMaxSpeed() const {
        return _maxVelocity.get(this);
    }

    void phInertialCS::SetMaxSpeed(float speed) {
        _maxVelocity.set(this, speed);
    }

    Vector3 phInertialCS::GetMaxAngSpeed() const {
        return _maxAngularVelocity.get(this);
    }

    void phInertialCS::SetMaxAngSpeed(Vector3 const& speed) {
        _maxAngularVelocity.set(this, speed);
    }

    Vector3 phInertialCS::GetPosition() const {
        return this->GetMatrix().GetRow(3);
    }

    Vector3 phInertialCS::GetVelocity() const {
        return _velocity.get(this);
    }

    void phInertialCS::SetVelocity(Vector3 const & velocity) {
        _velocity.set(this, velocity);
        _momentum.set(this, velocity * this->GetMass());
    }

    Vector3 phInertialCS::GetMomentum() const {
        return _momentum.get(this);
    }

    void phInertialCS::SetMomentum(Vector3 const& momentum) {
        _velocity.set(this, momentum / this->GetMass());
        _momentum.set(this, momentum);
    }

    Vector3 phInertialCS::GetAngVelocity() const {
        return _angularVelocity.get(this);
    }

    void phInertialCS::SetAngVelocity(Vector3 const& velocity) {
        _angularVelocity.set(this, velocity);
        _angularMomentum.set(this, velocity * this->GetMass());
    }

    Vector3 phInertialCS::GetAngMomentum() const {
        return _angularMomentum.get(this);
    }

    void phInertialCS::SetAngMomentum(Vector3 const& momentum) {
        _angularVelocity.set(this, momentum / this->GetMass());
        _angularMomentum.set(this, momentum);
    }

    Matrix34 phInertialCS::GetMatrix() const {
        return _matrix.get(this);
    }

    void phInertialCS::SetPosition(Vector3 const & position) {
        auto mtx = GetMatrix();
        mtx.SetRow(3, position);
        SetMatrix(mtx);
    }

    void phInertialCS::SetMatrix(Matrix34 const & matrix) {
        _matrix.set(this, matrix);
    }

    Vector3 phInertialCS::GetForce() const {
        return _force.get(this);
    }

    void phInertialCS::AddForce(Vector3 const & force) {
        auto current = GetForce();
        SetForce(current + force);
    }

    void phInertialCS::SetForce(Vector3 const & force) {
        _force.set(this, force);
    }

    Vector3 phInertialCS::GetTorque() const {
        return _torque.get(this);
    }

    void phInertialCS::AddTorque(Vector3 const & torque) {
        auto current = GetTorque();
        SetTorque(current + torque);
    }

    void phInertialCS::SetTorque(Vector3 const & torque) {
        _torque.set(this, torque);
    }

    Vector3 phInertialCS::GetImpulse() const {
        return _impulse.get(this);
    }

    void phInertialCS::SetImpulse(Vector3 const& impulse) {
        _impulse.set(this, impulse);
    }

    Vector3 phInertialCS::GetAngImpulse() const {
        return _angImpulse.get(this);
    }

    void phInertialCS::SetAngImpulse(Vector3 const& impulse) {
        _angImpulse.set(this, impulse);
    }

    Vector3 phInertialCS::GetAngInertia() const
    {
        return _angInertia.get(this);
    }

    //members (not all here yet!)
    AGE_API void phInertialCS::Zero()                               { hook::Thunk<0x475DED>::Call<void>(this); }
    AGE_API void phInertialCS::Reset()                              { hook::Thunk<0x475E10>::Call<void>(this); }
    AGE_API void phInertialCS::Freeze()                             { hook::Thunk<0x475E20>::Call<void>(this); }
    AGE_API void phInertialCS::ZeroForces()                         { hook::Thunk<0x475E60>::Call<void>(this); }
    AGE_API void phInertialCS::Init(float mass, float angInertiaX, float angInertiaY, float angInertiaZ)
                                                                    { hook::Thunk<0x475FC0>::Call<void>(this, mass, angInertiaX, angInertiaY, angInertiaZ); }
    AGE_API void phInertialCS::InitBoxMass(float mass, float inertiaBoxX, float inertiaBoxY, float inertiaBoxZ)
                                                                    { hook::Thunk<0x4760D0>::Call<void>(this, mass, inertiaBoxX, inertiaBoxY, inertiaBoxZ); }
    AGE_API void phInertialCS::Rotate(Vector3 const & angles)       { hook::Thunk<0x476A00>::Call<void>(this, &angles); }
    AGE_API void phInertialCS::CalcNetPush(Vector3 const & amount)  { hook::Thunk<0x478700>::Call<void>(this, &amount); }
    AGE_API void phInertialCS::CalcNetTurn(Vector3 const& amount)   { hook::Thunk<0x478820>::Call<void>(this, &amount); }
    AGE_API void phInertialCS::ClearInertialValues()                { hook::Thunk<0x476B20>::Call<void>(this); }
        
    AGE_API void phInertialCS::MoveICS()                            { hook::Thunk<0x478680>::Call<void>(this); }

    //lua
    void phInertialCS::BindLua(LuaState L) {
        LuaBinding(L).beginClass<phInertialCS>("phInertialCS")
            .addFunction("Zero", &Zero)
            .addFunction("Reset", &Reset)
            .addFunction("Freeze", &Freeze)
            .addFunction("ZeroForces", &ZeroForces)
            .addFunction("Init", &Init)
            .addFunction("InitBoxMass", &InitBoxMass)
            .addFunction("Rotate", &Rotate)
            .addFunction("ClearInertialValues", &ClearInertialValues)
            .addFunction("CalcNetPush", &CalcNetPush)
            .addFunction("CalcNetTurn", &CalcNetTurn)
            .addFunction("MoveICS", &MoveICS)

            .addFunction("AddForce", &AddForce)
            .addFunction("AddTorque", &AddTorque)

            .addProperty("Velocity", &GetVelocity, &SetVelocity)
            .addProperty("Momentum", &GetMomentum, &SetMomentum)
            .addProperty("Impulse", &GetImpulse, &SetImpulse)
            .addProperty("AngVelocity", &GetAngVelocity, &SetAngVelocity)
            .addProperty("AngMomentum", &GetAngMomentum, &SetAngMomentum)
            .addProperty("AngImpulse", &GetAngImpulse, &SetAngImpulse)
            .addPropertyReadOnly("AngInertia", &GetAngInertia)

            .addProperty("MaxSpeed", &GetMaxSpeed, &SetMaxSpeed)
            .addProperty("MaxAngSpeed", &GetMaxAngSpeed, &SetMaxAngSpeed)

            .addProperty("Mass", &GetMass, &SetMass)
            .addProperty("Force", &GetForce, &SetForce)
            .addProperty("Torque", &GetTorque, &SetTorque)

            .addFunction("GetPosition", &GetPosition)
            .addFunction("SetPosition", &SetPosition)
            .addFunction("GetMatrix", &GetMatrix)
            .addFunction("SetMatrix", &SetMatrix)
        .endClass();
    }
}