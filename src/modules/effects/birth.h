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
    private:
        Vector3 m_Position;
        Vector3 m_PositionVar;
        Vector3 m_Velocity;
        Vector3 m_VelocityVar;
        float m_Life;
        float m_LifeVar;
        float m_Mass;
        float m_MassVar;
        float m_Radius;
        float m_RadiusVar;
        float m_DRadius;
        float m_DRadiusVar;
        float m_Drag;
        float m_DragVar;
        float m_Damp;
        float m_DampVar;
        float m_SpewRate;
        float m_SpewTimeLimit;
        float m_Gravity;
        float m_Height;
        float m_Intensity;
        ColorARGB m_Color;
        int m_DAlpha;
        int m_DAlphaVar;
        int m_DRotation;
        int m_DRotationVar;
        int m_TexFrameStart;
        int m_TexFrameEnd;
        int m_InitialBlast;
        int m_BirthFlags;
        void(__cdecl* OnSparkAdded)(struct asSparkInfo*, struct asSparkPos*);
    public:
        AGE_API asBirthRule(void);
        AGE_API virtual ~asBirthRule(void);

        //overrides
        AGE_API char* GetClassName() override;
        AGE_API const char* GetDirName() override;
        AGE_API void FileIO(datParser& parser) override;

        //properties
        Vector3 GetPosition() const;
        void SetPosition(Vector3 position);
        Vector3 GetPositionVar() const;
        void SetPositionVar(Vector3 positionVar);
        Vector3 GetVelocity() const;
        void SetVelocity(Vector3 velocity);
        Vector3 GetVelocityVar() const;
        void SetVelocityVar(Vector3 velocityVar);
        float GetLife() const;
        void SetLife(float life);
        float GetLifeVar() const;
        void SetLifeVar(float lifeVar);
        float GetMass() const;
        void SetMass(float mass);
        float GetMassVar() const;
        void SetMassVar(float massVar);
        float GetRadius() const;
        void SetRadius(float radius);
        float GetRadiusVar() const;
        void SetRadiusVar(float radiusVar);
        float GetDRadius() const;
        void SetDRadius(float dRadius);
        float GetDRadiusVar() const;
        void SetDRadiusVar(float dRadiusVar);
        float GetDrag() const;
        void SetDrag(float drag);
        float GetDragVar() const;
        void SetDragVar(float dragVar);
        float GetDamp() const;
        void SetDamp(float damp);
        float GetDampVar() const;
        void SetDampVar(float dampVar);
        int GetDAlpha() const;
        void SetDAlpha(int dAlpha);
        int GetDAlphaVar() const;
        void SetDAlphaVar(int dAlphaVar);
        int GetDRotation() const;
        void SetDRotation(float dRotation);
        int GetDRotationVar() const;
        void SetDRotationVar(float dRotationVar);
        float GetSpewRate() const;
        void SetSpewRate(float rate);
        float GetSpewTimeLimit() const;
        void SetSpewTimeLimit(float timeLimit);
        float GetGravity() const;
        void SetGravity(float gravity);
        float GetHeight() const;
        void SetHeight(float height);
        float GetIntensity() const;
        void SetIntensity(float intensity);
        int GetTexFrameStart() const;
        void SetTexFrameStart(int frame);
        int GetTexFrameEnd() const;
        void SetTexFrameEnd(int frame);
        int GetBirthFlags() const;
        void SetBirthFlags(int flags);
        int GetInitialBlast() const;
        void SetInitialBlast(int initialBlast);
        void SetTexFrame(int start, int end);

        //helpers
        inline std::tuple<byte, byte, byte, byte> getColorTuple(void);
        inline void setColorTuple(std::tuple<byte, byte, byte, byte> setColor);

        //lua
        static void BindLua(LuaState L);
    };

    ASSERT_SIZEOF(asBirthRule, 0xB4);

    // Lua initialization

}