#pragma once
#include <modules\node.h>
#include <modules\rgl.h>

namespace MM2
{
    // Forward declarations
    class asBirthRule;
    struct asMeshCardVertex;
    struct asSparkPos;
    struct asMeshCardInfo;

    // External declarations


    // Class definitions
    struct asMeshCardVertex {
        float x;
        float y;
        float z;
        float w;

        static void BindLua(LuaState L);
    };

    struct asSparkPos
    {
        byte TexCoordOffset;
        byte OffsetMask;
        byte SomeFlag;
        float Scale;
        uint Color;
        float ShadowHeight;
        Vector3 Pos;
    };

    struct asMeshCardInfo
    {
        int Count;
        uint PositionsIndex;
        Vector2 *Positions;
        Vector2 *TexCoords;

        void Draw(asSparkPos* sparks, int sparkCount);
    };

    class asBirthRule : public asNode
    {
    public:
        Vector3 Position;
        Vector3 PositionVar;
        Vector3 Velocity;
        Vector3 VelocityVar;
        float Life;
        float LifeVar;
        float Mass;
        float MassVar;
        float Radius;
        float RadiusVar;
        float DRadius;
        float DRadiusVar;
        float Drag;
        float DragVar;
        float Damp;
        float DampVar;
        float SpewRate;
        float SpewTimeLimit;
        float Gravity;
        float Height;
        float Intensity;
        ColorARGB Color;
        int DAlpha;
        int DAlphaVar;
        int DRotation;
        int DRotationVar;
        int TexFrameStart;
        int TexFrameEnd;
        int InitialBlast;
        int BirthFlags;
        void(__cdecl* OnSparkAdded)(struct asSparkInfo*, struct asSparkPos*);
    public:
        AGE_API asBirthRule(void);
        AGE_API virtual ~asBirthRule(void);

        //overrides
        AGE_API char* GetClassName() override;
        AGE_API const char* GetDirName() override;
        AGE_API void FileIO(datParser& parser) override;

        //helpers
        inline std::tuple<byte, byte, byte, byte> getColorTuple(void);
        inline void setColorTuple(std::tuple<byte, byte, byte, byte> setColor);

        //lua
        static void BindLua(LuaState L);
    };

    ASSERT_SIZEOF(asBirthRule, 0xB4);

    // Lua initialization

}