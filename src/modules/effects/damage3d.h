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
        float* m_VertDeformAmount;
        float m_DamageRadius;
        bool* m_PacketMatchesVertCount;
    public:
        ANGEL_ALLOCATOR

        fxDamage3D()
        {
            m_DamageRadius = 0.9f;
        }

        virtual ~fxDamage3D()
        {
            delete[] m_VertDeformAmount;
            delete[] m_PacketMatchesVertCount;
        }

        void Init(modStatic* bodyModel, modStatic* damageModel)
        {
            m_BodyModel = bodyModel;
            m_DamageModel = damageModel;
            m_DeformModel = bodyModel->Clone();

            int bodyPacketCount = 0;
            int bodyVertCount = 0;
            for (int i = 0; i < bodyModel->GetPacketCount(); i++)
            {
                auto packet = bodyModel->GetPacket(i);
                while (packet) 
                {
                    bodyVertCount += packet->GetAdjunctCount();
                    bodyPacketCount++;
                    packet = packet->GetNext();
                }
            }

            m_VertDeformAmount = new float[bodyVertCount];
            m_PacketMatchesVertCount = new bool[bodyPacketCount];

            int currentPacket = 0;
            for (int i = 0; i < bodyModel->GetPacketCount(); i++)
            {
                if (i >= damageModel->GetPacketCount()) break;

                auto bodyPacket = bodyModel->GetPacket(i);
                auto damagePacket = damageModel->GetPacket(i);

                while (bodyPacket && damagePacket)
                {
                    m_PacketMatchesVertCount[currentPacket] = (bodyPacket->GetAdjunctCount() == damagePacket->GetAdjunctCount());
                    bodyPacket = bodyPacket->GetNext();
                    damagePacket = damagePacket->GetNext();
                    currentPacket++;
                }
            }

            SetNoDamage();
        }

        void SetNoDamage()
        {
            int currentVertex = 0;

            for (int i = 0; i < m_DeformModel->GetPacketCount(); i++)
            {
                if (i >= m_BodyModel->GetPacketCount()) break;

                auto deformPacket = m_DeformModel->GetPacket(i);
                auto bodyPacket = m_BodyModel->GetPacket(i);

                while (bodyPacket && deformPacket)
                {
                    bodyPacket->DoLock(); deformPacket->DoLock();
                    
                    for (unsigned int j = 0; j < bodyPacket->GetAdjunctCount() && j < deformPacket->GetAdjunctCount(); j++)
                    {
                        Vector3 position = Vector3::ORIGIN;
                        bodyPacket->GetPosition(position, j);
                        deformPacket->SetPosition(position, j);
                        m_VertDeformAmount[currentVertex] = 0.0f;
                        currentVertex++;
                    }

                    bodyPacket->DoUnlock(); deformPacket->DoUnlock();
                    bodyPacket = bodyPacket->GetNext();
                    deformPacket = deformPacket->GetNext();
                }
            }
        }

        void SetFullDamage()
        {
            int currentVertex = 0;

            for (int i = 0; i < m_DeformModel->GetPacketCount(); i++)
            {
                if (i >= m_DamageModel->GetPacketCount()) break;

                auto deformPacket = m_DeformModel->GetPacket(i);
                auto damagePacket = m_DamageModel->GetPacket(i);

                while (damagePacket && deformPacket)
                {
                    damagePacket->DoLock(); deformPacket->DoLock();

                    for (unsigned int j = 0; j < damagePacket->GetAdjunctCount() && j < deformPacket->GetAdjunctCount(); j++)
                    {
                        Vector3 position = Vector3::ORIGIN;
                        damagePacket->GetPosition(position, j);
                        deformPacket->SetPosition(position, j);
                        m_VertDeformAmount[currentVertex] = 1.0f;
                        currentVertex++;
                    }

                    damagePacket->DoUnlock(); deformPacket->DoUnlock();
                    damagePacket = damagePacket->GetNext();
                    deformPacket = deformPacket->GetNext();
                }
            }
        }

        void Impact(Vector3 localImpactPosition)
        {
            int currentVertex = 0;
            int currentPacket = 0;

            for (int i = 0; i < m_DeformModel->GetPacketCount(); i++)
            {
                if (i >= m_DamageModel->GetPacketCount()) break;

                auto deformPacket = m_DeformModel->GetPacket(i);
                auto damagePacket = m_DamageModel->GetPacket(i);
                auto bodyPacket = m_DamageModel->GetPacket(i);

                while (damagePacket && deformPacket && bodyPacket)
                {
                    if (m_PacketMatchesVertCount[currentPacket]) {
                        damagePacket->DoLock(); deformPacket->DoLock(); bodyPacket->DoLock();

                        for (unsigned int j = 0; j < damagePacket->GetAdjunctCount() && j < deformPacket->GetAdjunctCount(); j++)
                        {
                            Vector3 position = Vector3::ORIGIN;
                            bodyPacket->GetPosition(position, j);
                            
                            float squareDistance = (localImpactPosition - position).Mag2();
                            if (squareDistance <= m_DamageRadius)
                            {
                                Vector3 fullDamagePosition = Vector3::ORIGIN;
                                Vector3 bodyModelPosition = Vector3::ORIGIN;

                                damagePacket->GetPosition(fullDamagePosition, j);
                                bodyPacket->GetPosition(bodyModelPosition, j);

                                float deformAmount = 1.0f - (squareDistance / m_DamageRadius);
                                if (deformAmount < m_VertDeformAmount[currentVertex])
                                {
                                    deformAmount = m_VertDeformAmount[currentVertex];
                                }
                                m_VertDeformAmount[currentVertex] = deformAmount;

                                Vector3 deformedPosition = Vector3::ORIGIN;
                                deformedPosition.Lerp(deformAmount, bodyModelPosition, fullDamagePosition);
                                deformPacket->SetPosition(deformedPosition, j);

                                currentVertex++;
                            }
                        }

                        damagePacket->DoUnlock(); deformPacket->DoUnlock(); bodyPacket->DoUnlock();
                    }

                    damagePacket = damagePacket->GetNext();
                    deformPacket = deformPacket->GetNext();
                    bodyPacket = bodyPacket->GetNext();
                    currentPacket++;
                }
            }
        }

        modStatic* GetDeformModel()
        {
            return m_DeformModel;
        }
    };
}