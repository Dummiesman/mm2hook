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

    float phInertialCS::GetMaxVelocity() const {
        return _maxVelocity.get(this);
    }

    void phInertialCS::SetMaxVelocity(float velocity) {
        _maxVelocity.set(this, velocity);
    }

    Vector3 phInertialCS::GetPosition() const {
        return this->GetMatrix().GetRow(3);
    }

    Vector3 phInertialCS::GetVelocity() const {
        return _velocity.get(this);
    }

    void phInertialCS::SetVelocity(Vector3 velocity) {
        _velocity.set(this, velocity);
        _scaledVelocity.set(this, velocity * this->GetMass());
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

    void phInertialCS::AddForce(Vector3 force) {
        auto current = GetForce();
        SetForce(current + force);
    }

    void phInertialCS::SetForce(Vector3 force) {
        _force.set(this, force);
    }

    Vector3 phInertialCS::GetTorque() const {
        return _torque.get(this);
    }

    void phInertialCS::AddTorque(Vector3 torque) {
        auto current = GetTorque();
        SetTorque(current + torque);
    }

    void phInertialCS::SetTorque(Vector3 torque) {
        _torque.set(this, torque);
    }

    //members (not all here yet!)
    AGE_API void phInertialCS::Zero()                     { hook::Thunk<0x475DED>::Call<void>(this); }
    AGE_API void phInertialCS::Reset()                    { hook::Thunk<0x475E10>::Call<void>(this); }
    AGE_API void phInertialCS::Freeze()                   { hook::Thunk<0x475E20>::Call<void>(this); }
    AGE_API void phInertialCS::ZeroForces()               { hook::Thunk<0x475E60>::Call<void>(this); }
    AGE_API void phInertialCS::Init(float mass, float inertiaTensorX, float inertiaTensorY, float inertiaTensorZ)
                                            { hook::Thunk<0x475FC0>::Call<void>(this, mass, inertiaTensorX, inertiaTensorY, inertiaTensorZ); }
    AGE_API void phInertialCS::InitBoxMass(float mass, float inertiaBoxX, float inertiaBoxY, float inertiaBoxZ)
                                            { hook::Thunk<0x4760D0>::Call<void>(this, mass, inertiaBoxX, inertiaBoxY, inertiaBoxZ); }
    AGE_API void phInertialCS::Rotate(Vector3 *angles)    { hook::Thunk<0x476A00>::Call<void>(this, angles); }
    AGE_API void phInertialCS::ClearInertialValues()      { hook::Thunk<0x476B20>::Call<void>(this); }
        
    AGE_API void phInertialCS::MoveICS()                  { hook::Thunk<0x478680>::Call<void>(this); }

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
            .addFunction("MoveICS", &MoveICS)

            .addFunction("AddForce", &AddForce)
            .addFunction("AddTorque", &AddTorque)

            .addProperty("Mass", &GetMass, &SetMass)
            .addProperty("Force", &GetForce, &SetForce)
            .addProperty("Torque", &GetTorque, &SetTorque)
            .addProperty("Velocity", &GetVelocity, &SetVelocity)
            .addProperty("Position", &GetPosition, &SetPosition)
            .addProperty("Matrix", &GetMatrix, &SetMatrix)
        .endClass();
    }
}