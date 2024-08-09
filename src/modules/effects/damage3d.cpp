#include "damage3d.h"

namespace MM2
{
    fxDamage3D::fxDamage3D()
    {
        m_DamageRadius = 0.25f;
    }

    fxDamage3D::~fxDamage3D()
    {
        delete[] m_VertDeformAmount;
        delete[] m_PacketMatchesVertCount;

        if (m_EnableMM1Damage)
            delete[] m_VertDeformPosition;
    }

    void fxDamage3D::Init(modStatic* bodyModel, modStatic* damageModel, bool mm1Damage)
    {
        m_BodyModel = bodyModel;
        m_DamageModel = damageModel;
        m_DeformModel = bodyModel->Clone();
        m_DeformDamageModel = bodyModel->Clone();

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

        m_EnableMM1Damage = mm1Damage;

        if (mm1Damage)
        {
            m_VertDeformPosition = new Vector3[bodyVertCount];

            int currentVertex = 0;
            for (int i = 0; i < bodyModel->GetPacketCount(); i++)
            {
                auto bodyPacket = bodyModel->GetPacket(i);
                while (bodyPacket)
                {
                    bodyPacket->DoLock();

                    for (unsigned int j = 0; j < bodyPacket->GetAdjunctCount(); j++)
                    {
                        bodyPacket->GetPosition(m_VertDeformPosition[currentVertex], j);
                        currentVertex++;
                    }

                    bodyPacket->DoUnlock();
                    bodyPacket = bodyPacket->GetNext();
                }
            }
        }

        SetNoDamage();
    }

    void fxDamage3D::SetNoDamage()
    {
        int currentVertex = 0;

        for (int i = 0; i < m_DeformModel->GetPacketCount(); i++)
        {
            if (i >= m_BodyModel->GetPacketCount()) break;

            auto deformDamagePacket = m_DeformDamageModel->GetPacket(i);
            auto deformPacket = m_DeformModel->GetPacket(i);
            auto bodyPacket = m_BodyModel->GetPacket(i);

            while (bodyPacket && deformPacket && deformDamagePacket)
            {
                bodyPacket->DoLock(); deformPacket->DoLock(); deformDamagePacket->DoLock();

                for (unsigned int j = 0; j < bodyPacket->GetAdjunctCount() && j < deformPacket->GetAdjunctCount(); j++)
                {
                    Vector3 position = Vector3::ORIGIN;
                    bodyPacket->GetPosition(position, j);
                    deformPacket->SetPosition(position, j);
                    deformDamagePacket->SetPosition(position, j);
                    m_VertDeformAmount[currentVertex] = 0.0f;
                    currentVertex++;
                }

                if (m_EnableMM1Damage)
                {
                    for (unsigned int j = 0; j < deformPacket->GetTriangleCount() / 3; j++)
                    {
                        int tri[3];
                        bodyPacket->GetTri(tri, j);
                        deformPacket->SetTri(tri[0], tri[1], tri[2], j);
                        deformDamagePacket->SetTri(0, 0, 0, j);
                    }
                }

                bodyPacket->DoUnlock(); deformPacket->DoUnlock(); deformDamagePacket->DoUnlock();

                bodyPacket = bodyPacket->GetNext();
                deformPacket = deformPacket->GetNext();
                deformDamagePacket = deformDamagePacket->GetNext();
            }
        }
    }

    void fxDamage3D::SetFullDamage()
    {
        int currentVertex = 0;

        for (int i = 0; i < m_DeformModel->GetPacketCount(); i++)
        {
            if (i >= m_DamageModel->GetPacketCount()) break;

            auto deformDamagePacket = m_DeformDamageModel->GetPacket(i);
            auto deformPacket = m_DeformModel->GetPacket(i);
            auto damagePacket = m_DamageModel->GetPacket(i);

            while (damagePacket && deformPacket && deformDamagePacket)
            {
                damagePacket->DoLock(); deformPacket->DoLock(); deformDamagePacket->DoLock();

                for (unsigned int j = 0; j < damagePacket->GetAdjunctCount() && j < deformPacket->GetAdjunctCount(); j++)
                {
                    Vector3 position = Vector3::ORIGIN;
                    damagePacket->GetPosition(position, j);
                    deformPacket->SetPosition(position, j);
                    deformDamagePacket->SetPosition(position, j);
                    m_VertDeformAmount[currentVertex] = 1.0f;
                    currentVertex++;
                }

                if (m_EnableMM1Damage)
                {
                    for (unsigned int j = 0; j < deformDamagePacket->GetTriangleCount() / 3; j++)
                    {
                        int tri[3];
                        damagePacket->GetTri(tri, j);
                        deformDamagePacket->SetTri(tri[0], tri[1], tri[2], j);
                        deformPacket->SetTri(0, 0, 0, j);
                    }
                }

                damagePacket->DoUnlock(); deformPacket->DoUnlock(); deformDamagePacket->DoUnlock();

                damagePacket = damagePacket->GetNext();
                deformPacket = deformPacket->GetNext();
                deformDamagePacket = deformDamagePacket->GetNext();
            }
        }
    }

