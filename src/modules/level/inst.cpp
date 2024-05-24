#pragma once
#include "inst.h"
#include <modules\data\hash.h>
#include <modules\model\package.h>
#include <modules\model\shader.h>
#include <modules\phys\bound.h>
#include <modules\phys\physmgr.h>

using namespace MM2;

extern class lvlFixedAny;
extern class lvlFixedRotY;
extern class lvlFixedMatrix;
extern class lvlLandmark;
extern class aiTrafficLightInstance;
extern class dgBangerInstance;
extern class dgUnhitYBangerInstance;
extern class dgUnhitMtxBangerInstance;
extern class gizBridge;
extern class vehCarModel;
extern class vehTrailerInstance;

extern class phBoundGeometry;
extern class phBoundSphere;
extern class phBoundHotdog;
extern class phBoundBox;
extern class phForceSphere;
extern class phBoundTerrain;
extern class phBoundTerrainLocal;

/*
    GeomTableEntry
*/
modStatic* lvlInstance::GeomTableEntry::GetLOD(int lod) const {
    if (lod < 0 || lod > 3)
        return nullptr;

    return this->LOD[lod];
}

modStatic* lvlInstance::GeomTableEntry::GetHighestLOD() const {
    for (int i = 3; i >= 0; i--) {
        if (this->LOD[i] != nullptr)
            return this->LOD[i];
    }
    return nullptr;
}

modStatic* lvlInstance::GeomTableEntry::GetLowestLOD() const {
    for (int i = 0; i < 4; i++) {
        if (this->LOD[i] != nullptr)
            return this->LOD[i];
    }
    return nullptr;
}

modStatic* lvlInstance::GeomTableEntry::GetVeryLowLOD() const {
    return this->LOD[0];
}

modStatic* lvlInstance::GeomTableEntry::GetLowLOD() const {
    return this->LOD[1];
}

modStatic* lvlInstance::GeomTableEntry::GetMedLOD() const {
    return this->LOD[2];
}

modStatic* lvlInstance::GeomTableEntry::GetHighLOD() const {
    return this->LOD[3];
}

float lvlInstance::GeomTableEntry::GetRadius() const {
    return Radius;
}

phBound* lvlInstance::GeomTableEntry::GetBound() const {
    return this->Bound;
}

void lvlInstance::GeomTableEntry::BindLua(LuaState L) {
    LuaBinding(L).beginClass<GeomTableEntry>("GeomTableEntry")
        //fields
        .addPropertyReadOnly("VL", &GetVeryLowLOD)
        .addPropertyReadOnly("L", &GetLowLOD)
        .addPropertyReadOnly("M", &GetMedLOD)
        .addPropertyReadOnly("H", &GetHighLOD)
        .addPropertyReadOnly("Radius", &GetRadius)
        .addPropertyReadOnly("Bound", &GetBound)
        .addFunction("GetLOD", &GetLOD)
        .addFunction("GetHighestLOD", &GetHighestLOD)
        .addFunction("GetLowestLOD", &GetLowestLOD)
        .endClass();
}

/*
    lvlInstance
*/

