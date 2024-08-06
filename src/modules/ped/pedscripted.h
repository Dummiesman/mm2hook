#pragma once
#include "pedanim.h"
#include <modules\level\inst.h>

namespace MM2
{
    // Forward declarations
    class pedScriptedPedInstance;
    class pedScriptedPed;    

    // Class definitions
    class pedScriptedPedInstance : public lvlInstance
    {
    private:
        pedScriptedPed* m_Ped;
    public:
        pedScriptedPedInstance(pedScriptedPed* pPed);

        // Inherited via lvlInstance
        AGE_API const Vector3& GetPosition() override;
        AGE_API const Matrix34& GetMatrix(Matrix34* a1) override;
        AGE_API void SetMatrix(const Matrix34& a1) override;
        AGE_API void Draw(int) override;
        AGE_API unsigned int SizeOf(void) override;
    };

    class pedScriptedPed  {
    private:
        static const int STATE_IDLE = 0;
        static const int STATE_TARGETING = 1;
        static const int STATE_GENANIM = 2;
    private:
        pedAnimationInstance m_Animation;
        Vector3 m_FinalPosition;
        Vector3 m_Position;
        Vector3 m_Forward;
        Vector3 m_Target;
        int m_State;
        bool m_SnapToGround;
        int m_CurrentRoom;

        // State info
        bool m_GenAnimLooping;
        int m_GenAnimStateNum;
        int m_LastAnimFrame;
        int m_LastAnimState;

        // Instance ptr
        pedScriptedPedInstance* m_Instance;

        // Animation states
        int m_seqSTAND;
        int m_seqWALK;
        int m_seqRUN;
    public:
        pedScriptedPed();
        ~pedScriptedPed();

        void Init(const char* pedName);
        void Reset();

        void PlayAnimation(const char* name, bool loop);
        void Stop();
        void TravelTo(Vector3 target, bool run);
        void DiveLeft();
        void DiveRight();

        pedAnimationInstance GetAnimation() const;
        Matrix34 GetMatrix() const;
        const Vector3& GetPosition() const;
        void SetPosition(Vector3 const & position);
        void SetMatrix(Matrix34 const& matrix);

        void UpdateWalkRun();
        void UpdateAnimation();
        void Update();

        // lua
        static void BindLua(LuaState L);
    };
}