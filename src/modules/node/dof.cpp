#include "dof.h"

using namespace MM2;

asDofCS::asDofCS() 
{
    scoped_vtable x(this);
    m_Type = DofType::Rotate;
    m_Mode = DofMode::Bounce;
    m_ModeAfterGoal = DofMode::Stop;
    m_Axis = Vector3(1.0f, 0.0f, 0.0f);
    m_Offset = Vector3(0.0f, 0.0f, 0.0f);
    m_Origin = Vector3(0.0f, 0.0f, 0.0f);
    m_Value = 0.0;
    dword_d0_possiblyunused = 0;
    m_LastValue = 41172.0;
    m_LastOffset = Vector3(56267.102f, 30631.699f, 93781.5f);
    m_Delta = 0.0;
    m_GoalValue = 0.0;
    m_SinTime = 0.0;
    m_SinAmp = 0.0;
    m_Delay = 0.0;
    m_DelayTimeRemaining = 0.0;
    m_Max = -1000000.0;
    m_Min = 1000000.0;
    m_SinFreq = 1.0;
    m_ClampValue = 1;
}

asDofCS::~asDofCS() 
{
    scoped_vtable x(this);
}

AGE_API void asDofCS::Reset()
{
    if (m_Type == DofType::Scale)
        m_Value = 1.0;
    else
        m_Value = 0.0;
    m_LastValue = -1.0;
    asNode::Reset();
}

AGE_API void asDofCS::Update()
{
    hook::Thunk<0x5970F0>::Call<void>(this);
}

void asDofCS::operator=(asDofCS const& other)
{
    m_Type = other.m_Type;
    m_Mode = other.m_Mode;
    m_Axis = other.m_Axis;
    m_Offset = other.m_Offset;
    m_Origin = other.m_Origin;
    m_Value = other.m_Value;
    m_LastValue = other.m_LastValue;
    m_LastOffset = other.m_LastOffset;
    m_Max = other.m_Max;
    m_Min = other.m_Min;
    m_Delta = other.m_Delta;
    m_GoalValue = other.m_GoalValue;
    dword_d0_possiblyunused = other.dword_d0_possiblyunused;
    m_ModeAfterGoal = other.m_ModeAfterGoal;
    m_SinAmp = other.m_SinAmp;
    m_SinTime = other.m_SinTime;
    m_SinFreq = other.m_SinFreq;
    m_DelayTimeRemaining = other.m_DelayTimeRemaining;
    m_Delay = other.m_Delay;
}

void asDofCS::SetTime(float value, float time, bool additive)
{
    m_Mode = DofMode::Goal;
    float goalValue = (additive) ? m_Value + value : value;
    m_GoalValue = goalValue;
    if (time == 0.0f)
    {
        m_Value = goalValue;
        m_Delta = 0.0f;
    }
    else
    {
        m_Delta = fabsf(goalValue - m_Value) / time;
    }
}

DofMode asDofCS::GetMode() const { return m_Mode; }
void asDofCS::SetMode(DofMode mode) { m_Mode = mode; }

DofType asDofCS::GetType() const { return m_Type; }
void asDofCS::SetType(DofType type) { m_Type = type; }

float asDofCS::GetMin() const { return m_Min; }
void asDofCS::SetMin(float value) { m_Min = value; }

float asDofCS::GetMax() const { return m_Max; }
void asDofCS::SetMax(float value) { m_Max = value; }

float asDofCS::GetDelay() const { return m_Delay; }
void asDofCS::SetDelay(float value) { m_Delay = value; }

Vector3 asDofCS::GetAxis() const { return m_Axis; }
void asDofCS::SetAxis(Vector3 const& axis) { m_Axis.Set(axis); }

Vector3 asDofCS::GetOffset() const { return m_Offset; }
void asDofCS::SetOffset(Vector3 const& offset) { m_Offset.Set(offset); }

Vector3 asDofCS::GetOrigin() const { return m_Origin; }
void asDofCS::SetOrigin(Vector3 const& origin) { m_Origin.Set(origin); }

float asDofCS::GetSinAmp() const { return m_SinAmp; }
void asDofCS::SetSinAmp(float value) { m_SinAmp = value; }

float asDofCS::GetSinFreq() const { return m_SinFreq; }
void asDofCS::SetSinFreq(float value) { m_SinFreq = value; }

void asDofCS::BindLua(LuaState L) {
    LuaBinding(L).beginExtendClass<asDofCS, asLinearCS>("asDofCs")
        .addProperty("Mode", &GetMode, &SetMode)
        .addProperty("Type", &GetType, &SetType)
        .addProperty("Min", &GetMin, &SetMin)
        .addProperty("Max", &GetMax, &SetMax)
        .addProperty("SinAmp", &GetSinAmp, &SetSinAmp)
        .addProperty("SinFreq", &GetSinFreq, &SetSinFreq)
        .addProperty("Delay", &GetDelay, &SetDelay)
        .addProperty("Axis", &GetAxis, &SetAxis)
        .addProperty("Origin", &GetOrigin, &SetOrigin)
        .addProperty("Offset", &GetOffset, &SetOffset)
        .addFunction("SetTime", &SetTime)
        .endClass();
}