int lvlInstance::castLua(lua_State* L)
{
    auto vtblPtr = *reinterpret_cast<uintptr_t*>(this);
    switch (vtblPtr) {
        case 0x5B150C: //dgUnhitYBangerInstance
            LuaState(L).push(reinterpret_cast<dgUnhitYBangerInstance*>(this));
            break;
        case 0x5B1494: //dgUnhitMtxBangerInstance
            LuaState(L).push(reinterpret_cast<dgUnhitMtxBangerInstance*>(this));
            break;
        case 0x5B54AC: //aiTrafficLightInstance
            LuaState(L).push(reinterpret_cast<aiTrafficLightInstance*>(this));
            break;
        case 0x5B5F8C: //gizBridge
            LuaState(L).push(reinterpret_cast<gizBridge*>(this));
            break;
        case 0x5B15B8: //dgHitBangerInstance
            LuaState(L).push(reinterpret_cast<dgBangerInstance*>(this));
            break;
        case 0x5B2CB0: //vehCarModel
            LuaState(L).push(reinterpret_cast<vehCarModel*>(this));
            break;
        case 0x5B2F84: //vehTrailerInstance
            LuaState(L).push(reinterpret_cast<vehTrailerInstance*>(this));
            break;
        case 0x5B1B10: //lvlFixedAny
            LuaState(L).push(reinterpret_cast<lvlFixedAny*>(this));
            break;
        case 0x5B1B78: //lvlFixedRotY
            LuaState(L).push(reinterpret_cast<lvlFixedRotY*>(this));
            break;
        case 0x5B1BE0: //lvlFixedMatrix
            LuaState(L).push(reinterpret_cast<lvlFixedMatrix*>(this));
            break;
        case 0x5B1A88: //lvlLandmark
            LuaState(L).push(reinterpret_cast<lvlLandmark*>(this));
            break;
        case 0x5B6180: //gizFerry
        case 0x5B60D4: //gizTrainCar
        default:
            LuaState(L).push(this);
            break;
    }
    return 1;
}

int lvlInstance::getBoundLua(lua_State* L, int a1)
{
    auto bound = this->GetBound(a1);
    if (bound == nullptr) {
        LuaState(L).push(nullptr);
        return 1;
    }

    switch (bound->GetType()) 
    {
    case phBound::BoundType::Box:
        LuaState(L).push(reinterpret_cast<phBoundBox*>(bound));
        break;
    case phBound::BoundType::Geometry:
        LuaState(L).push(reinterpret_cast<phBoundGeometry*>(bound));
        break;
    case phBound::BoundType::Hotdog:
        LuaState(L).push(reinterpret_cast<phBoundHotdog*>(bound));
        break;
    case phBound::BoundType::Level:
        LuaState(L).push(nullptr);
        break;
    case phBound::BoundType::ForceSphere:
        LuaState(L).push(reinterpret_cast<phForceSphere*>(bound));
        break;
    case phBound::BoundType::Sphere:
        LuaState(L).push(reinterpret_cast<phBoundSphere*>(bound));
        break;
    case phBound::BoundType::Terrain:
        LuaState(L).push(reinterpret_cast<phBoundTerrain*>(bound));
        break;
    case phBound::BoundType::TerrainLocal:
        LuaState(L).push(reinterpret_cast<phBoundTerrainLocal*>(bound));
        break;
    default:
        LuaState(L).push(bound);
        break;
    }
    return 1;
}

AGE_API int lvlInstance::GetGeomSet(char const * a1, char const * a2, int a3) { return hook::StaticThunk<0x4632C0>::Call<int>(a1, a2, a3); }
AGE_API void lvlInstance::CreateTempBounds() { hook::StaticThunk<0x464680>::Call<void>(); }
AGE_API void lvlInstance::DeleteTempBounds() { hook::StaticThunk<0x4647E0>::Call<void>(); }
    


lvlInstance::GeomTableEntry* lvlInstance::GetGeomTablePtr() {
    return reinterpret_cast<GeomTableEntry*>(0x6316D8);
}

const char** lvlInstance::GetGeomNameTablePtr() {
    return reinterpret_cast<const char**>(0x651760);
}

int lvlInstance::GetGeomSetCount() {
    return *reinterpret_cast<int*>(0x655764);
}
    
Matrix34 lvlInstance::getMatrixLua()
{
    Matrix34 discard;
    auto matrix = this->GetMatrix(&discard);
    discard.Set(matrix);
    return discard;
}

Vector3 lvlInstance::getPositionLua()
{
    auto position = this->GetPosition();
    return Vector3(position);
}

