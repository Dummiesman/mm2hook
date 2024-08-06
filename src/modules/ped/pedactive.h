#pragma once
#include <modules\phys\physentity.h>
#include <modules\phys\phsleep.h>

namespace MM2
{
    // Forward declarations
    class pedActive;
    class pedActiveData;

    // External declarations
    extern class crSkeleton;
    extern class dgPhysEntity;
    extern class dgRagdoll;
    extern class aiPedestrianInstance;

    // Class definitions

    class pedActiveData {
    private:
        void* SkeletonData; /*crSkeletonData*/
        void* BodyData; /*crBodyData*/
        void* RagdollData; /*dgRagdollData*/
    public:
        ANGEL_ALLOCATOR

        pedActiveData() 
        {
            hook::Thunk<0x57BE90>::Call<void>(this);
        }

        void Init(const char* skelName)
        {
            hook::Thunk<0x57BEA0>::Call<void>(this, skelName);
        }
    };

    class pedActive : public dgPhysEntity {
    private:
        phSleep m_Sleep;
        crSkeleton* m_Skeleton;
        dgRagdoll* m_Ragdoll;
        int m_Kinematics;
        aiPedestrianInstance* m_PedInstance;
    public:
        pedActive()
        {
            scoped_vtable x(this);
            hook::Thunk<0x057BD70>::Call<void>(this);
        }

        virtual ~pedActive()
        {
            scoped_vtable x(this);
            hook::Thunk<0x57BF80>::Call<void>(this);
        }

        crSkeleton * GetSkeleton() const             { return m_Skeleton; }
        dgRagdoll * GetRagdoll() const               { return m_Ragdoll; }

        BOOL IsAsleep() const
        {
            return hook::Thunk<0x57C090>::Call<BOOL>(this);
        }

        /*
            dgPhysEntity virtuals
        */
        virtual void Update() override                      { hook::Thunk<0x57C0F0>::Call<void>(this); }
        virtual void PostUpdate() override                  { hook::Thunk<0x57C500>::Call<void>(this); }
        virtual phInertialCS* GetICS() override             { return hook::Thunk<0x57C290>::Call<phInertialCS*>(this); }
        virtual lvlInstance* GetInst() override             { return hook::Thunk<0x57C2A0>::Call<lvlInstance*>(this); }
        virtual void FirstImpactCallback() override         { hook::Thunk<0x57C2E0>::Call<void>(this); }

        /*
            pedActive virtuals 
        */
        virtual void Reset()                                                      { hook::Thunk<0x57C0C0>::Call<void>(this); }
        virtual void Activate(aiPedestrianInstance* pedInst, pedActiveData* data) { hook::Thunk<0x57C170>::Call<void>(this, pedInst, data); }
        virtual void Deactivate()                                                 
        {
            pedRagdollMgr::Instance->Detach(m_PedInstance);
            m_PedInstance = nullptr;
        }
    };
    
    ASSERT_SIZEOF(pedActive, 0x114);
}