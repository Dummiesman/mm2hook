#pragma once
#include <mm2_common.h>
#include <modules\node\node.h>

namespace MM2
{
    // Forward declarations
    class pedRagdollMgr;

    // External declarations
    extern class asNode;
    extern class aiPedestrianInstance;
    extern class phBoundBox;
    extern class pedActiveData;
    extern class pedActive;

    // Class definitions

    class pedRagdollMgr : public asNode {
    private:
        static const int NUM_RAGDOLLS = 16;
    private:
        pedActiveData* m_ActiveData;
        pedActive* m_Actives;
        aiPedestrianInstance* m_PedInstances[NUM_RAGDOLLS];
        phBoundBox* m_BoundBox;
    public:
        AGE_API pedRagdollMgr(void);
        virtual AGE_API ~pedRagdollMgr(void);
        static hook::Type<pedRagdollMgr *> Instance;

        phBoundBox* GetBoundBox() const;

        AGE_API void Init(int skelCount, char** skelNames);
        AGE_API pedActive* Attach(aiPedestrianInstance* a1);
        AGE_API void Detach(aiPedestrianInstance* a1);
        AGE_API bool UnusedActive();

        /*
            asNode virtuals
        */

        virtual AGE_API void Update() override;
        virtual AGE_API void Reset() override;
    };
}