    void fxDamage3D::Impact(Vector3& localImpactPosition, bool dynamicDamage)
    {
        int currentVertex = 0;
        int currentVertex2 = 0;
        int currentPacket = 0;

        for (int i = 0; i < m_DeformModel->GetPacketCount(); i++)
        {
            if (i >= m_DamageModel->GetPacketCount()) break;

            auto deformDamagePacket = m_DeformDamageModel->GetPacket(i);
            auto deformPacket = m_DeformModel->GetPacket(i);
            auto damagePacket = m_DamageModel->GetPacket(i);
            auto bodyPacket = m_DamageModel->GetPacket(i);

            while (damagePacket && deformPacket && bodyPacket && deformDamagePacket)
            {
                if (m_PacketMatchesVertCount[currentPacket]) {
                    damagePacket->DoLock(); deformPacket->DoLock(); bodyPacket->DoLock(); deformDamagePacket->DoLock();

                    for (unsigned int j = 0; j < damagePacket->GetAdjunctCount() && j < deformPacket->GetAdjunctCount(); j++)
                    {
                        Vector3 position = Vector3::ORIGIN;
                        bodyPacket->GetPosition(position, j);

                        float squareDistance = (localImpactPosition - position).Mag2();
                        if (squareDistance < m_DamageRadius)
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
                            dynamicDamage ?
                                deformedPosition.Lerp(0.5f, bodyModelPosition, bodyModelPosition - localImpactPosition * 0.1f) :
                                deformedPosition.Lerp(deformAmount, bodyModelPosition, fullDamagePosition);
                            deformPacket->SetPosition(deformedPosition, j);
                            deformDamagePacket->SetPosition(deformedPosition, j);

                            currentVertex++;
                        }
                    }

                    if (m_EnableMM1Damage)
                    {
                        for (unsigned int j = 0; j < deformDamagePacket->GetTriangleCount() / 3; j++)
                        {
                            int tri[3];
                            bodyPacket->GetTri(tri, j);

                            float dist0 = (m_VertDeformPosition[currentVertex2 + tri[0]] - localImpactPosition).Mag2();
                            float dist1 = (m_VertDeformPosition[currentVertex2 + tri[1]] - localImpactPosition).Mag2();
                            float dist2 = (m_VertDeformPosition[currentVertex2 + tri[2]] - localImpactPosition).Mag2();

                            if (dist0 < m_DamageRadius || dist1 < m_DamageRadius || dist2 < m_DamageRadius)
                            {
                                deformDamagePacket->SetTri(tri[0], tri[1], tri[2], j);
                                deformPacket->SetTri(0, 0, 0, j);
                            }
                        }
                    }

                    damagePacket->DoUnlock(); deformPacket->DoUnlock(); bodyPacket->DoUnlock(); deformDamagePacket->DoUnlock();
                }
                currentVertex2 += deformDamagePacket->GetAdjunctCount();

                damagePacket = damagePacket->GetNext();
                deformPacket = deformPacket->GetNext();
                bodyPacket = bodyPacket->GetNext();
                deformDamagePacket = deformDamagePacket->GetNext();
                currentPacket++;
            }
        }
    }

    modStatic* fxDamage3D::GetDeformModel()
    {
        return m_DeformModel;
    }

    modStatic* fxDamage3D::GetDeformDamageModel()
    {
        return m_DeformDamageModel;
    }
}