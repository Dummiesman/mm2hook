#pragma once
#include <modules\node.h>
#include <modules\mmgame.h>

namespace MM2
{
    // Forward declarations
    class mmGameManager;

    // External declarations


    // Class definitions
    class mmGameManager: public asNode {
    private:
        asCamera m_Camera;
        mmGame *m_CurrentGame;
        mmSingleRoam *m_SingleRoamGame;
        mmSingleRace *m_SingleRaceGame;
        mmSingleStunt *m_SingleStuntGame;
        mmSingleCircuit *m_SingleCircuitGame;
        mmSingleBlitz *m_SingleBlitzGame;
        mmMultiRace *m_MultiRaceGame;
        mmMultiRoam* m_MultiRoamGame;
        mmMultiCR* m_MultiCRGame;
        mmMultiCircuit* m_MultiCircuitGame;
        mmMultiBlitz* m_MultiBlitzGame;
        int NumUpdateCallsSinceReset;
    private:
        static mmGame* sm_CustomGame; // Extension for hook
    public:
        bool isCurrentGameSingleGame() const;
        bool isCurrentGameMultiGame() const;
        mmGame* getGame() const;
        mmPlayer* getPlayerSafe() const;
    public:
        ANGEL_ALLOCATOR
        void* operator new (std::size_t, void* p) throw() { return p; }

        static hook::Type<mmGameManager *> Instance;

        AGE_API mmGameManager(void);
        virtual AGE_API ~mmGameManager(void);

        /*
            asNode virtuals
        */
        AGE_API void Cull()  override;
        AGE_API void Update()  override;
        AGE_API void Reset()  override;

        /* TODO?
        mmGameManager::mmGameManager(void)
        mmGameManager::~mmGameManager(void)

        void mmGameManager::BeDone(void)
        void mmGameManager::ForcePopupUI(void)
        void mmGameManager::ForceReplayUI(void)
        */

        static void BindLua(LuaState L);
    };

    ASSERT_SIZEOF(mmGameManager, 0x1B8);
}