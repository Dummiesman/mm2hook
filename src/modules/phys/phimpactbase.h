#pragma once
#include <mm2_common.h>

namespace MM2
{
    // Forward declarations
    struct ImpactCBData;
    class phImpactBase;
    
    // External declarations
    extern class phColliderBase;

    // Class definitions
    struct ImpactCBData
    {
        Vector3* InvImpulse; // I think?
        phImpactBase* Impact;

        static void BindLua(LuaState L) {
            LuaBinding(L).beginClass<ImpactCBData>("ImpactCBData")
                .addVariableRef("InvImpulse", &ImpactCBData::InvImpulse, false)
                .addVariableRef("Impact", &ImpactCBData::Impact, false)
                .endClass();
        }
    };

    class phImpactBase
    {
    private:
        int dword_00;
        int dword_04;
        int dword_08;
        int ComponentA;
        int ComponentB;
        Vector3 Position;
        Vector3 Normal;
        float dword_2c;
        float Penetration;
        float Friction;
        float Elasticity;
        phColliderBase* ColliderA;
        phColliderBase* ColliderB;
    public:
        phColliderBase* GetColliderA() const { return ColliderA; }
        phColliderBase* GetColliderB() const { return ColliderB; }

        int GetComponentA() const { return ComponentA; }
        int GetComponentB() const { return ComponentB; }

        float GetFriction() const { return Friction; }
        float GetElasticity() const { return Elasticity; }
        float GetPenetration() const { return Penetration; }

        Vector3 GetPosition() const { return Position; }
        Vector3 GetNormal() const { return Normal; }

        static void BindLua(LuaState L) {
            LuaBinding(L).beginClass<phImpactBase>("phImpactBase")
                .addPropertyReadOnly("ColliderA", &GetColliderA)
                .addPropertyReadOnly("ColliderB", &GetColliderB)
                .addPropertyReadOnly("ComponentA", &GetComponentA)
                .addPropertyReadOnly("ComponentB", &GetComponentB)
                .addPropertyReadOnly("Friction", &GetFriction)
                .addPropertyReadOnly("Elasticity", &GetElasticity)
                .addPropertyReadOnly("Position", &GetPosition)
                .addPropertyReadOnly("Normal", &GetNormal)
                .addPropertyReadOnly("Penetration", &GetPenetration)
                .endClass();
        }
    };

}