void lvlInstance::setPositionLua(Vector3 position)
{
    auto matrix = getMatrixLua();
    matrix.SetRow(3, position);
    this->SetMatrix(matrix);
}

modShader* lvlInstance::getShaderLua(int variant, int index)
{
    if (variant < 0 || variant >= this->GetVariantCount()) return nullptr;
    if (index < 0 || index >= this->GetShaderCount()) return nullptr;
    return &this->GetShader(variant)[index];
}

std::tuple<bool, Matrix34> lvlInstance::computeShadowMatrixLua(Matrix34 const& inMtx, int room)
{
    Matrix34 outMtx = Matrix34::I;
    bool res = ComputeShadowMatrix(outMtx, room, inMtx);
    return std::make_tuple(res, outMtx);
}

AGE_API lvlInstance::lvlInstance(void)
{
    scoped_vtable x(this);
    hook::Thunk<0x4631F0>::Call<void>(this);
}

AGE_API lvlInstance::~lvlInstance(void)
{
    scoped_vtable x(this);
    hook::Thunk<0x463220>::Call<void>(this);
}

inline lvlInstance* lvlInstance::GetNext() const
{
    return this->Next;
}

inline lvlInstance* lvlInstance::GetPrevious() const
{
    return this->Previous;
}

short lvlInstance::GetRoomId() const
{
    return room;
}

short lvlInstance::GetGeomIndex() const
{
    return GeomIndex;
}

void lvlInstance::SetGeomIndex(unsigned short id)
{
    GeomIndex = id;
}

short lvlInstance::GetFlags() const
{
    return this->Flags;
}

void lvlInstance::SetFlags(ushort flags)
{
    this->Flags = flags;
}

void lvlInstance::SetFlag(ushort flag)
{
    this->Flags |= flag;
}

byte lvlInstance::GetOwner(void) const {
    return this->Owner;
}

void lvlInstance::SetOwner(byte owner) {
    this->Owner = owner;
}

byte lvlInstance::GetSubType(void) const {
    return this->Subtype;
}

inline void lvlInstance::SetSubType(byte subtype) {
    this->Subtype = subtype;
}

lvlInstance::GeomTableEntry* lvlInstance::GetGeomBase(int geom) const {
    return this->GeomIndex == 0 ? nullptr : &lvlInstance::GetGeomTablePtr()[GeomIndex - 1 + geom];
}

modStatic* lvlInstance::GetGeom(int lod, int geom) const {
    return this->GeomIndex == 0 ? nullptr : (&lvlInstance::GetGeomTablePtr()[GeomIndex - 1 + geom])->GetLOD(lod);
}

const char* lvlInstance::GetName() const {
    return this->GeomIndex == 0 ? "(none)" : lvlInstance::GetGeomNameTablePtr()[GeomIndex - 1];
}

modShader* lvlInstance::GetShader(int index) const {
    if (GeomIndex == 0 || index >= this->GetVariantCount() || index < 0)
        return nullptr;
    return (&lvlInstance::GetGeomTablePtr()[GeomIndex - 1])->pShaders[index];
}

int lvlInstance::GetShaderCount() const {
    return this->GeomIndex == 0 ? 0 : (&lvlInstance::GetGeomTablePtr()[GeomIndex - 1])->numShadersPerVariant;
}

int lvlInstance::GetVariantCount() const {
    return this->GeomIndex == 0 ? 1 : (&lvlInstance::GetGeomTablePtr()[GeomIndex - 1])->numShaders;
}

AGE_API bool lvlInstance::ComputeShadowMatrix(Matrix34& outMatrix, int room, Matrix34 const& inMatrix)
{
    return ComputeShadowMatrix(outMatrix, room, inMatrix, nullptr);
}

