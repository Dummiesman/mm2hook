#pragma once
#include <modules/node/camera.h>
namespace MM2
{
    // Forward declarations
    class asViewCS;

    // External declarations

    // Class definitions
    enum class asViewCsMode : int
    {
        Polar = 0,
        Roam = 1,
        POV = 2,
        LookAt = 3,
        Track = 4
    };

    class asViewCS : public asLinearCS {
    private:
        byte _buffer[0x9C];
    protected:
        static hook::Field<0x80, asViewCsMode> _mode;
        static hook::Field<0x84, BOOL> _stereoEnabled;
        static hook::Field<0x98, Vector3> _offset;
        static hook::Field<0xA4, float> _distance; 
        static hook::Field<0xAC, float> _incline;
        static hook::Field<0xB4, float> _spinRate;
        static hook::Field<0xB8, float> _bobAmp;
        static hook::Field<0xBC, float> _bobFreq;    
    public:
        AGE_API asViewCS(void) {
            scoped_vtable x(this);
            hook::Thunk<0x596200>::Call<void>(this);
        };

        virtual AGE_API ~asViewCS(void) {
            scoped_vtable x(this);
            hook::Thunk<0x596E10>::Call<void>(this);
        };


        asViewCsMode GetMode() const          { return _mode.get(this); }
        void SetMode(asViewCsMode mode)       { _mode.set(this, mode); }

        float GetIncline() const              { return _incline.get(this); }
        void SetIncline(float incline)        { _incline.set(this, incline); }

        float GetDistance() const             { return _distance.get(this); }
        void SetDistance(float distance)      { _distance.set(this, distance); }

        float GetSpinRate() const             { return _spinRate.get(this); }
        void SetSpinRate(float rate)          { _spinRate.set(this, rate); }

        float GetBobAmp() const               { return _bobAmp.get(this); }
        void SetBobAmp(float amp)             { _bobAmp.set(this, amp); }

        float GetBobFreq() const              { return _bobFreq.get(this); }
        void SetBobFreq(float freq)           { _bobFreq.set(this, freq); }

        //lua
        static void BindLua(LuaState L) {
            LuaBinding(L).beginExtendClass<asViewCS, asLinearCS>("asViewCS")
                .addProperty("Mode", &GetMode, &SetMode)
                .addProperty("Incline", &GetIncline, &SetDistance)
                .addProperty("Distance", &GetDistance, &SetIncline)
                .addProperty("SpinRate", &GetSpinRate, &SetSpinRate)
                .addProperty("BobAmp", &GetBobAmp, &SetBobAmp)
                .addProperty("BobFreq", &GetBobFreq, &SetBobFreq)
                .endClass();
        }
    };
    ASSERT_SIZEOF(asViewCS, 0x11C);
}