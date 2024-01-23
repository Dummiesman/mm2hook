#pragma once
#include <modules\mminput.h>

namespace MM2
{
    // Forward declarations
    class mmInput;

    // External declarations


    // Class definitions
    class mmInput {
    private:
        char buffer[0x248];
    private:
        float getSteeringLua()  {
            return GetSteering();
        }
    protected:
        static hook::Field<0x1D4, BOOL> _pedalsSwapped;
        static hook::Field<0x18C, BOOL> _autoReverse;
    public:
        static hook::Type<mmInput*> GameInputPtr;
    public:
        mmInput(void)                                                   DONOTCALL;
        
        AGE_API float GetThrottle() const                               { return hook::Thunk<0x52D950>::Call<float>(this); }
        AGE_API float GetThrottleVal() const                            { return hook::Thunk<0x52D970>::Call<float>(this); }
        AGE_API float GetBrakes() const                                 { return hook::Thunk<0x52D9E0>::Call<float>(this); }
        AGE_API float GetBrakesVal() const                              { return hook::Thunk<0x52DA0>::Call<float>(this); }
        AGE_API float GetHandBrake() const                              { return hook::Thunk<0x52DA70>::Call<float>(this); }
        AGE_API float GetSteering(float(__cdecl* filterFunc)(float) = nullptr) 
                                                                        { return hook::Thunk<0x52DE90>::Call<float>(this, filterFunc); }
        AGE_API float GetCamPan() const                                 { return hook::Thunk<0x52DF60>::Call<float>(this); }

        AGE_API BOOL DeviceConnected() const                            { return hook::Thunk<0x52E0A0>::Call<BOOL>(this); }
        AGE_API BOOL JoystickConnected() const                          { return hook::Thunk<0x52E0C0>::Call<BOOL>(this); }
        AGE_API BOOL GamepadConnected() const                           { return hook::Thunk<0x52E0D0>::Call<BOOL>(this); }
        AGE_API BOOL WheelConnected() const                             { return hook::Thunk<0x52E0E0>::Call<BOOL>(this); }

        AGE_API BOOL JoystickHasCoolie() const                          { return hook::Thunk<0x52E120>::Call<BOOL>(this); }
        AGE_API BOOL JoystickHasThrottle() const                        { return hook::Thunk<0x52E140>::Call<BOOL>(this); }
        AGE_API BOOL WheelHas3Axis() const                              { return hook::Thunk<0x52E160>::Call<BOOL>(this); }

        AGE_API void SetDeadZone(float  deadzone)                       { hook::Thunk<0x52E0F0>::Call<void>(this, deadzone); }
        AGE_API void SetForceFeedbackScale(float  scale)                { hook::Thunk<0x52E1E0>::Call<void>(this, scale); }
        AGE_API void SetRoadForceScale(float  scale)                    { hook::Thunk<0x52E230>::Call<void>(this, scale); }

        AGE_API void PrintIODev()                                       { hook::Thunk<0x52E610>::Call<void>(this); }
        AGE_API void SaveCodeConfig()                                   { hook::Thunk<0x52E6E0>::Call<void>(this); } // Really int (Stream*) but always returns zero and doesn't use parameter

        void SwapBrakeAndThrottleInput(bool swap)                       { _pedalsSwapped.set(this, (swap) ? TRUE : FALSE); }
        bool AutoReverseEnabled() const                                 { return _autoReverse.get(this) == TRUE; }
        bool PedalsSwapped() const                                      { return _pedalsSwapped.get(this) == TRUE; }

        //lua
        static void BindLua(LuaState L) {
            LuaBinding(L).beginClass<mmInput>("mmInput")
                .addFunction("GetThrottle", &GetThrottle)
                .addFunction("GetThrottleVal", &GetThrottleVal)
                .addFunction("GetBrakes", &GetBrakes)
                .addFunction("GetBrakesVal", &GetBrakesVal)
                .addFunction("GetSteering", &getSteeringLua)
                .addFunction("GetHandBrake", &GetHandBrake)
                .addFunction("GetCamPan", &GetCamPan)

                .addFunction("PrintIODev", &PrintIODev)
                .addFunction("SaveCodeConfig", &SaveCodeConfig)

                .endClass();
        }
    };
    declhook(0x6B1CF0, _Type<mmInput*>, GameInputPtr);
}