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
    public:
        bool isCurrentGameSingleGame() const {
            auto game = getGame();
            return (game != nullptr && (game == m_SingleRoamGame || game == m_SingleRaceGame || game == m_SingleStuntGame
                                        || game == m_SingleCircuitGame || game == m_SingleBlitzGame));
        }

        bool isCurrentGameMultiGame() const {
            auto game = getGame();
            return (game != nullptr && (game == m_MultiRaceGame || game == m_MultiRoamGame || game == m_MultiCRGame
                                        || game == m_MultiCircuitGame || game == m_MultiBlitzGame));
        }

        mmGame* getGame() const {
            return this->m_CurrentGame;
        };

        mmPlayer* getPlayerSafe() const {
            auto game = this->getGame();
            if (game == nullptr)
                return nullptr;
            auto player = game->GetPlayer();
            return player;
        }
    public:
        ANGEL_ALLOCATOR
        void* operator new (std::size_t, void* p) throw() { return p; }

        static hook::Type<mmGameManager *> Instance;

        AGE_API mmGameManager(void)  {
            scoped_vtable x(this);
            hook::Thunk<0x4029E0>::Call<void>(this);
        }

        virtual AGE_API ~mmGameManager(void) {
            scoped_vtable x(this);
            hook::Thunk<0x402E40>::Call<void>(this);
        }

        /*
            asNode virtuals
        */
        AGE_API void Cull()  override                       { hook::Thunk<0x4031D0>::Call<void>(this); }
        AGE_API void Update()  override                     { hook::Thunk<0x403000>::Call<void>(this); }
        AGE_API void Reset()  override                      { hook::Thunk<0x402E30>::Call<void>(this); }

        /* TODO?
        mmGameManager::mmGameManager(void)
        mmGameManager::~mmGameManager(void)

        void mmGameManager::BeDone(void)
        void mmGameManager::ForcePopupUI(void)
        void mmGameManager::ForceReplayUI(void)
        */

        static void BindLua(LuaState L) {
            LuaBinding(L).beginExtendClass<mmGameManager, asNode>("mmGameManager")
                //properties
                .addPropertyReadOnly("Game", &getGame)
                
                //statics
                .addStaticFunction("Instance", [] {return (mmGameManager *)Instance; })
            .endClass();
        }
    };

    ASSERT_SIZEOF(mmGameManager, 0x1B8);
}