AGE_API bool MM2::lvlInstance::ComputeShadowMatrix(Matrix34& outMatrix, int room, Matrix34 const& inMatrix, lvlInstance* ignoreInstance)
{
    auto physmgr = dgPhysManager::Instance.get();
    lvlIntersection isect = lvlIntersection();
    lvlSegment segment = lvlSegment();

    // Collide with ground +/- 1m
    Vector3 startPos = inMatrix.GetRow(3) + (Vector3::YAXIS * 1.0f);
    Vector3 endPos = inMatrix.GetRow(3) - (Vector3::YAXIS * 1.0f);
    segment.Set(startPos, endPos, 0xFFFFFFFF, nullptr);

    if (!physmgr->Collide(segment, &isect, room, ignoreInstance, lvlInstance::INST_WHEELCOLLISION))
    {
        // Try colliding with a larger range
        startPos = inMatrix.GetRow(3) + (Vector3::YAXIS * 1.0f);
        endPos = inMatrix.GetRow(3) - (Vector3::YAXIS * 10.0f); // Default is 5.0 but we need this to be larger for billboard shadowing purposes
        segment.Set(startPos, endPos, 0xFFFFFFFF, nullptr);

        if (!physmgr->Collide(segment, &isect, room, ignoreInstance, lvlInstance::INST_WHEELCOLLISION))
        {
            // Failed to find ground
            return false;
        }
    }

    // Check if ground was flat enough
    if (isect.IntersectionPoint.Normal.Y < 0.7f)
    {
        return false;
    }

    // Compute shadow matrix
    outMatrix.Set(inMatrix);
    if (inMatrix.m11 <= 0.0f)
    {
        Vector3 up = inMatrix.GetRow(1);
        up.Negate();
        outMatrix.RotateTo(up, isect.IntersectionPoint.Normal);

        up.Set(outMatrix.GetRow(1)); // Haha, upset
        up.Negate();
        outMatrix.SetRow(1, up);
    }
    else 
    {
        outMatrix.RotateTo(inMatrix.GetRow(1), isect.IntersectionPoint.Normal);
    }
    outMatrix.SetRow(3, isect.IntersectionPoint.Point);
    return true;
}

AGE_API bool MM2::lvlInstance::ComputeShadowProjectionMatrix(Matrix34& outMatrix, int room, float lightPitch, float lightHeading, Matrix34 const& inMatrix)
{
    return ComputeShadowProjectionMatrix(outMatrix, room, lightPitch, lightHeading, inMatrix, nullptr);
}

AGE_API bool lvlInstance::ComputeShadowProjectionMatrix(Matrix34 & outMatrix, int room, float lightPitch, float lightHeading, Matrix34 const& inMatrix, lvlInstance* ignoreInstance)
{
    if (!ComputeShadowMatrix(outMatrix, room, inMatrix, ignoreInstance))
        return false;

    Vector3 lightDirection;
    lightDirection.X = cosf(lightHeading) * cosf(lightPitch);
    lightDirection.Y = sinf(lightPitch);
    lightDirection.Z = sinf(lightHeading) * cosf(lightPitch);

    float angle = lightDirection.X * outMatrix.m10 + lightDirection.Z * outMatrix.m12;
    outMatrix.SetRow(1, Vector3(lightDirection.X, -angle, lightDirection.Z));

    float groundDist = inMatrix.m31 - outMatrix.m31;
    outMatrix.SetRow(3, outMatrix.GetRow(3) + outMatrix.GetRow(1) * groundDist);
    return true;
}

AGE_API void lvlInstance::ResetInstanceHeap()             { hook::StaticThunk<0x4631A0>::Call<void>(); }
AGE_API void lvlInstance::ResetAll()                      { hook::StaticThunk<0x4631E0>::Call<void>(); }
AGE_API void lvlInstance::SetShadowBillboardMtx(Matrix44 &a1)
                                                    { hook::StaticThunk<0x463290>::Call<void>(&a1); }

AGE_API int lvlInstance::AddSphere(float a1)              { return hook::StaticThunk<0x463D50>::Call<int>(a1); }

