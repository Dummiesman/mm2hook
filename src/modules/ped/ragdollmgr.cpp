#include "ragdollmgr.h"
#include <modules\ped\pedactive.h>
#include <modules\ped\pedinst.h>

using namespace MM2;


AGE_API pedRagdollMgr::pedRagdollMgr(void)
{
    if (pedRagdollMgr::Instance.get())
        Quitf("Can only have one pedRagdollMgr");
    pedRagdollMgr::Instance.set(this);
}

AGE_API pedRagdollMgr::~pedRagdollMgr(void)
{
    delete[] m_ActiveData;
    delete[] m_Actives;
    delete m_BoundBox;
    pedRagdollMgr::Instance.set(nullptr);
}

phBoundBox* pedRagdollMgr::GetBoundBox() const
{
    return this->m_BoundBox;
}

AGE_API void pedRagdollMgr::Init(int skelCount, char** skelNames)
{
    m_ActiveData = new pedActiveData[skelCount];
    for (int i = 0; i < skelCount; i++)
    {
        m_ActiveData[i].Init(skelNames[i]);
    }

    m_Actives = new pedActive[NUM_RAGDOLLS];
    m_BoundBox = new phBoundBox();
    m_BoundBox->SetSize(Vector3(0.5f, 1.0f, 0.5f));
    this->Reset();
}

AGE_API pedActive* pedRagdollMgr::Attach(aiPedestrianInstance* ped)
{
    int attachToIndex = 0;
    for (int i = 0; i < NUM_RAGDOLLS; i++)
    {
        if (m_PedInstances[i] == nullptr)
        {
            attachToIndex = i;
            break;
        }
    }

    // reset if there's already an active in this slot
    if (m_PedInstances[attachToIndex])
    {
        m_PedInstances[attachToIndex]->Reset();
    }
    
    // setup this active
    auto active = &m_Actives[attachToIndex];
    active->Activate(ped, &this->m_ActiveData[ped->GetAnimationInstance()->GetAnimation()->GetHashTableIndex()]);
    m_PedInstances[attachToIndex] = ped;
    return active;
}

AGE_API void pedRagdollMgr::Detach(aiPedestrianInstance* ped)
{
    for (int i = 0; i < NUM_RAGDOLLS; i++)
    {
        if (m_PedInstances[i] == ped)
        {
            m_PedInstances[i] = nullptr;
            break;
        }
    }
}

AGE_API bool pedRagdollMgr::UnusedActive()
{
    for (int i = 0; i < NUM_RAGDOLLS; i++)
    {
        if (m_PedInstances[i] == nullptr)
        {
            return true;
        }
    }
    return false;
}

/*
    asNode virtuals
*/

AGE_API void pedRagdollMgr::Update()
{
    for (int i = 0; i < NUM_RAGDOLLS; i++)
    {
        auto instance = m_PedInstances[i];
        auto active = &m_Actives[i];
        if (instance != nullptr && active->IsAsleep())
        {
            m_PedInstances[i]->GetAnimationInstance()->SetActive(nullptr);
            m_PedInstances[i] = nullptr;
        }
    }
    for (int i = 0; i < NUM_RAGDOLLS; i++)
    {
        auto instance = m_PedInstances[i];
        if (instance)
        {
            dgPhysManager::Instance->DeclareMover(instance, 1, 0x1B);
        }
    }
}

AGE_API void pedRagdollMgr::Reset()                        
{
    for (int i = 0; i < NUM_RAGDOLLS; i++)
    {
        if (m_PedInstances[i])
        {
            (&m_Actives[i])->Reset();
            m_PedInstances[i] = nullptr;
        }
    }
}