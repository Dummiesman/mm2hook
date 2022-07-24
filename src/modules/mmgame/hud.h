#pragma once
#include <modules\node.h>
#include "cd.h"

namespace MM2
{
    // Forward declarations
    class mmArrow;
    class mmHUD;
    class mmTimer;

    // External declarations
    extern class mmPlayer;
    extern class mmHudMap;

    // Class definitions
    class mmArrow : public asNode {
    private:
        static Vector3 m_LuaInterest;

        //lua helpers
        void luaClearInterest(void) {
            this->SetInterest(nullptr);
        }

        void luaSetInterest(Vector3 interest) {
            m_LuaInterest.Set(interest);
            this->SetInterest(&m_LuaInterest);
        }
    private: 
        byte _buffer[0x50];
    public:
        /*
            asNode virtuals
        */
        AGE_API void Reset() override                       { hook::Thunk<0x42E780>::Call<void>(this); }
        AGE_API void Update() override                      { hook::Thunk<0x42E7F0>::Call<void>(this); }

        /*
            mmArrow
        */
        AGE_API void SetInterest(Vector3 *interestPoint)    { hook::Thunk<0x42E790>::Call<void>(this, interestPoint); }

        static void BindLua(LuaState L) {
            LuaBinding(L).beginExtendClass<mmArrow, asNode>("mmArrow")
                .addFunction("SetInterest", &luaSetInterest)
                .addFunction("ClearInterest", &luaClearInterest)
                .endClass();
        }                                                      
    };
    ASSERT_SIZEOF(mmArrow, 0x68);

    class mmTimer : public asNode {
    private:
        BOOL ReverseMode;
        BOOL TicksMode;
        float StartTime;
        float Time;
        BOOL Running;
        Timer Timer;
    private:
        void luaInit(bool reverse, float startTime, bool ticksMode) {
            this->Init(reverse ? TRUE : FALSE, startTime, ticksMode ? TRUE : FALSE);
        }
    public:
        inline bool GetCountdownMode() {
            return this->ReverseMode == TRUE;
        }

        inline void SetCountdownMode(bool mode) 
        {
            this->ReverseMode = (mode) ? TRUE : FALSE;
        }

        inline bool GetRunning() {
            return this->Running == TRUE;
        }
    public:
        ANGEL_ALLOCATOR

        AGE_API mmTimer(void) {
            scoped_vtable x(this);
            hook::Thunk<0x42E420>::Call<void>(this);
        };

        virtual AGE_API ~mmTimer(void) {
            scoped_vtable x(this);
            hook::Thunk<0x42E480>::Call<void>(this);
        };

        AGE_API void Init(BOOL reverse, float startTime, BOOL ticksMode)
                                                    { hook::Thunk<0x42E490>::Call<void>(this, reverse, startTime, ticksMode); }
        AGE_API void Start()                        { hook::Thunk<0x42E610>::Call<void>(this); }
        AGE_API void Stop()                         { hook::Thunk<0x42E630>::Call<void>(this); }
        AGE_API void StartStop()                    { hook::Thunk<0x42E640>::Call<void>(this); }
        AGE_API float GetTime()                     { return hook::Thunk<0x42E4C0>::Call<float>(this); }

        /*
            asNode virtuals
        */

        AGE_API void Update() override              { hook::Thunk<0x42E4D0>::Call<void>(this); }
        AGE_API void Reset() override               { hook::Thunk<0x42E5F0>::Call<void>(this); }

        static void BindLua(LuaState L) {
            LuaBinding(L).beginExtendClass<mmTimer, asNode>("mmTimer")
                .addConstructor(LUA_ARGS())

                .addFunction("Init", &luaInit)
                .addFunction("Start", &Start)
                .addFunction("Stop", &Stop)
                .addFunction("StartStop", &StartStop)

                .addVariableRef("StartTime", &mmTimer::StartTime)
                .addPropertyReadOnly("Time", &GetTime)
                .addPropertyReadOnly("Running", &GetRunning)
                .addProperty("CountdownMode", &GetCountdownMode, &SetCountdownMode)
                .endClass();
        }
    };
    ASSERT_SIZEOF(mmTimer, 0x30);

    class mmHUD : public asNode {
    private:
        byte _buffer[0xBC0]; // unconfirmed
    protected:
        static hook::Field<0xB94, mmCDPlayer*> _cdplayer;
        static hook::Field<0xBA0, mmHudMap*> _hudmap;
        static hook::Field<0xBA4, camViewCS*> _camview;
        static hook::Field<0x9BC, mmArrow> _arrow;
        static hook::Field<0x950, BOOL> _showTimer;
        static hook::Field<0x914, BOOL> _useCountdownTimer;
        static hook::Field<0xA54, mmTimer> _timer;
        static hook::Field<0xA84, mmTimer> _countdownTimer;
    public:
        mmTimer* GetTimer()
        {
            return _timer.ptr(this);
        }

        mmTimer* GetCountdownTimer()
        {
            return _countdownTimer.ptr(this);
        }

        bool GetUseCountdownTimer()
        {
            return _useCountdownTimer.get(this) == TRUE;
        }

        void SetUseCountdownTimer(bool value)
        {
            _useCountdownTimer.set(this, (value) ? TRUE : FALSE);
        }

        bool GetShowTimer()
        {
            return _showTimer.get(this) == TRUE;
        }

        void SetShowTimer(bool value)
        {
            _showTimer.set(this, (value) ? TRUE : FALSE);
        }

        mmCDPlayer* GetCDPlayer() 
        {
            return _cdplayer.get(this);
        }

        mmArrow* GetArrow() 
        {
            return _arrow.ptr(this);
        }

        camViewCS* GetCamView() 
        {
            return _camview.get(this);
        }