AGE_API bool lvlInstance::LoadBoundOnLastEntry(const char *a1)   { return hook::Thunk<0x463940>::Call<bool>(this, a1); }
AGE_API Vector4 * lvlInstance::GetBoundSphere(Vector4 *a1)       { return hook::Thunk<0x463A40>::Call<Vector4 *>(this, a1); }

AGE_API bool lvlInstance::BeginGeom(const char *a1, const char *a2, int a3)
                                                    { return hook::Thunk<0x463A80>::Call<bool>(this, a1, a2, a3); }
AGE_API int lvlInstance::AddGeom(const char *a1, const char *a2, int a3)
                                                    { return hook::Thunk<0x463BA0>::Call<int>(this, a1, a2, a3); }
AGE_API void lvlInstance::EndGeom()                              { hook::Thunk<0x463BC0>::Call<void>(this); }

AGE_API bool lvlInstance::InitBoundTerrain(const char *a1)       { return hook::Thunk<0x463DA0>::Call<bool>(this, a1); }
AGE_API bool lvlInstance::InitBoundTerrainLocal(const char *a1)  { return hook::Thunk<0x463F50>::Call<bool>(this, a1); }
AGE_API bool lvlInstance::NeedGhostBound(const Vector3 *a1, int a2)
                                                    { return hook::Thunk<0x4641A0>::Call<bool>(this, a1, a2); }
AGE_API bool lvlInstance::InitGhostBound(phBound *a1, const Vector3 *a2, int a3)
                                                    { return hook::Thunk<0x464200>::Call<bool>(this, a1, a2, a3); }
AGE_API int lvlInstance::InitGhost(const char *a1, const Matrix34 &a2)
                                                    { return hook::Thunk<0x464330>::Call<int>(this, a1, &a2); }

AGE_API void lvlInstance::PreLoadShader(int variant)             { hook::Thunk<0x464B00>::Call<void>(this, variant); }
AGE_API void lvlInstance::Optimize(int variant)                  { hook::Thunk<0x464B70>::Call<void>(this, variant); }

/*
    Custom additions
*/
bool lvlInstance::BeginGeomWithGroup(const char* a1, const char* a2, const char* group, int a3)
{
    string_buf<256> groupedName("%s_%s", a1, group);

    *(bool*)0x651720 = (a3 & 8) != 0; //set dontPreload
    *(const char**)0x6516DC = groupedName; //set lastName
    auto PackageHash = (HashTable*)0x651728;

    int existingGeomSet;
    if (PackageHash->Access(groupedName, &existingGeomSet))
    {
        this->GeomIndex = existingGeomSet;
    }
    else
    {
        auto package = new modPackage();
        *(modPackage**)0x0651740 = package; //set PKG

        if (package->Open("geometry", a1))
        {
            this->GeomIndex = GetGeomSet(a1, a2, a3);
            PackageHash->Insert(groupedName, (void*)this->GeomIndex);
            return true;
        }

        if (package != nullptr)
        {
            delete package;
        }

        *(modPackage**)0x0651740 = nullptr; //set PKG
        PackageHash->Insert(groupedName, (void*)0);
    }
    return false;
}

/*
    lvlInstance virtuals
*/

AGE_API void lvlInstance::Reset()                        { hook::Thunk<0x463280>::Call<void>(this); }
AGE_API int lvlInstance::IsVisible(const gfxViewport *a1)
                                                    { return hook::Thunk<0x4649F0>::Call<int>(this, a1); }
        
