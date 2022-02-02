#pragma once
#include <handlers\lua_drawable.h>
#include <modules\phys\segment.h>
#include <modules\level\inst.h>
#include <modules\rgl.h>

namespace MM2
{
    // Forward declarations
    class lvlLevel;
    struct lvlRoomInfo;
    class cltLight;

    // External declarations
    extern class gfxTexture;
    extern class asParticles;
    extern class gfxViewport;
    extern struct lvlSegment;
    extern struct lvlIntersection;

    // Class definitions
    enum class RoomFlags : byte
    {
        Unknown = 0x1,
        Subterranean = 0x2,
        HasWater = 0x4,
        Road = 0x8,
        Intersection = 0x10,
        SpecialBound = 0x20, //Might actually be instance
        Warp = 0x40,
        Instance = 0x80,
    };

    class cltLight
    {

    };

    struct lvlRoomInfo
    {
    private:
        int getInstancesLua(lua_State* L)
        {
            return CppFunctor::make<LvlInstanceLuaIterator>(L, this->FirstInstance);
        }

        int getStaticInstancesLua(lua_State* L)
        {
            return CppFunctor::make<LvlInstanceLuaIterator>(L, this->FirstStaticInstance);
        }
    public:
        unsigned __int16 Flags;
        unsigned __int16 InstanceFlags;
        lvlInstance* FirstInstance;
        lvlInstance* FirstStaticInstance;
        Vector4 BoundSphere;
        int Color;
        float MinY;
        float MaxY;

        static void BindLua(LuaState L) {
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
    };


    class lvlLevel : public asCullable {
    private:
        int InstanceLabelMask;
        lvlRoomInfo** RoomInfo;
        int RoomCount;
        lvlInstance* FirstLoadedInstance;
        int LoadedInstanceCount;
        char* Name;
    private:
        //lua drawables!
        inline int RegisterLuaDrawable(LuaRef self, LuaRef function, int phase = 1) 
        {
            return luaDrawableHandler::RegisterCallback(self, function, phase);
        }

        inline void UnregisterLuaDrawable(int id)
        {
            luaDrawableHandler::UnregisterCallback(id);
        }
    public:
        inline lvlRoomInfo* GetRoomInfo(int room) 
        {
            if (this->RoomInfo == nullptr || room >= RoomCount || room < 0)
                return nullptr;
            return this->RoomInfo[room];
        }

        inline int GetRoomCount()
        {
            return this->RoomCount;
        }
    public:
        static hook::Type<lvlLevel*> Singleton;

        AGE_API lvlLevel() {
            scoped_vtable x(this);
            hook::Thunk<0x4653A0>::Call<void>(this);
        };

        virtual AGE_API ~lvlLevel() {
            scoped_vtable x(this);
            hook::Thunk<0x465400>::Call<void>(this);
        };

        /* 
            lvlLevel virtuals
        */

        AGE_API virtual void Load(char const* a1)                   PURE;
        AGE_API virtual void Update()                               { hook::Thunk<0x465460>::Call<void>(this); }
        AGE_API virtual void PreDraw()                              PURE;
        AGE_API virtual void PostDraw()                             PURE;
        AGE_API virtual void Draw(const gfxViewport& a1, uint a2)   PURE;
                                                                    
        AGE_API virtual int FindRoomId(Vector3 const& a1, int a2)   PURE;
        AGE_API virtual int GetNeighborCount(int a1)                PURE;
        AGE_API virtual int GetNeighbors(int* a1, int a2)           PURE;
        AGE_API virtual int GetTouchedNeighbors(int* a1, int a2, int a3, const Vector4& a4)   
                                                                    PURE;
        AGE_API virtual int GetRoomPerimeter(int roomId, Vector3* out, int outSize)
                                                                    PURE;
        AGE_API virtual int GetVisitList(int* a1, int a2, Vector3 const& a3, Vector3 const& a4, int a5, int a6)
                                                                    { return 0; }
        AGE_API virtual bool Collide(int a1, lvlSegment & a2, lvlIntersection & a3)
                                                                    { return false; }
        AGE_API virtual bool GetBoundSphere(Vector4& a1, int a2)    { return false; }
        AGE_API virtual const class lvlLevelBound* GetBound()       PURE;
        AGE_API virtual void SetObjectDetail(int a1)                {}
        AGE_API virtual float GetWaterLevel(int a1)                 PURE;
        AGE_API virtual float GetLightingIntensity(Vector3 const& a1)
                                                                    PURE;
        AGE_API virtual void SetPtxHeight(asParticles& a1)          PURE;
        AGE_API virtual bool ClampToWorld(Vector3& a1)              { return false; }
        AGE_API virtual bool LoadInstances(const char* a1, const char* a2)
                                                                    { return hook::Thunk<0x656F0>::Call<bool>(this, a1, a2); }
        AGE_API virtual gfxTexture* GetEnvMap(int a1, Vector3 const& a2, float* a3)
                                                                    { return nullptr; }
                                                                    

        /*
            lvlLevel functions
        */
        AGE_API void MoveToRoom(lvlInstance *instance, int room) {
            hook::Thunk<0x465480>::Call<void>(this, instance, room);
        }

        AGE_API void ResetInstances() {
            hook::Thunk<0x465E50>::Call<void>(this);
        }

        AGE_API void LabelInstances(int room) {
            auto roomInfo = this->GetRoomInfo(room);
            if (roomInfo == nullptr)
                return;

            for (auto i = roomInfo->FirstInstance; i; i = i->GetNext()) {
                if ((i->GetFlags() & this->InstanceLabelMask) != 0) {
                    auto pos = i->GetPosition();
                    const char* name;

                    if (i->GetGeomIndex() != 0)
                        name = lvlInstance::GetGeomName(i->GetGeomIndex() - 1);
                    else
                        name = "(none)";

                    vglDrawLabelf(pos, "%s", name);
                }
            }
        }

        //lua
        static void BindLua(LuaState L) {
            LuaBinding(L).beginExtendClass<lvlLevel, asCullable>("lvlLevel")
                //properties
                .addPropertyReadOnly("NumRooms", &GetRoomCount)
                .addVariableRef("InstanceLabelMask", &lvlLevel::InstanceLabelMask)

                //virtual functions
                .addFunction("FindRoomId", &FindRoomId, LUA_ARGS(Vector3, _def<int, -1>))
                .addFunction("GetNeighborCount", &GetNeighborCount)
                .addFunction("SetObjectDetail", &SetObjectDetail)
                .addFunction("GetWaterLevel", &GetWaterLevel)
                .addFunction("GetLightingIntensity", &GetLightingIntensity)
                .addFunction("SetPtxHeight", &SetPtxHeight)

                //functions
                .addFunction("GetRoomInfo", &GetRoomInfo)
                .addFunction("MoveToRoom", &MoveToRoom)
                .addFunction("ResetInstances", &ResetInstances)
                .addFunction("LabelInstances", &LabelInstances)

                //drawable
                .addFunction("UnregisterDrawable", &UnregisterLuaDrawable)
                .addFunction("RegisterDrawable", &RegisterLuaDrawable, 
                    LUA_ARGS(LuaRef, LuaRef, _opt<int>)) //register a LuaCallback

                //singleton
                .addStaticProperty("Singleton", [] { return Singleton.get(); })
                .endClass();
        }
    };

    // Lua initialization

}