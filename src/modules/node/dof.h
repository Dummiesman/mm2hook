#pragma once
#include "linear.h"

namespace MM2
{
    // Forward declarations
    class asDofCS;

    // External declarations


    // Class definitions
    enum class DofMode : int
    {
        Bounce = 0x0,
        Loop = 0x1,
        Stop = 0x2,
        Goal = 0x3,
    };

    enum class DofType : int
    {
        Rotate = 0x0,
        Translate = 0x1,
        Scale = 0x2,
    };

    class asDofCS : public asLinearCS
    {
    private:
        DofType m_Type;
        DofMode m_Mode;
        Vector3 m_Axis;
        Vector3 m_Offset;
        Vector3 m_Origin;
        Vector3 m_LastOffset;
        float m_Value;
        float m_LastValue;
        float m_Max;
        float m_Min;
        float m_Delta;
        float m_GoalValue;
        int dword_d0_possiblyunused;
        float m_SinAmp;
        float m_SinFreq;
        float m_SinTime;
        float m_DelayTimeRemaining;
        float m_Delay;
        int m_ClampValue;
        DofMode m_ModeAfterGoal;
    public:
        AGE_API asDofCS();
        virtual AGE_API ~asDofCS();

        /*
            asNode Virtuals
        */
        virtual AGE_API void Reset() override;
        virtual AGE_API void Update() override;

        /*
            asDofCs Members
        */
        void operator=(asDofCS const& other);

        void SetTime(float value, float time, bool additive);

        DofMode GetMode() const;
        void SetMode(DofMode mode);
        DofType GetType() const;
        void SetType(DofType type);
        float GetMin() const;
        void SetMin(float value);
        float GetMax() const;
        void SetMax(float value);
        float GetDelay() const;
        void SetDelay(float value);
        Vector3 GetAxis() const;
        void SetAxis(Vector3 const& axis);
        Vector3 GetOffset() const;
        void SetOffset(Vector3 const& offset);
        Vector3 GetOrigin() const;
        void SetOrigin(Vector3 const& origin);
        float GetSinAmp() const;
        void SetSinAmp(float value);
        float GetSinFreq() const;
        void SetSinFreq(float value);

        //lua
        static void BindLua(LuaState L);
    };

    ASSERT_SIZEOF(asDofCS, 0xF0);
}