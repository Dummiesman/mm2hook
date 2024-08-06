#pragma once
#include "pedscripted.h"
#include <modules\level\level.h>
#include <modules\phys\physmgr.h>

using namespace MM2;

/*
    pedScriptedPedInstance
*/

pedScriptedPedInstance::pedScriptedPedInstance(pedScriptedPed* pPed)
{
    m_Ped = pPed;
}

AGE_API const Vector3& pedScriptedPedInstance::GetPosition()
{
    return m_Ped->GetPosition();
}

AGE_API const Matrix34& pedScriptedPedInstance::GetMatrix(Matrix34* a1)
{
    a1->Set(m_Ped->GetMatrix());
    return *a1;
}

AGE_API void pedScriptedPedInstance::SetMatrix(const Matrix34& a1)
{
    Quitf("Cannot set pedScriptedPedInstance matrix from instance API!");
}

AGE_API void pedScriptedPedInstance::Draw(int lod)
{
    auto animationInstance = m_Ped->GetAnimation();
    gfxRenderState::SetWorldMatrix(m_Ped->GetMatrix());
    animationInstance.Draw(lod >= 2);
}

AGE_API unsigned int pedScriptedPedInstance::SizeOf(void)
{
    return sizeof(pedScriptedPedInstance);
}

/*
    pedScriptedPed
*/
pedScriptedPed::pedScriptedPed()
{
    m_Position.Set(Vector3::ORIGIN);
    m_FinalPosition.Set(Vector3::ORIGIN);
    m_CurrentRoom = 0;
    m_SnapToGround = true;
    m_State = pedScriptedPed::STATE_IDLE;
}

pedScriptedPed::~pedScriptedPed()
{
    delete m_Instance;
}

void pedScriptedPed::Init(const char* pedName)
{
    m_Animation.Init(pedName);
    m_seqSTAND = m_Animation.GetAnimation()->LookupSequence("STAND");
    m_seqRUN = m_Animation.GetAnimation()->LookupSequence("RUN");
    m_seqWALK = m_Animation.GetAnimation()->LookupSequence("WALK");
    m_Instance = new pedScriptedPedInstance(this);
}

void MM2::pedScriptedPed::Reset()
{
    this->Stop();
}

void pedScriptedPed::PlayAnimation(const char* name, bool loop)
{
    int index = m_Animation.GetAnimation()->LookupSequence(name);
    m_Animation.Start(index);
    m_GenAnimLooping = loop;
    m_GenAnimStateNum = index;
    m_LastAnimFrame = 0;
    m_LastAnimState = index;
}

void pedScriptedPed::Stop()
{
    m_State = pedScriptedPed::STATE_IDLE;
    m_Animation.Start(m_seqSTAND);
    m_LastAnimFrame = 0;
    m_LastAnimState = m_seqSTAND;
}

void pedScriptedPed::TravelTo(Vector3 target, bool run)
{
    int animationNum = (run) ? m_seqRUN : m_seqWALK;
    m_Target.Set(target);
    m_State = pedScriptedPed::STATE_TARGETING;
    if (m_Animation.GetCurrentState() != animationNum)
    {
        m_Animation.Start(animationNum);
        m_LastAnimFrame = 0;
        m_LastAnimState = animationNum;
    }
}

void pedScriptedPed::DiveLeft()
{
    this->PlayAnimation("LDIVE", false);
}

void pedScriptedPed::DiveRight()
{
    this->PlayAnimation("RDIVE", false);
}

pedAnimationInstance pedScriptedPed::GetAnimation() const
{
    return m_Animation;
}

Matrix34 pedScriptedPed::GetMatrix() const
{
    Matrix34 mtx;

    Vector3 side = Vector3::YAXIS;
    side.Cross(m_Forward);
    mtx.SetRow(0, side);

    mtx.SetRow(1, Vector3::YAXIS);
    mtx.SetRow(2, m_Forward);
    mtx.SetRow(3, m_FinalPosition);
    return mtx;
}

const Vector3& pedScriptedPed::GetPosition() const
{
    return m_Position;
}

