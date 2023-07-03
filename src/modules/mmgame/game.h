#pragma once
#include <modules\node.h>

namespace MM2
{
    // Forward declarations
    class mmGame;

    // External declarations
    extern class mmPlayer;
    extern class mmPopup;
    extern class gizBridgeMgr;
    extern class gizTrainMgr;
    extern class mmWaypoints;
    
    // Class definitions
    class mmGame : public asNode {
    protected:
        static hook::Field<0x48, mmPlayer *> _player;
        static hook::Field<0x54, mmIcons*> _icons;
        static hook::Field<0x94, mmPopup *> _popup;
        static hook::Field<0x240, mmViewMgr> _viewManager;
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

        gizBridgeMgr* GetBridgeManager(void) const {
            return _bridgeManager.get(this);
        }

        gizTrainMgr* GetTrainManager(void) const {
            return _trainManager.get(this);
        }

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
        virtual AGE_API void SendChatMessage(char *message) { hook::Thunk<0x414E50>::Call<void>(this, message); };
        virtual AGE_API void SwitchState(int)               PURE;
        virtual AGE_API void BeDone(int p1)                 { hook::Thunk<0x414D30>::Call<void>(this, p1); };
        virtual AGE_API mmWaypoints * GetWaypoints(void)    PURE;

        static void BindLua(LuaState L) {
            LuaBinding(L).beginExtendClass<mmGame, asNode>("mmGame")
                //properties
                .addPropertyReadOnly("TrainManager", &GetTrainManager)
                .addPropertyReadOnly("BridgeManager", &GetBridgeManager)
                .addPropertyReadOnly("Player", &GetPlayer)
                .addPropertyReadOnly("Popup", &GetPopup)
                .addPropertyReadOnly("Icons", &GetIcons)
                .addPropertyReadOnly("ViewManager", &GetViewManager)
                .addPropertyReadOnly("Waypoints", &GetWaypoints)
                .addFunction("BeDone", &BeDone, LUA_ARGS(_def<int, 0>))
            .endClass();
        }
    };
}