#include "birth.h"
using namespace MM2;

// asMeshCardVertex
void asMeshCardVertex::BindLua(LuaState L) 
{
    LuaBinding(L).beginClass<asMeshCardVertex>("asMeshCardVertex")
        .addVariable("x", &asMeshCardVertex::x)
        .addVariable("y", &asMeshCardVertex::y)
        .addVariable("z", &asMeshCardVertex::z)
        .addVariable("w", &asMeshCardVertex::w)
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

//properties
Vector3 asBirthRule::GetPosition() const { return this->m_Position; }
void asBirthRule::SetPosition(Vector3 position) { this->m_Position = position; }

Vector3 asBirthRule::GetPositionVar() const { return this->m_PositionVar; }
void asBirthRule::SetPositionVar(Vector3 positionVar) { this->m_PositionVar = positionVar; }

Vector3 asBirthRule::GetVelocity() const { return this->m_Velocity; }
void asBirthRule::SetVelocity(Vector3 velocity) { this->m_Velocity = velocity; }

Vector3 asBirthRule::GetVelocityVar() const { return this->m_VelocityVar; }
void asBirthRule::SetVelocityVar(Vector3 velocityVar) { this->m_VelocityVar = velocityVar; }

float asBirthRule::GetLife() const { return this->m_Life; }
void asBirthRule::SetLife(float life) { this->m_Life = life; }

float asBirthRule::GetLifeVar() const { return this->m_LifeVar; }
void asBirthRule::SetLifeVar(float lifeVar) { this->m_LifeVar = lifeVar; }

float asBirthRule::GetMass() const { return this->m_Mass; }
void asBirthRule::SetMass(float mass) { this->m_Mass = mass; }

float asBirthRule::GetMassVar() const { return this->m_MassVar; }
void asBirthRule::SetMassVar(float massVar) { this->m_MassVar = massVar; }

float asBirthRule::GetRadius() const { return this->m_Radius; }
void asBirthRule::SetRadius(float radius) { this->m_Radius = radius; }

float asBirthRule::GetRadiusVar() const { return this->m_RadiusVar; }
void asBirthRule::SetRadiusVar(float radiusVar) { this->m_RadiusVar = radiusVar; }

float asBirthRule::GetDRadius() const { return this->m_DRadius; }
void asBirthRule::SetDRadius(float dRadius) { this->m_DRadius = dRadius; }

float asBirthRule::GetDRadiusVar() const { return this->m_DRadiusVar; }
void asBirthRule::SetDRadiusVar(float dRadiusVar) { this->m_DRadiusVar = dRadiusVar; }

float asBirthRule::GetDrag() const { return this->m_Drag; }
void asBirthRule::SetDrag(float drag) { this->m_Drag = drag; }

float asBirthRule::GetDragVar() const { return this->m_DragVar; }
void asBirthRule::SetDragVar(float dragVar) { this->m_DragVar = dragVar; }

float asBirthRule::GetDamp() const { return this->m_Damp; }
void asBirthRule::SetDamp(float damp) { this->m_Damp = damp; }

float asBirthRule::GetDampVar() const { return this->m_DampVar; }
void asBirthRule::SetDampVar(float dampVar) { this->m_DampVar = dampVar; }

int asBirthRule::GetDAlpha() const { return this->m_DAlpha; }
void asBirthRule::SetDAlpha(int dAlpha) { this->m_DAlpha = dAlpha; }

int asBirthRule::GetDAlphaVar() const { return this->m_DAlphaVar; }
void asBirthRule::SetDAlphaVar(int dAlphaVar) { this->m_DAlphaVar = dAlphaVar; }

int asBirthRule::GetDRotation() const { return this->m_DRotation; }
void asBirthRule::SetDRotation(int dRotation) { this->m_DRotation = dRotation; }

int asBirthRule::GetDRotationVar() const { return this->m_DRotationVar; }
void asBirthRule::SetDRotationVar(int dRotationVar) { this->m_DRotationVar = dRotationVar; }

void asBirthRule::SetTexFrame(int start, int end) {
    this->m_TexFrameStart = start;
    this->m_TexFrameEnd = end;
}

float asBirthRule::GetSpewRate() const { return this->m_SpewRate; }
void asBirthRule::SetSpewRate(float rate) { this->m_SpewRate = rate; }

float asBirthRule::GetSpewTimeLimit() const { return this->m_SpewTimeLimit; }
void asBirthRule::SetSpewTimeLimit(float timeLimit) { this->m_SpewTimeLimit = timeLimit; }

float asBirthRule::GetGravity() const { return this->m_Gravity; }
void asBirthRule::SetGravity(float gravity) { this->m_Gravity = gravity; }

float asBirthRule::GetHeight() const { return this->m_Height; }
void asBirthRule::SetHeight(float height) { this->m_Height = height; }

float asBirthRule::GetIntensity() const { return this->m_Intensity; }
void asBirthRule::SetIntensity(float intensity) { this->m_Intensity = intensity; }

int asBirthRule::GetTexFrameStart() const { return this->m_TexFrameStart; }
void asBirthRule::SetTexFrameStart(int frame) { this->m_TexFrameStart = frame; }

int asBirthRule::GetTexFrameEnd() const { return this->m_TexFrameEnd; }
void asBirthRule::SetTexFrameEnd(int frame) { this->m_TexFrameEnd = frame; }

int asBirthRule::GetBirthFlags() const { return this->m_BirthFlags; }
void asBirthRule::SetBirthFlags(int flags) { this->m_BirthFlags = flags; }

int asBirthRule::GetInitialBlast() const { return this->m_InitialBlast; }
void asBirthRule::SetInitialBlast(int initialBlast) { this->m_InitialBlast = initialBlast; }

void MM2::asBirthRule::SetColor(const Vector4 & color) { this->m_Color = color.PackColorRGBA(); }
Vector4 MM2::asBirthRule::GetColor() {
    Vector4 vec;
    vec.UnpackColorRGBA(this->m_Color);
    return vec;
}

//lua
void asBirthRule::BindLua(LuaState L) 
{
    LuaBinding(L).beginExtendClass<asBirthRule, asNode>("asBirthRule")
        .addFactory([]() { return new asBirthRule(); })
        .addProperty("Position", &GetPosition, &SetPosition)
        .addProperty("PositionVar", &GetPositionVar, &SetPositionVar)
        .addProperty("Velocity", &GetVelocity, &SetVelocity)
        .addProperty("VelocityVar", &GetVelocityVar, &SetVelocityVar)
        .addProperty("Life", &GetLife, &SetLife)
        .addProperty("LifeVar", &GetLifeVar, &SetLifeVar)
        .addProperty("Mass", &GetMass, &SetMass)
        .addProperty("MassVar", &GetMassVar, &SetMassVar)
        .addProperty("Radius",  &GetRadius, &SetRadius)
        .addProperty("RadiusVar", &GetRadiusVar, &SetRadiusVar)
        .addProperty("DRadius", &GetDRadius, &SetDRadius)
        .addProperty("DRadiusVar", &GetDRadiusVar, &SetDRadiusVar)
        .addProperty("Drag", &GetDrag, &SetDrag)
        .addProperty("DragVar", &GetDragVar, &SetDragVar)
        .addProperty("Damp", &GetDamp, &SetDamp)
        .addProperty("DampVar", &GetDampVar, &SetDampVar)
        .addProperty("SpewRate", &GetSpewRate, &SetSpewRate)
        .addProperty("SpewTimeLimit", &GetSpewTimeLimit, &SetSpewTimeLimit)
        .addProperty("Gravity", &GetGravity, &SetGravity)
        .addProperty("Height", &GetHeight, &SetHeight)
        .addProperty("Intensity", &GetIntensity, &SetIntensity)
        .addProperty("Color", &GetColor, &SetColor)
        .addProperty("DAlpha", &GetDAlpha, &SetDAlpha)
        .addProperty("DAlphaVar", &GetDAlphaVar, &SetDAlphaVar)
        .addProperty("DRotation", &GetDRotation, &SetDRotation)
        .addProperty("DRotationVar", &GetDRotationVar, &SetDRotationVar)
        .addProperty("TexFrameStart", &GetTexFrameStart, &SetTexFrameStart)
        .addProperty("TexFrameEnd", &GetTexFrameEnd, &SetTexFrameEnd)
        .addProperty("InitialBlast", &GetInitialBlast, &SetInitialBlast)
        .addProperty("BirthFlags", &GetBirthFlags, &SetBirthFlags)
        .endClass();
}