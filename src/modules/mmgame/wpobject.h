#pragma once
#include "..\node\node.h"
#include "wpinst.h"

namespace MM2
{
    // Forward declarations
    class mmWaypointObject;

    // Class definitions
    enum class mmWaypointObjectType : int
    {
        Checkpoint,
        Billboard,
        Powerup
    };

    class mmWaypointObject : public asNode {
    private:
        BOOL m_Cleared;
        mmWaypointObjectType Type;
        int HitFlag;
        int HitID;
        int HitRoom;
        mmWaypointInstance* lvlInstancePtr;
        Vector2 RightGatePt;
        Vector2 LeftGatePt;
        Vector3 Position;
        float Heading;
        int HeadingType;
        float Radius;
        float yOffset;
    private:
        bool radiusHitLua(Vector3 pos);
        bool planeHitLua(Matrix34 vehMatrix, Vector2 linePtA, Vector2 linePtB, Vector3 vehSize);
        void setHitFlagLua(bool flag);
    public:
        mmWaypointObject() = delete;
        mmWaypointObject(Vector4 const& positionAndHeading, const char* modelName, int id, float radius, mmWaypointObjectType type, float yOffset);
        ~mmWaypointObject();

        /*
            asNode virtuals
        */
        virtual AGE_API void Update() override;
        virtual AGE_API void Reset() override;

        /*
            mmWaypointObject
        */
        AGE_API void Activate();
        AGE_API void Deactivate();
        AGE_API BOOL RadiusHit(Vector3 pos);
        AGE_API BOOL PlaneHit(Matrix34 vehMatrix, Vector2 linePtA, Vector2 linePtB, Vector3 vehSize);
        AGE_API void SetHitFlag(BOOL flag);
        AGE_API void SetRadius(float radius);
        AGE_API void SetPos(Vector3 pos);
        AGE_API void SetHeading(float heading);
        AGE_API void SetHeadingType(int type);
        AGE_API void Move();
        AGE_API void CalculateGatePoints();

        void Clear();
        bool Cleared() const;
        int GetHitIDMask() const;

        static void BindLua(LuaState L);
    };
    ASSERT_SIZEOF(mmWaypointObject, 0x5C);
}