AGE_API void lvlInstance::SetVariant(int a1)             { hook::Thunk<0x4643D0>::Call<void>(this, a1); }
AGE_API const float lvlInstance::GetRadius()             { return hook::Thunk<0x4643E0>::Call<float>(this); }
AGE_API dgPhysEntity * lvlInstance::GetEntity()          { return hook::Thunk<0x4643B0>::Call<dgPhysEntity *>(this); }
AGE_API dgPhysEntity * lvlInstance::AttachEntity()       { return hook::Thunk<0x4643C0>::Call<dgPhysEntity *>(this); }
AGE_API const Vector3 & lvlInstance::GetVelocity()       { return hook::Thunk<0x4643A0>::Call<const Vector3 &>(this); }
AGE_API void lvlInstance::Detach()                       { hook::Thunk<0x43FC30>::Call<void>(this); }
        
AGE_API void lvlInstance::DrawShadow()                   { hook::Thunk<0x4643F0>::Call<void>(this); }
AGE_API void lvlInstance::DrawShadowMap()                { hook::Thunk<0x464400>::Call<void>(this); }
AGE_API void lvlInstance::DrawGlow()                     { hook::Thunk<0x464410>::Call<void>(this); }
AGE_API void lvlInstance::DrawReflected(float a1)        { hook::Thunk<0x464420>::Call<void>(this, a1); }
AGE_API void lvlInstance::DrawReflectedParts(int a1)     { hook::Thunk<0x4648B0>::Call<void>(this, a1); }
AGE_API int lvlInstance::Init(const char *a1, const Matrix34 &a2, int a3)
                                            {return hook::Thunk<0x463D90>::Call<int>(this, a1, &a2, a3); }


AGE_API bool lvlInstance::IsLandmark()                   { return hook::Thunk<0x463180>::Call<bool>(this); }
AGE_API bool lvlInstance::IsCollidable()                 { return hook::Thunk<0x43FC40>::Call<bool>(this); }
AGE_API bool lvlInstance::IsTerrainCollidable()          { return hook::Thunk<0x43FC50>::Call<bool>(this); }
AGE_API int lvlInstance::GetNumLightSources()            { return hook::Thunk<0x4632B0>::Call<int>(this); }
AGE_API void lvlInstance::GetLightInfo(int a1, cltLight *a2)
                                            { hook::Thunk<0x4630B0>::Call<void>(this); }
AGE_API int lvlInstance::SetupGfxLights(const Matrix34 &a1)
                                            { return hook::Thunk<0x464670>::Call<int>(this, &a1); }
AGE_API phBound * lvlInstance::GetBound(int a1)          { return hook::Thunk<0x4648C0>::Call<phBound *>(this, a1); };

void lvlInstance::BindLuaLate(LuaState L) {
    LuaBinding(L).beginClass<lvlInstance>("lvlInstance")
        .addFunction("GetBound", &getBoundLua)
        .addFunction("Cast", &castLua)
    .endClass();
}

