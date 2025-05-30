#pragma once
#include <modules\gfx\packet.h>
#include <modules\model\static2.h>

namespace MM2
{
    // Forward declarations
    class fxDamage3D;

    // External declarations


    // Class definitions
    class fxDamage3D
    {
    private:
        modStatic* m_BodyModel;
        modStatic* m_DamageModel;
        modStatic* m_DeformModel;
        modStatic* m_DeformDamageModel;
        Vector3* m_VertDeformPosition;
        float* m_VertDeformAmount;
        float m_DamageRadius;
        bool* m_PacketMatchesVertCount;
        bool m_EnableMM1Damage;
    public:
        ANGEL_ALLOCATOR

        fxDamage3D();

        virtual ~fxDamage3D();

        void Init(modStatic* bodyModel, modStatic* damageModel, bool mm1Damage);

        void SetNoDamage();

        void SetFullDamage();

        void Impact(Vector3& localImpactPosition, bool dynamicDamage);

        modStatic* GetDeformModel();

        modStatic* GetDeformDamageModel();
    };
}