        mmHudMap* GetHudMap()
        {
            return _hudmap.get(this);
        }

        /*
            asNode virtuals
        */
        AGE_API void Update() override                      { hook::Thunk<0x42DBC0>::Call<void>(this); }
        AGE_API void UpdatePaused() override                { hook::Thunk<0x42DF10>::Call<void>(this); }
        AGE_API void Cull() override                        { hook::Thunk<0x42DF40>::Call<void>(this); }
        AGE_API void Reset() override                       { hook::Thunk<0x42DB20>::Call<void>(this); }
        AGE_API void ResChange(int width, int height) override
                                                            { hook::Thunk<0x42D2E0>::Call<void>(this, width, height); }

        /*
            mmHUD
        */
        AGE_API void Init(LPCSTR vehName, mmPlayer* player, BOOL useAltTimingMethod)
                                                            { hook::Thunk<0x42D5E0>::Call<void>(this); }
        AGE_API void StartTimers()                          { hook::Thunk<0x42D560>::Call<void>(this); }
        AGE_API void StopTimers()                           { hook::Thunk<0x42D580>::Call<void>(this); }
        AGE_API void ResetTimers()                          { hook::Thunk<0x42D5A0>::Call<void>(this); }
        AGE_API void Enable()                               { hook::Thunk<0x42D910>::Call<void>(this); }
        AGE_API void Disable(BOOL a2)                       { hook::Thunk<0x42D970>::Call<void>(this, a2); }
        AGE_API void Toggle()                               { hook::Thunk<0x42D9D0>::Call<void>(this); }
        AGE_API void SetDash(BOOL active)                   { hook::Thunk<0x42DA90>::Call<void>(this, active); }
        AGE_API void ActivateDash()                         { hook::Thunk<0x42DAB0>::Call<void>(this); }
        AGE_API void DeactivateDash()                       { hook::Thunk<0x42DAE0>::Call<void>(this); }
        AGE_API void ActivateGold()                         { hook::Thunk<0x42E300>::Call<void>(this); }
        AGE_API void DeactivateGold()                       { hook::Thunk<0x42E310>::Call<void>(this); }
        AGE_API BOOL IsDashActive()                         { return hook::Thunk<0x42DB10>::Call<BOOL>(this); }
        AGE_API void SetTransparency(bool transparency)     { hook::Thunk<0x42DB60>::Call<void>(this, transparency); }
        AGE_API void ShowSplitTime()                        { hook::Thunk<0x42E040>::Call<void>(this); }
        AGE_API void SetStandings(int lhs, int rhs)         { hook::Thunk<0x42E0A0>::Call<void>(this, lhs, rhs); }
        AGE_API void PostLapTime(bool finalLap)             { hook::Thunk<0x42E0E0>::Call<void>(this, finalLap); }
        AGE_API void SetLapTime(int lap, int a3, bool stopTimer) 
                                                            { hook::Thunk<0x42E140>::Call<void>(this, lap, a3, stopTimer); }
        AGE_API void SetMessage(LPCSTR message, float duration, int position)
                                                            { hook::Thunk<0x42E1F0>::Call<void>(this, message, duration, position); };
        AGE_API void SetMessage(LPCSTR message)             { hook::Thunk<0x42E240>::Call<void>(this, message); };
        AGE_API void PostChatMessage(LPCSTR message)        { hook::Thunk<0x42D280>::Call<void>(this, message); };
        AGE_API void PlayNetAlert()                         { hook::Thunk<0x42E340>::Call<void>(this); }
        AGE_API void TogglePositionDisplay(BOOL enable)     { hook::Thunk<0x42E360>::Call<void>(this, enable); }

        static void BindLua(LuaState L) {
            LuaBinding(L).beginExtendClass<mmHUD, asNode>("mmHUD")
                .addPropertyReadOnly("CDPlayer", &GetCDPlayer)
                .addPropertyReadOnly("Arrow", &GetArrow)
                .addPropertyReadOnly("Map", &GetHudMap)
                .addPropertyReadOnly("CamView", &GetCamView)
                
                .addPropertyReadOnly("Timer", &GetTimer)
                .addPropertyReadOnly("CountdownTimer", &GetCountdownTimer)

                .addProperty("UseCountdownTimer", &GetUseCountdownTimer, &SetUseCountdownTimer)
                .addProperty("ShowTimer", &GetShowTimer, &SetShowTimer)

                .addFunction("Init", &Init, LUA_ARGS(LPCSTR, mmPlayer*, BOOL))
                .addFunction("SetMessage", static_cast<void (mmHUD::*)(LPCSTR, float, int)>(&SetMessage), LUA_ARGS(LPCSTR, _def<float, 3>, _def<int, 0>))
                .addFunction("StartTimers", &StartTimers)
                .addFunction("StopTimers", &StopTimers)
                .addFunction("ResetTimers", &ResetTimers)
                .addFunction("Enable", &Enable)
                .addFunction("Disable", &Disable, LUA_ARGS(bool))
                .addFunction("Toggle", &Toggle)
                .addFunction("SetDash", &SetDash, LUA_ARGS(bool))
                .addFunction("ActivateDash", &ActivateDash)
                .addFunction("DeactivateDash", &DeactivateDash)
                .addFunction("SetTransparency", &SetTransparency)
                .addFunction("ShowSplitTime", &ShowSplitTime)
                .addFunction("SetStandings", &SetStandings)
                .addFunction("PostLapTime", &PostLapTime)
                .addFunction("SetLapTime", &SetLapTime)
                .addFunction("PostChatMessage", &PostChatMessage)
                .addFunction("PlayNetAlert", &PlayNetAlert)
                .addFunction("TogglePositionDisplay", &TogglePositionDisplay, LUA_ARGS(bool))
            .endClass();
        }
    };
}