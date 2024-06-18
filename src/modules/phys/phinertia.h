#pragma once
#include <mm2_common.h>

namespace MM2
{
    // Forward declarations
    class phInertialCS;

    // External declarations


    // Class definitions

    // Inertial coordinate system
    class phInertialCS {
    private:
        byte _buffer[0x1B4];
    protected:
        static hook::Field<0xC, float> _mass;
        static hook::Field<0x14, Vector3> _angInertia;
        static hook::Field<0x2C, float> _maxVelocity;
        static hook::Field<0x30, Vector3> _maxAngularVelocity;
        static hook::Field<0x54, Matrix34> _matrix;
        static hook::Field<0x9C, Vector3> _force;
        static hook::Field<0x84, Vector3> _velocity;
        static hook::Field<0x3C, Vector3> _momentum;
        static hook::Field<0x90, Vector3> _angularVelocity;
        static hook::Field<0x48, Vector3> _angularMomentum;
        static hook::Field<0xA8, Vector3> _torque;
        static hook::Field<0xD0, Vector3> _impulse;
        static hook::Field<0xDC, Vector3> _angImpulse;
    public:
        AGE_API phInertialCS();

        //props
        float GetMass() const;
        void SetMass(float mass);
        float GetMaxSpeed() const;
        void SetMaxSpeed(float speed);
        Vector3 GetMaxAngSpeed() const;
        void SetMaxAngSpeed(Vector3 const& speed);
        Vector3 GetPosition() const;
        Vector3 GetVelocity() const;
        void SetVelocity(Vector3 const &velocity);
        Vector3 GetMomentum() const;
        void SetMomentum(Vector3 const& momentum);
        Vector3 GetAngVelocity() const;
        void SetAngVelocity(Vector3 const& velocity);
        Vector3 GetAngMomentum() const;
        void SetAngMomentum(Vector3 const& momentum);
        Matrix34 GetMatrix() const;
        void SetPosition(Vector3 const& position);
        void SetMatrix(Matrix34 const& matrix);
        Vector3 GetForce() const;
        void AddForce(Vector3 const & force);
        void SetForce(Vector3 const & force);
        Vector3 GetTorque() const;
        void AddTorque(Vector3 const & torque);
        void SetTorque(Vector3 const & torque);
        Vector3 GetImpulse() const;
        void SetImpulse(Vector3 const& impulse);
        Vector3 GetAngImpulse() const;
        void SetAngImpulse(Vector3 const& impulse);
        Vector3 GetAngInertia() const;

        //members (not all here yet!)
        AGE_API void Zero();
        AGE_API void Reset();
        AGE_API void Freeze();
        AGE_API void ZeroForces();
        AGE_API void Init(float mass, float angInertiaX, float angInertiaY, float angInertiaZ);
        AGE_API void InitBoxMass(float mass, float inertiaBoxX, float inertiaBoxY, float inertiaBoxZ);
        AGE_API void Rotate(Vector3 const & angles);
        AGE_API void ClearInertialValues();
        AGE_API void CalcNetPush(Vector3 const & amount);
        AGE_API void CalcNetTurn(Vector3 const& amount);
        AGE_API void MoveICS();

        //lua
        static void BindLua(LuaState L);
    };

    ASSERT_SIZEOF(phInertialCS, 0x1B4);
}