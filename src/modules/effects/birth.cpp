#include "birth.h";
using namespace MM2;

// asMeshCardVertex
void asMeshCardVertex::BindLua(LuaState L) 
{
    LuaBinding(L).beginClass<asMeshCardVertex>("asMeshCardVertex")
        .addVariableRef("x", &asMeshCardVertex::x)
        .addVariableRef("y", &asMeshCardVertex::y)
        .addVariableRef("z", &asMeshCardVertex::z)
        .addVariableRef("w", &asMeshCardVertex::w)
        .endClass();
}

// asMeshCardInfo
void asMeshCardInfo::Draw(asSparkPos* sparks, int sparkCount)
{
    auto currentCullMode = gfxRenderState::GetCullMode();
    auto camMatrix = gfxRenderState::GetCameraMatrix();

    for (int i = 0; i < sparkCount; ++i)
    {
        asSparkPos* spark = &sparks[i];

        Vector2* positions = &this->Positions[4 * ((this->PositionsIndex - 1) & spark->OffsetMask)];
        Vector2* texcoords = &this->TexCoords[4 * spark->TexCoordOffset];

        vglCurrentColor = spark->Color;

        vglBegin(gfxDrawMode::DRAWMODE_TRIANGLEFAN, 0);
        for (int j = 0; j < this->Count; ++j)
        {
            int aj = currentCullMode != D3DCULL::D3DCULL_CW ? this->Count - j - 1 : j;

            vglTexCoord2f(texcoords[aj].X, texcoords[aj].Y);

            float scaleX = spark->Scale * positions[aj].X;
            float scaleY = spark->Scale * positions[aj].Y;

            float z = camMatrix.m12 * scaleY + camMatrix.m02 * scaleX + spark->Pos.Z;
            float y = camMatrix.m11 * scaleY + camMatrix.m01 * scaleX + spark->Pos.Y;
            float x = camMatrix.m10 * scaleY + camMatrix.m00 * scaleX + spark->Pos.X;

            vglVertex(x, y, z);
        }
        vglEnd();
    }
}

// asBirthRule
AGE_API asBirthRule::asBirthRule(void) 
{
    scoped_vtable x(this);
    hook::Thunk<0x45ECE0>::Call<void>(this);
}

AGE_API asBirthRule::~asBirthRule(void) 
{
    scoped_vtable x(this);
    hook::Thunk<0x45FBF0>::Call<void>(this);
}

//overrides
AGE_API char* asBirthRule::GetClassName() 
{
    return hook::Thunk<0x45FC20>::Call<char*>(this); 
}

AGE_API const char* asBirthRule::GetDirName() 
{
    return hook::Thunk<0x45EDB0>::Call<const char*>(this);
}

AGE_API void asBirthRule::FileIO(datParser& parser) 
{
    hook::Thunk<0x45F900>::Call<void>(this, &parser); 
}

//helpers
inline std::tuple<byte, byte, byte, byte> asBirthRule::getColorTuple(void) {
    return std::make_tuple(Color.r, Color.g, Color.b, Color.a);
}

inline void asBirthRule::setColorTuple(std::tuple<byte, byte, byte, byte> setColor) {
    auto myColor = &this->Color;
    myColor->a = std::get<3>(setColor);
    myColor->r = std::get<0>(setColor);
    myColor->g = std::get<1>(setColor);
    myColor->b = std::get<2>(setColor);
}

//lua
void asBirthRule::BindLua(LuaState L) 
{
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