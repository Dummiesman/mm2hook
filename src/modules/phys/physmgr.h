#pragma once
#include <modules\node\cullable.h>
#include <modules\phys\segment.h>
#include <modules\phys\phintersection.h>

namespace MM2
{
    // Forward declarations
    class phPhysManager;
    class dgPhysManager;
    struct PhysicsStats;

    // External declarations
    extern class lvlInstance;
    extern class dgPhysEntity;

    // Class definitions
    struct LuaRaycastResult 
    {
    private:
        lvlSegment* segment;
        lvlIntersection* isect;
    private:
        Vector3 getIntersectionPoint();
        Vector3 getNormal();
        float getNormalizedDistance();
        float getPenetration();
        phBound* getBound();
        phPolygon* getPolygon();
    public:
        LuaRaycastResult(lvlSegment* segment, lvlIntersection* isect);

        static void BindLua(LuaState L);
    };

    struct PhysicsStats
    {
    public:
        float TotalUpdateTime;
        float ObjPairCollectingTime;
        float TotalCollisionTime;
        float MoverUpdateTime;
        float MoverGatherTime;
        float ObjToObjCollisionTime;
        float ObjToObjImpactTime;
        float CarImpactTime;
        float ObjTerrainCollisionTime;
        float ObjTerrainImpactTime;
        float PostCollisionTime;
        float CollisionsTime;

        int MoverVsMover;
        int MoverVsCollidable;

        int TotalProbes;
        float TotalProbeTime;
        int ExternalProbes;
        float ExternalProbeTime;
    public:
        static void BindLua(LuaState L);
    };

    class phPhysicsManager {
    public:
        virtual bool TestProbe(phSegment const &a1, phIntersection* a2, unsigned int a3, int a4)             PURE;
        virtual bool TestSphere(Vector3 const &a1, float a2, class phImpact* a3, unsigned int a4, int a5)    PURE;
        virtual void PromoteInstance(class phInstance* a1)                                                   PURE;
        virtual void DemoteInstance(class phInstance* a1)                                                    PURE;
        virtual void KillInstance(class phInstance* a1)                                                      PURE;
        virtual void DisableInstance(class phInstance* a1)                                                   PURE;
        virtual void EnableInstance(class phInstance* a1, int a2)                                            PURE;
        virtual class phColliderBase *  GetCollider(class phInstance* a1)                                    PURE;
        virtual class phInstance* CreateInstance()                                                           PURE;
    };

    class dgPhysManager : phPhysicsManager, asCullable {
    public:
        static const int MAX_MOVERS = 64; // Default 32
        static const int MAX_ROOMS = 20;
    public:
        struct CollisionTableEntry
        {
        private:
            lvlInstance* m_Instance;
            dgPhysEntity* m_PhysEntity;
            unsigned short m_CollidablesCount;
            lvlInstance* m_Collidables[32];
            short m_Flags;
            byte m_Priority;
            byte byte_8f;
        public:
            void SetFlags(short flags);
            short GetFlags() const;
            byte GetPriority() const;
            dgPhysEntity* GetEntity() const;
            lvlInstance* GetInstance() const;
            int GetCollidablesCount() const;
            lvlInstance* GetCollidable(int num);
            void SetCollidable(int num, lvlInstance* instance);
            void ClearCollidables();
            void Reset();
            void Set(dgPhysEntity* entity, lvlInstance* instance, short flags, byte priority);

            static void BindLua(LuaState L);
        };
    private:
        int NumActiveRooms;
        int NumCulledMovers;
        int ActiveRoomIds[MAX_ROOMS];
        int NumSamples;
        float perfTotalUpdateTime;
        float perfObjObjCollision;
        float perfObjObjImpact;
        float perfObjTerrainCollision;
        float perfObjTerrainImpact;
        int dword_78;
        float CarDamageImpactTime;
        int NumMovers;
        int MoverVsMover; // unused
        int MoverVsCollidable;
        int ExternalProbes;
        float TotalProbeTime;
        float ExternalProbeTime;
        BOOL IsUpdating;
        class phContactMgr * ContactMgr;
        int LinesMode; // unused
        dgPhysManager::CollisionTableEntry Table[MAX_MOVERS];
        int NumActiveMovers;
        int MaxSamples;
        float SampleStep;
    private:
        std::shared_ptr<LuaRaycastResult> collideLua(Vector3 start, Vector3 end);
        static hook::Type<float> TimeDiscrepancy;
    public:
        static hook::Type<lvlInstance *> PlayerInst;
        static hook::Type<dgPhysManager *> Instance;
        static hook::Type<float> Gravity;

        static hook::Type<float> perfObjPairCollecting;
        static hook::Type<float> perfMoverUpdate;
        static hook::Type<float> perfPhysGathering;
        static hook::Type<float> perfPhysCollide;
        static hook::Type<float> perfTotalCollisionTime;
        static hook::Type<float> perfPostCollision;

        PhysicsStats GetStats() const;

        int GetLastActiveMoverCount() const;
        int GetActiveMoverCount() const;
        int GetActiveRoomCount() const;
        int GetActiveRoomId(int index) const;
        int GetNumSamples() const;
        int GetNumCulledMovers() const;
        int GetMaxSamples() const;
        void SetMaxSamples(int samples);
        float GetSampleStep() const;
        void SetSampleStep(float step);

        CollisionTableEntry* GetMover(int num);
        CollisionTableEntry* FindMover(lvlInstance* instance);

        static float GetGravity();
        static void SetGravity(float gravity);

        AGE_API bool CollideTerrain(CollisionTableEntry* entry);
        AGE_API void GatherCollidables(CollisionTableEntry* entry);
        AGE_API void IgnoreMover(lvlInstance* instance);
        AGE_API void NewMover(lvlInstance* instance);
        AGE_API void NewMover(lvlInstance* instanceA, lvlInstance* instanceB);
        AGE_API void NewMover(lvlInstance* instanceA, lvlInstance* instanceB, lvlInstance* instanceC);
        AGE_API void DeclareMover(lvlInstance* instance, int a3, int a4);
        AGE_API bool TrivialCollideInstances(lvlInstance* instanceA, lvlInstance* instanceB);
        AGE_API bool CollideInstances(lvlInstance* instanceA, lvlInstance* instanceB);
        AGE_API bool Collide(lvlSegment& segment, lvlIntersection* intersection, int a4, lvlInstance* ignoreInstance, ushort flags1, int flags2);
        void ResetTable();
        void Reset();
        void Update();

        static void BindLua(LuaState L);
    };

    //ASSERT_SIZEOF(dgPhysManager, 0x12B0);
}