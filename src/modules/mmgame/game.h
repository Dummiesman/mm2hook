#pragma once
#include <modules\node.h>
#include "mmgamemusicdata.h"

namespace MM2
{
    // Forward declarations
    class mmGame;

    // External declarations
    extern class mmPlayer;
    extern class AudSoundBase;
    extern class mmPopup;
    extern class mmWaypoints;
    extern class mmIcons;
    extern class mmViewMgr;
    extern class gizBridgeMgr;
    extern class gizTrainMgr;
    extern class gizSailboatMgr;
    extern class gizFerryMgr;
    extern class mmRaceData;

    // Class definitions
    class mmGame : public asNode {
    private:
        char _buffer[0x7670];
    private:
        Vector4 luaRespawnXYZ(bool disallowHighwaySpawning, bool disallowAlleySpawning, bool useNetIdForSeed)
        {
            Vector3 pos = Vector3::ORIGIN;
            float rot = 0.0f;
            this->RespawnXYZ(pos, rot, disallowHighwaySpawning, disallowAlleySpawning, useNetIdForSeed);
            return Vector4(pos.X, pos.Y, pos.Z, rot);
        }
    protected:
        static hook::Field<0x48, mmPlayer *> _player;
        static hook::Field<0x8C, AudSoundBase*> _gameSounds;
        static hook::Field<0x98, mmRaceData*> _raceData;
        static hook::Field<0x54, mmIcons*> _icons;
        static hook::Field<0x94, mmPopup *> _popup;
        static hook::Field<0x240, mmViewMgr> _viewManager;
        static hook::Field<0x270, int> _state;
        static hook::Field<0x408, float> _stateTimer;
        static hook::Field<0x7670, gizSailboatMgr*> _sailboatManager;
        static hook::Field<0x7674, gizFerryMgr*> _ferryManager;
        static hook::Field<0x767C, gizBridgeMgr*> _bridgeManager;
        static hook::Field<0x7680, gizTrainMgr*> _trainManager;
    public:
        ANGEL_ALLOCATOR

        AGE_API mmGame(void) {
            scoped_vtable x(this);
            hook::Thunk<0x412550>::Call<void>(this);
        };

        virtual AGE_API ~mmGame(void) {
            scoped_vtable x(this);
            hook::Thunk<0x413940>::Call<void>(this);
        };

        mmPlayer * GetPlayer(void) const {
            return _player.get(this);
        };

        mmPopup * GetPopup(void) const {
            return _popup.get(this);
        };

        mmIcons* GetIcons(void) const {
            return _icons.get(this);
        }

        mmViewMgr* GetViewManager(void) const {
            return _viewManager.ptr(this);
        }

        gizSailboatMgr* GetSailboatManager(void) const {
            return _sailboatManager.get(this);
        }

        gizFerryMgr* GetFerryManager(void) const {
            return _ferryManager.get(this);
        }

        gizBridgeMgr* GetBridgeManager(void) const {
            return _bridgeManager.get(this);
        }

        gizTrainMgr* GetTrainManager(void) const {
            return _trainManager.get(this);
        }

        int GetState(void) const {
            return _state.get(this);
        }

        AudSoundBase* GetSounds(void) const {
            return _gameSounds.get(this);
        }

        void SetSounds(AudSoundBase* sounds) {
            _gameSounds.set(this, sounds);
        }

        AGE_API void RespawnXYZ(Vector3& pos, float& rot, bool disallowHighwaySpawning, bool disallowAlleySpawning, bool useNetIdForSeed)
                                                            { hook::Thunk<0x413B70>::Call<void>(this, &pos, &rot, disallowHighwaySpawning, disallowAlleySpawning, useNetIdForSeed); }
        AGE_API void InitWeather(void)                      { hook::Thunk<0x413370>::Call<void>(this); }
        AGE_API void SetIconsState(void)                    { hook::Thunk<0x414BB0>::Call<void>(this); }

        /*
            asNode virtuals
        */

        virtual AGE_API void Update(void) override          { hook::Thunk<0x413E90>::Call<void>(this); };
        virtual AGE_API void Reset(void) override           { hook::Thunk<0x413D40>::Call<void>(this); };
        virtual AGE_API void UpdatePaused(void) override    { hook::Thunk<0x4144A0>::Call<void>(this); };

        /*
            mmGame virtuals
        */

        virtual AGE_API int Init()                          { return hook::Thunk<0x412710>::Call<int>(this); };
        virtual AGE_API void InitGameStrings()              { hook::Thunk<0x413650>::Call<void>(this); };
        virtual AGE_API void InitMyPlayer()                 PURE;
        virtual AGE_API void InitOtherPlayers()             { hook::Thunk<0x4133F0>::Call<void>(this); };
        virtual AGE_API void InitGameObjects()              PURE;
        virtual AGE_API void InitHUD()                      PURE;
        virtual AGE_API void UpdateGameInput(int)           PURE;
        virtual AGE_API void UpdateDebugKeyInput(int)       PURE;
        virtual AGE_API void UpdateGame()                   PURE;
        virtual AGE_API void NextRace()                     PURE;
        virtual AGE_API void HitWaterHandler()              { hook::Thunk<0x414290>::Call<void>(this); };
        virtual AGE_API void DropThruCityHandler()          { hook::Thunk<0x414280>::Call<void>(this); };
        virtual AGE_API void SendChatMessage(LPCSTR *message) 
                                                            { hook::Thunk<0x414E50>::Call<void>(this, message); };
        virtual AGE_API void SwitchState(int)               PURE;
        virtual AGE_API void BeDone(int p1)                 { hook::Thunk<0x414D30>::Call<void>(this, p1); };
        virtual AGE_API mmWaypoints * GetWaypoints(void)    PURE;

        static void BindLua(LuaState L) {
            LuaBinding(L).beginExtendClass<mmGame, asNode>("mmGame")
                //properties
                .addProperty("State", &GetState, &SwitchState)
                .addProperty("Sounds", &GetSounds, &SetSounds)
                .addPropertyReadOnly("FerryManager", &GetFerryManager)
                .addPropertyReadOnly("SailboatManager", &GetSailboatManager)
                .addPropertyReadOnly("TrainManager", &GetTrainManager)
                .addPropertyReadOnly("BridgeManager", &GetBridgeManager)
                .addPropertyReadOnly("Player", &GetPlayer)
                .addPropertyReadOnly("Popup", &GetPopup)
                .addPropertyReadOnly("Icons", &GetIcons)
                .addPropertyReadOnly("ViewManager", &GetViewManager)
                .addPropertyReadOnly("Waypoints", &GetWaypoints)
                .addFunction("BeDone", &BeDone, LUA_ARGS(_def<int, 0>))
                .addFunction("SendChatMessage", &SendChatMessage)
                .addFunction("RespawnXYZ", &luaRespawnXYZ)
            .endClass();
        }
    };
    ASSERT_SIZEOF(mmGame, 0x7688);
}