void pedScriptedPed::SetPosition(Vector3 const& position)
{
    Vector3 previousFinalPosition = Vector3(m_FinalPosition);
    m_Position.Set(position);
    if (m_SnapToGround)
    {
        lvlSegment segment;
        lvlIntersection intersection;

        segment.IsVertical = true;
        segment.StartPos = m_Position + (Vector3::YAXIS * 5.0f);
        segment.EndPos = m_Position - (Vector3::YAXIS * 10.0f);
        if (dgPhysManager::Instance->Collide(segment, &intersection, m_CurrentRoom, nullptr, 0x20))
        {
            m_FinalPosition = intersection.IntersectionPoint.Point;
        }
        else
        {
            m_FinalPosition = Vector3(m_Position.X, m_FinalPosition.Y, m_Position.Z);
        }
    }
    else
    {
        m_FinalPosition = m_Position;
    }
}

void pedScriptedPed::SetMatrix(Matrix34 const& matrix)
{
    this->SetPosition(matrix.GetRow(3));
    m_Forward.Set(matrix.GetRow(2));
    m_Forward.Y = 0.0f;
    m_Forward.Normalize();
}

void MM2::pedScriptedPed::UpdateWalkRun()
{
    Vector3 dirToTarget = m_Target - m_Position;
    dirToTarget.Normalize();
    float distToTarget = m_Target.Dist(m_Position);

    auto& sequence = m_Animation.GetAnimation()->Animations[m_Animation.GetCurrentState()];
    float fMoveAmount = sequence.FSpeed * datTimeManager::Seconds;
    fMoveAmount = fminf(fMoveAmount, distToTarget);

    float sMoveAmount = sequence.LSpeed * datTimeManager::Seconds;
    Vector3 sideDir = Vector3(dirToTarget);
    sideDir.Dot(Vector3::YAXIS);

    this->SetPosition(m_Position + (dirToTarget * fMoveAmount));
    if (m_Position.Dist(m_Target) == 0.0f)
    {
        this->Stop();
    }

    m_Forward.Set(dirToTarget * -1.0f);
    m_Forward.Y = 0.0f;
    m_Forward.Normalize();
}

void pedScriptedPed::UpdateAnimation()
{
    // check for looping around, or changed state
    if ((m_Animation.GetCurrentFrame() < m_LastAnimFrame && !m_GenAnimLooping) || m_Animation.GetCurrentState() != m_GenAnimStateNum)
    {
        this->Stop();
        return;
    }
    
    // update
    auto& sequence = m_Animation.GetAnimation()->Animations[m_Animation.GetCurrentState()];
    float fMoveAmount = sequence.FSpeed * datTimeManager::Seconds;

    float sMoveAmount = sequence.LSpeed * datTimeManager::Seconds;
    Vector3 sideDir = Vector3(m_Forward);
    sideDir.Dot(Vector3::YAXIS);

    this->SetPosition(m_Position + (m_Forward * fMoveAmount) + (sideDir * sMoveAmount));
}

void pedScriptedPed::Update()
{
    switch (m_State)
    {
    case pedScriptedPed::STATE_IDLE:
        break;
    case pedScriptedPed::STATE_TARGETING:
        this->UpdateWalkRun();
        break;
    case pedScriptedPed::STATE_GENANIM:
        this->UpdateAnimation();
    }
    
    m_Animation.Update();
    m_CurrentRoom = lvlLevel::GetSingleton()->FindRoomId(m_Position, m_CurrentRoom);
    if (m_Instance->GetRoomId() != m_CurrentRoom)
    {
        lvlLevel::GetSingleton()->MoveToRoom(m_Instance, m_CurrentRoom);
    }
    m_LastAnimFrame = m_Animation.GetCurrentFrame();
    m_LastAnimState = m_Animation.GetCurrentState();
}

void pedScriptedPed::BindLua(LuaState L)
{
    LuaBinding(L).beginClass<pedScriptedPed>("pedScriptedPed")
        .addFactory([]() { return new pedScriptedPed(); })
        .addFunction("Init", &Init)
        .addFunction("Stop", &Stop)
        .addFunction("TravelTo", &TravelTo)
        .addFunction("DiveLeft", &DiveLeft)
        .addFunction("DiveRight", &DiveRight)
        .addFunction("PlayAnimation", &PlayAnimation)
        .addFunction("Update", &Update)
        .addFunction("GetPosition", &GetPosition)
        .addFunction("SetPosition", &SetPosition)
        .addFunction("GetMatrix", &GetMatrix)
        .addFunction("SetMatrix", &SetMatrix)
        .endClass();
}