void lvlInstance::BindLua(LuaState L) {
    LuaBinding(L).beginClass<lvlInstance>("lvlInstance")
        //fields
        .addProperty("Owner", &GetOwner, &SetOwner)
        .addProperty("SubType", &GetSubType, &SetSubType)
        .addProperty("Flags", &GetFlags, &SetFlags)
        .addProperty("GeomIndex", &GetGeomIndex, &SetGeomIndex)
        .addPropertyReadOnly("Previous", &GetPrevious)
        .addPropertyReadOnly("Next", &GetNext)
        .addPropertyReadOnly("CurrentRoom", &GetRoomId)
        .addPropertyReadOnly("NumShaders", &GetShaderCount)
        .addPropertyReadOnly("NumVariants", &GetVariantCount)
        .addPropertyReadOnly("Name", &GetName)

        //statics
        .addStaticProperty("GeomTableSize", &GetGeomSetCount)
        .addStaticFunction("ResetInstanceHeap", &ResetInstanceHeap)
        .addStaticFunction("ResetAll", &ResetAll)
        .addStaticFunction("SetShadowBillboardMtx", &SetShadowBillboardMtx)
        .addStaticFunction("GetGeomSet", &GetGeomSet)

        //functions
        .addStaticFunction("ComputeShadowMatrix", &computeShadowMatrixLua)
        .addFunction("GetName", &GetName)
        .addFunction("LoadBoundOnLastEntry", &LoadBoundOnLastEntry)
        .addFunction("GetBoundSphere", &GetBoundSphere)
        .addFunction("BeginGeom", &BeginGeom)
        .addFunction("AddGeom", &AddGeom)
        .addFunction("EndGeom", &EndGeom)
        .addStaticFunction("AddSphere", &AddSphere)
        .addFunction("InitBoundTerrain", &InitBoundTerrain)
        .addFunction("InitBoundTerrainLocal", &InitBoundTerrainLocal)
        .addFunction("InitBoundTerrrainLocal", &InitBoundTerrainLocal) // Originally misspelled. Kept for compatibility.
        .addFunction("NeedGhostBound", &NeedGhostBound)
        .addFunction("InitGhostBound", &InitGhostBound)
        .addFunction("InitGhost", &InitGhost)
        .addStaticFunction("CreateTempBounds", &CreateTempBounds)
        .addStaticFunction("DeleteTempBounds", &DeleteTempBounds)
        .addFunction("PreLoadShader", &PreLoadShader)
        .addFunction("Optimize", &Optimize)
        .addFunction("GetGeom", &GetGeom)
        .addFunction("GetGeomBase", &GetGeomBase, LUA_ARGS(_def<int, 0>))
        .addFunction("GetShader", &getShaderLua, LUA_ARGS(int, _def<int, 0>))

        //virtuals
        .addFunction("Reset", &Reset)
        .addFunction("IsVisible", &IsVisible)
        .addFunction("GetMatrix", &getMatrixLua)
        .addFunction("GetPosition", &getPositionLua)
        .addFunction("SetMatrix", &SetMatrix)
        .addFunction("SetPosition", &setPositionLua)
        .addFunction("SetVariant", &SetVariant)
        .addFunction("GetRadius", &GetRadius)
        .addFunction("GetEntity", &GetEntity)
        .addFunction("AttachEntity", &AttachEntity)
        .addFunction("GetVelocity", &GetVelocity)
        .addFunction("Detach", &Detach)
        .addFunction("Draw", &Draw)
        .addFunction("DrawShadow", &DrawShadow)
        .addFunction("DrawShadowMap", &DrawShadowMap)
        .addFunction("DrawGlow", &DrawGlow)
        .addFunction("DrawReflected", &DrawReflected)
        .addFunction("DrawReflectedParts", &DrawReflectedParts)
        .addFunction("Init", &Init)
        .addFunction("SizeOf", &SizeOf)
        .addFunction("IsLandmark", &IsLandmark)
        .addFunction("IsCollidable", &IsCollidable)
        .addFunction("IsTerrainCollidable", &IsTerrainCollidable)
        .addFunction("GetNumLightSources", &GetNumLightSources)
        //addFunction("GetLightInfo", [](int light) { cltLight light; lvlInstance::GetLightInfo(light, &light); return light; })
        .addFunction("SetupGfxLights", &SetupGfxLights)
        //.addFunction("GetBound", &GetBound) -> moved to BindLuaLate
    .endClass();
}

/*
    lvlInstanceLuaIterator
*/

LvlInstanceLuaIterator::LvlInstanceLuaIterator(lvlInstance* instance)
{
    this->last = instance;
    this->end = nullptr;
}

LvlInstanceLuaIterator::LvlInstanceLuaIterator(lvlInstance* instance, lvlInstance* end)
{
    this->last = instance;
    this->end = end;
}

LvlInstanceLuaIterator::~LvlInstanceLuaIterator()
{
}

int LvlInstanceLuaIterator::run(lua_State* L)
{
    if (this->last)
    {
        LuaState(L).push(this->last);
        this->last = this->last->GetNext();
        if (this->last == this->end)
            this->last = nullptr;
        return 1;
    }
    else
    {
        return 0;
    }
}