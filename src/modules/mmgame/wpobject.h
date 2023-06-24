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
        int dword_18;
        mmWaypointObjectType Type;
        int HitFlag;
        int ID;
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
    public:
        ANGEL_ALLOCATOR

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
        AGE_API void SetRadius(float radius);
        AGE_API void SetPos(Vector3 pos);
        AGE_API void SetHeading(float heading);
        AGE_API void SetHeadingType(int type);
        AGE_API void Move();

        static void BindLua(LuaState L);
    };
    ASSERT_SIZEOF(mmWaypointObject, 0x5C);
}