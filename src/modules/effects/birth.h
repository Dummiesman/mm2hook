#pragma once
#include <modules\node\node.h>
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

        static void BindLua(LuaState L) {
            LuaBinding(L).beginClass<asMeshCardVertex>("asMeshCardVertex")
                .addVariableRef("x", &asMeshCardVertex::x)
                .addVariableRef("y", &asMeshCardVertex::y)
                .addVariableRef("z", &asMeshCardVertex::z)
                .addVariableRef("w", &asMeshCardVertex::w)
                .endClass();
        }
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

        void asMeshCardInfo::Draw(asSparkPos *sparks, int sparkCount)
        {
            byte currentCullMode = *(byte*)0x6856BB;

            for (int i = 0; i < sparkCount; ++i)
            {
                asSparkPos* spark = &sparks[i];

                Vector2* positions = &this->Positions[4 * ((this->PositionsIndex - 1) & spark->OffsetMask)];
                Vector2* texcoords = &this->TexCoords[4 * spark->TexCoordOffset];

                vglCurrentColor = spark->Color;

                vglBegin(gfxDrawMode::DRAWMODE_TRIANGLEFAN, 0);
                for (int j = 0; j < this->Count; ++j)
                {
                    int aj = currentCullMode != 2 ? this->Count - j - 1 : j;

                    vglTexCoord2f(texcoords[aj].X, texcoords[aj].Y);

                    float scaleX = spark->Scale * positions[aj].X;
                    float scaleY = spark->Scale * positions[aj].Y;

                    float z = gfxRenderState::sm_Camera->m23 * scaleY + gfxRenderState::sm_Camera->m13 * scaleX + spark->Pos.Z;
                    float y = gfxRenderState::sm_Camera->m22 * scaleY + gfxRenderState::sm_Camera->m12 * scaleX + spark->Pos.Y;
                    float x = gfxRenderState::sm_Camera->m21 * scaleY + gfxRenderState::sm_Camera->m11 * scaleX + spark->Pos.X;

                    vglVertex(x, y, z);
                }
                vglEnd();
            }
        }
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
        AGE_API asBirthRule(void) {
            scoped_vtable x(this);
            hook::Thunk<0x45ECE0>::Call<void>(this);
        }

        AGE_API virtual ~asBirthRule(void) {
            scoped_vtable x(this);
            hook::Thunk<0x45FBF0>::Call<void>(this);
        }

        //overrides
        AGE_API char* GetClassName() override           { return hook::Thunk<0x45FC20>::Call<char*>(this); }
        AGE_API const char* GetDirName() override       { return hook::Thunk<0x45EDB0>::Call<const char*>(this); }
        AGE_API void FileIO(datParser &parser) override { hook::Thunk<0x45F900>::Call<void>(this, &parser); }

        //helpers
        inline std::tuple<byte, byte, byte, byte> getColorTuple(void) {
            return std::make_tuple(Color.r, Color.g, Color.b, Color.a);
        }

        inline void setColorTuple(std::tuple<byte, byte, byte, byte> setColor) {
            auto myColor = &this->Color;
            myColor->a = std::get<3>(setColor);
            myColor->r = std::get<0>(setColor);
            myColor->g = std::get<1>(setColor);
            myColor->b = std::get<2>(setColor);
        }

        //lua
        static void BindLua(LuaState L) {
            LuaBinding(L).beginExtendClass<asBirthRule, asNode>("asBirthRule")
                .addConstructor(LUA_ARGS())
                .addVariableRef("Position", &asBirthRule::Position)
                .addVariableRef("PositionVar", &asBirthRule::PositionVar)
                .addVariableRef("Velocity", &asBirthRule::Velocity)
                .addVariableRef("VelocityVar", &asBirthRule::VelocityVar)
                .addVariableRef("Life", &asBirthRule::Life)
                .addVariableRef("LifeVar", &asBirthRule::LifeVar)
                .addVariableRef("Mass", &asBirthRule::Mass)
                .addVariableRef("MassVar", &asBirthRule::MassVar)
                .addVariableRef("Radius", &asBirthRule::Radius)
                .addVariableRef("RadiusVar", &asBirthRule::RadiusVar)
                .addVariableRef("DRadius", &asBirthRule::DRadius)
                .addVariableRef("DRadiusVar", &asBirthRule::DRadiusVar)
                .addVariableRef("Drag", &asBirthRule::Drag)
                .addVariableRef("DragVar", &asBirthRule::DragVar)
                .addVariableRef("Damp", &asBirthRule::Damp)
                .addVariableRef("DampVar", &asBirthRule::DampVar)
                .addVariableRef("SpewRate", &asBirthRule::SpewRate)
                .addVariableRef("SpewTimeLimit", &asBirthRule::SpewTimeLimit)
                .addVariableRef("Gravity", &asBirthRule::Gravity)
                .addVariableRef("Height", &asBirthRule::Height)
                .addVariableRef("Intensity", &asBirthRule::Intensity)
                .addProperty("Color", &getColorTuple, &setColorTuple)
                .addVariableRef("DAlpha", &asBirthRule::DAlpha)
                .addVariableRef("DAlphaVar", &asBirthRule::DAlphaVar)
                .addVariableRef("DRotation", &asBirthRule::DRotation)
                .addVariableRef("DRotationVar", &asBirthRule::DRotationVar)
                .addVariableRef("TexFrameStart", &asBirthRule::TexFrameStart)
                .addVariableRef("TexFrameEnd", &asBirthRule::TexFrameEnd)
                .addVariableRef("InitialBlast", &asBirthRule::InitialBlast)
                .addVariableRef("BirthFlags", &asBirthRule::BirthFlags)
                .endClass();
        }
    };

    ASSERT_SIZEOF(asBirthRule, 0xB4);

    // Lua initialization

}