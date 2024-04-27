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
        int getInstancesLua(lua_State* L);
        int getStaticInstancesLua(lua_State* L);
    public:
        unsigned __int16 Flags;
        unsigned __int16 InstanceFlags;
        lvlInstance* FirstInstance;
        lvlInstance* FirstStaticInstance;
        Vector4 BoundSphere;
        int LightmapColor;
        float MinY;
        float MaxY;

        Vector4 GetLightmapColor() const;
        void SetLightmapColor(const Vector4& vec);
        static void BindLua(LuaState L);
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
        int RegisterLuaDrawable(LuaRef self, LuaRef function, int phase = 1);
        void UnregisterLuaDrawable(int id);

        //lua function for getting neighbours
        std::vector<int> LuaGetNeighbors(int room);
    protected:
        static hook::Type<lvlLevel*> Singleton;
    public:
        AGE_API lvlLevel();
        virtual AGE_API ~lvlLevel();

        /* 
            lvlLevel virtuals
        */

        AGE_API virtual void Load(char const* a1)                   PURE;
        AGE_API virtual void Update();
        AGE_API virtual void PreDraw()                              PURE;
        AGE_API virtual void PostDraw()                             PURE;
        AGE_API virtual void Draw(const gfxViewport& a1, uint a2)   PURE;
                                                                    
        AGE_API virtual int FindRoomId(Vector3 const& position, int previousRoom) const
                                                                    PURE;
        AGE_API virtual int GetNeighborCount(int room) const        PURE;
        AGE_API virtual int GetNeighbors(int* a1, int a2) const     PURE;
        AGE_API virtual int GetTouchedNeighbors(int* a1, int a2, int a3, const Vector4& a4)   
                                                                    PURE;
        AGE_API virtual int GetRoomPerimeter(int roomId, Vector3* out, int outSize) const
                                                                    PURE;
        AGE_API virtual int GetVisitList(int* a1, int a2, Vector3 const& a3, Vector3 const& a4, int a5, int a6);
        AGE_API virtual bool Collide(int a1, lvlSegment& a2, lvlIntersection& a3);
        AGE_API virtual bool GetBoundSphere(Vector4& a1, int a2);
        AGE_API virtual const class lvlLevelBound* GetBound()       PURE;
        AGE_API virtual void SetObjectDetail(int a1);
        AGE_API virtual float GetWaterLevel(int room) const           PURE;
        AGE_API virtual float GetLightingIntensity(Vector3 const& a1) const
                                                                    PURE;
        AGE_API virtual void SetPtxHeight(asParticles& a1)          PURE;
        AGE_API virtual bool ClampToWorld(Vector3& a1) const;
        AGE_API virtual bool LoadInstances(const char* directory, const char* filename);
        AGE_API virtual gfxTexture* GetEnvMap(int room, Vector3 const& position, float& intensity);
                                                                    

        /*
            lvlLevel functions
        */
        AGE_API void MoveToRoom(lvlInstance* instance, int room);
        AGE_API void ResetInstances();
        AGE_API void LabelInstances(int room);

        /*
            lvlLevel properties
        */
        static lvlLevel* GetSingleton();
        void Reparent(lvlInstance* instance);
        lvlRoomInfo* GetRoomInfo(int room);
        int GetRoomCount() const;
        void SetLevelName(char* name);
        const char* GetLevelName() const;

        //lua
        static void BindLua(LuaState L);
    };
}