#pragma once
#include "level.h"

using namespace MM2;

/*
   lvlRoomInfo
*/

int lvlRoomInfo::getInstancesLua(lua_State* L)
{
    return CppFunctor::make<LvlInstanceLuaIterator>(L, this->FirstInstance);
}

int lvlRoomInfo::getStaticInstancesLua(lua_State* L)
{
    return CppFunctor::make<LvlInstanceLuaIterator>(L, this->FirstStaticInstance);
}


void lvlRoomInfo::BindLua(LuaState L) {
    LuaBinding(L).beginClass<lvlRoomInfo>("lvlRoomInfo")
        .addFunction("GetInstances", &lvlRoomInfo::getInstancesLua)
        .addFunction("GetStaticInstances", &lvlRoomInfo::getStaticInstancesLua)
        .addVariable("Flags", &lvlRoomInfo::Flags, false)
        .addVariable("InstanceFlags", &lvlRoomInfo::InstanceFlags, false)
        .addVariable("FirstInstance", &lvlRoomInfo::FirstInstance, false)
        .addVariable("FirstStaticInstance", &lvlRoomInfo::FirstStaticInstance, false)
        .addVariable("BoundSphere", &lvlRoomInfo::BoundSphere, false)
        .addVariable("Color", &lvlRoomInfo::Color, false)
        .addVariable("MinY", &lvlRoomInfo::MinY, false)
        .addVariable("MaxY", &lvlRoomInfo::MaxY, false)
        .endClass();
}
  
/*
   lvlLevel
*/

declfield(lvlLevel::Singleton)(0x655D9C);

int lvlLevel::RegisterLuaDrawable(LuaRef self, LuaRef function, int phase)
{
    return luaDrawableHandler::RegisterCallback(self, function, phase);
}

void lvlLevel::UnregisterLuaDrawable(int id)
{
    luaDrawableHandler::UnregisterCallback(id);
}

std::vector<int> lvlLevel::LuaGetNeighbors(int room)
{
    int neighborCount = this->GetNeighborCount(room);
    int* rooms = static_cast<int*>(alloca(neighborCount * 4));
    int neighborRoomCount = this->GetNeighbors(rooms, room);

    std::vector<int> vec(neighborRoomCount);
    for (int i = 0; i < neighborRoomCount; i++)
    {
        vec.push_back(rooms[i]);
    }

    return vec;
}

AGE_API lvlLevel::lvlLevel() {
    scoped_vtable x(this);
    hook::Thunk<0x4653A0>::Call<void>(this);
};

AGE_API lvlLevel::~lvlLevel() {
    scoped_vtable x(this);
    hook::Thunk<0x465400>::Call<void>(this);
};

/* 
    lvlLevel virtuals
*/

AGE_API void lvlLevel::Update()                               { hook::Thunk<0x465460>::Call<void>(this); }
AGE_API int lvlLevel::GetVisitList(int* a1, int a2, Vector3 const& a3, Vector3 const& a4, int a5, int a6)
                                                            { return 0; }
AGE_API bool lvlLevel::Collide(int a1, lvlSegment & a2, lvlIntersection & a3)
                                                            { return false; }
AGE_API bool lvlLevel::GetBoundSphere(Vector4& a1, int a2)    { return false; }
AGE_API void lvlLevel::SetObjectDetail(int a1)                {}
AGE_API bool lvlLevel::ClampToWorld(Vector3& a1) const        { return false; }
AGE_API bool lvlLevel::LoadInstances(const char* directory, const char* filename)
                                                            { return hook::Thunk<0x4656F0>::Call<bool>(this, directory, filename); }
AGE_API gfxTexture* lvlLevel::GetEnvMap(int room, Vector3 const& position, float& intensity)
                                                            { return nullptr; }
                                                                    

/*
    lvlLevel functions
*/
AGE_API void lvlLevel::MoveToRoom(lvlInstance *instance, int room) {
    hook::Thunk<0x465480>::Call<void>(this, instance, room);
}

AGE_API void lvlLevel::ResetInstances() {
    hook::Thunk<0x465E50>::Call<void>(this);
}

AGE_API void lvlLevel::LabelInstances(int room) {
    auto roomInfo = this->GetRoomInfo(room);
    if (roomInfo == nullptr)
        return;

    for (auto i = roomInfo->FirstInstance; i; i = i->GetNext()) {
        if ((i->GetFlags() & this->InstanceLabelMask) != 0) {
            vglDrawLabelf(i->GetPosition(), "%s", i->GetName());
        }
    }
}

/*
    lvlLevel properties
*/

lvlLevel* lvlLevel::GetSingleton() {
    return lvlLevel::Singleton.get();
}

void lvlLevel::Reparent(lvlInstance* instance) {
    auto &instancePos = instance->GetPosition();
    int room = this->FindRoomId(instancePos, instance->GetRoomId());
    if (room != instance->GetRoomId())
        this->MoveToRoom(instance, room);
}

lvlRoomInfo* lvlLevel::GetRoomInfo(int room) {
    if (this->RoomInfo == nullptr || room >= RoomCount || room < 0)
        return nullptr;
    return this->RoomInfo[room];
}

int lvlLevel::GetRoomCount() const {
    return this->RoomCount;
}

void lvlLevel::SetLevelName(char* name) {
    this->Name = name;
}

const char* lvlLevel::GetLevelName() const {
    return this->Name;
}

void lvlLevel::BindLua(LuaState L) {
    LuaBinding(L).beginExtendClass<lvlLevel, asCullable>("lvlLevel")
        //properties
        .addPropertyReadOnly("NumRooms", &GetRoomCount)
        .addVariable("InstanceLabelMask", &lvlLevel::InstanceLabelMask)

        //virtual functions
        .addFunction("FindRoomId", &FindRoomId, LUA_ARGS(Vector3, _def<int, -1>))
        .addFunction("GetNeighbors", &LuaGetNeighbors)
        .addFunction("GetNeighborCount", &GetNeighborCount)
        .addFunction("SetObjectDetail", &SetObjectDetail)
        .addFunction("GetWaterLevel", &GetWaterLevel)
        .addFunction("GetLightingIntensity", &GetLightingIntensity)
        .addFunction("SetPtxHeight", &SetPtxHeight)

        //functions
        .addFunction("GetRoomInfo", &GetRoomInfo)
        .addFunction("Reparent", &Reparent)
        .addFunction("MoveToRoom", &MoveToRoom)
        .addFunction("ResetInstances", &ResetInstances)
        .addFunction("LabelInstances", &LabelInstances)
        .addFunction("LoadInstances", &LoadInstances)

        //drawable
        .addFunction("UnregisterDrawable", &UnregisterLuaDrawable)
        .addFunction("RegisterDrawable", &RegisterLuaDrawable, 
            LUA_ARGS(LuaRef, LuaRef, _opt<int>)) //register a LuaCallback

        //singleton
        .addStaticProperty("Singleton", &lvlLevel::GetSingleton)
        .endClass();
}