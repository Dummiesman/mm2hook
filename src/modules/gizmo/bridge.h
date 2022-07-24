#pragma once
#include <mm2_common.h>
#include <modules\banger\banger.h>

namespace MM2
{
    // Forward declarations
    class gizBridge;

    // External declarations

    // Class definitions
    enum gizBridgeType {
        Inactive = 0x0,
        Proximity = 0x1,
        Timed = 0x2,
        AlwaysOpen = 0x3
    };

    class gizBridge : public dgUnhitMtxBangerInstance {
    private:
        byte buffer[0xC4-2];
        hook::Field<0x58, int> _state;
        hook::Field<0x5C, int> _type;
    private:
        static hook::Type<float> LiftSpeed;
        static hook::Type<float> GoalAngle;
        static hook::Type<float> DownInterval;
        static hook::Type<float> UpInterval;
        static hook::Type<float> ProximityDist2;
    public:
        AGE_API gizBridge();
        AGE_API ~gizBridge();
        
        /*
            lvlInstance virtuals
        */
        AGE_API virtual void Reset() override;
        AGE_API virtual void Draw(int lod) override;
        AGE_API virtual unsigned int SizeOf() override;

        /*
            gizBridge
        */
        AGE_API void Init(LPCSTR name, Matrix34 const& matrix);

        int GetState() const;

        gizBridgeType GetType() const;
        void SetType(gizBridgeType type);

        static float GetLiftSpeed();
        static void SetLiftSpeed(float speed);

        static float GetGoalAngle();
        static void SetGoalAngle(float angle);

        static float GetDownInterval();
        static void SetDownInterval(float interval);

        static float GetUpInterval();
        static void SetUpInterval(float interval);

        static float GetProximityDist();
        static void SetProximityDist(float dist);

        static void BindLua(LuaState L);
    };

    ASSERT_SIZEOF(gizBridge, 0x11C);
}