#pragma once
#include "physmgr.h"
#include <modules/level/level.h>
#include <modules/level/inst.h>
#include <modules/phys/segment.h>
#include <modules/phys/physentity.h>
#include <modules/phys/phcollider.h>
#include <modules/phys/phjoint.h>

using namespace MM2;

LPCSTR lastCollidedInstance = "(null)";
LPCSTR lastCollidedInstance2 = "(null)";

/*
    LuaRaycastResult
*/
LuaRaycastResult::LuaRaycastResult(lvlSegment segment, lvlIntersection isect)
{
    this->segment = segment;
    this->isect = isect;
}

void LuaRaycastResult::BindLua(LuaState L) {
    LuaBinding(L).beginClass<LuaRaycastResult>("LuaRaycastResult")
        .addPropertyReadOnly("NormalizedDistance", &getNormalizedDistance)
        .addPropertyReadOnly("Penetration", &getPenetration)
        .addPropertyReadOnly("Point", &getIntersectionPoint)
        .addPropertyReadOnly("Bound", &getBound)
        .addPropertyReadOnly("Normal", &getNormal)
        .addPropertyReadOnly("Polygon", &getPolygon)
        .endClass();
}

Vector3 LuaRaycastResult::getIntersectionPoint()
{
    return isect.IntersectionPoint.Point;
}

Vector3 LuaRaycastResult::getNormal()
{
    return isect.IntersectionPoint.Normal;
}

float LuaRaycastResult::getNormalizedDistance()
{
    return isect.IntersectionPoint.NormalizedDistance;
}

float LuaRaycastResult::getPenetration()
{
    return isect.IntersectionPoint.Penetration;
}

phBound* LuaRaycastResult::getBound()
{
    return isect.Bound;
}

phPolygon* LuaRaycastResult::getPolygon()
{
    return isect.Poly;
}

/*
    PhysicsStats
*/
void PhysicsStats::BindLua(LuaState L) {
    LuaBinding(L).beginClass<PhysicsStats>("PhysicsStats")
        .addVariable("TotalUpdateTime", &PhysicsStats::TotalUpdateTime, false)
        .addVariable("ObjPairCollectingTime", &PhysicsStats::ObjPairCollectingTime, false)
        .addVariable("TotalCollisionTime", &PhysicsStats::TotalCollisionTime, false)
        .addVariable("MoverUpdateTime", &PhysicsStats::MoverUpdateTime, false)
        .addVariable("MoverGatherTime", &PhysicsStats::MoverGatherTime, false)
        .addVariable("ObjToObjCollisionTime", &PhysicsStats::ObjToObjCollisionTime, false)
        .addVariable("ObjToObjImpactTime", &PhysicsStats::ObjToObjImpactTime, false)
        .addVariable("CarImpactTime", &PhysicsStats::CarImpactTime, false)
        .addVariable("ObjTerrainCollisionTime", &PhysicsStats::ObjTerrainCollisionTime, false)
        .addVariable("ObjTerrainImpactTime", &PhysicsStats::ObjTerrainImpactTime, false)
        .addVariable("PostCollisionTime", &PhysicsStats::PostCollisionTime, false)
        .addVariable("TotalProbes", &PhysicsStats::TotalProbes, false)
        .addVariable("TotalProbeTime", &PhysicsStats::TotalProbeTime, false)
        .addVariable("ExternalProbes", &PhysicsStats::ExternalProbes, false)
        .addVariable("ExternalProbeTime", &PhysicsStats::ExternalProbeTime, false)
        .addVariable("MoverVsMover", &PhysicsStats::MoverVsMover, false)
        .addVariable("MoverVsCollidable", &PhysicsStats::MoverVsCollidable, false)
        .addVariable("CollisionsTime", &PhysicsStats::CollisionsTime, false)
        .endClass();
}

void MM2::dgPhysManager::CollisionTableEntry::CopyFrom(CollisionTableEntry& other)
{
    this->byte_8f = other.byte_8f;
    this->m_Flags = other.m_Flags;
    this->m_CollidablesCount = other.m_CollidablesCount;
    this->m_Instance = other.m_Instance;
    this->m_Priority = other.m_Priority;
    this->m_PhysEntity = other.m_PhysEntity;
    for (int i = 0; i < other.m_CollidablesCount; i++)
    {
        this->m_Collidables[i] = other.m_Collidables[i];
    }
}

/*
    dgPhysManager::CollisionTableEntry
*/
void dgPhysManager::CollisionTableEntry::SetFlags(short flags)
{
    this->m_Flags = flags;
}

short dgPhysManager::CollisionTableEntry::GetFlags() const
{
    return this->m_Flags;
}

byte MM2::dgPhysManager::CollisionTableEntry::GetPriority() const
{
    return this->m_Priority;
}

dgPhysEntity* MM2::dgPhysManager::CollisionTableEntry::GetEntity() const
{
    return this->m_PhysEntity;
}

lvlInstance* dgPhysManager::CollisionTableEntry::GetInstance() const
{
    return this->m_Instance;
}

int dgPhysManager::CollisionTableEntry::GetCollidablesCount() const
{
    return this->m_CollidablesCount;
}

lvlInstance* dgPhysManager::CollisionTableEntry::GetCollidable(int num)
{
    if (num < 0 || num >= this->GetCollidablesCount())
        return nullptr;
    return this->m_Collidables[num];
}

void MM2::dgPhysManager::CollisionTableEntry::SetCollidable(int num, lvlInstance* instance)
{
    if (num < 0 || num >= 32)
        return;
    this->m_Collidables[num] = instance;

}

void MM2::dgPhysManager::CollisionTableEntry::ClearCollidables()
{
    m_CollidablesCount = 0;
}

void dgPhysManager::CollisionTableEntry::Reset() 
{
    this->ClearCollidables();
    m_Flags = 0;
}


void dgPhysManager::CollisionTableEntry::Set(dgPhysEntity* entity, lvlInstance* instance, short flags, byte priority)
{
    this->m_PhysEntity = entity;
    this->m_Instance = instance;
    this->m_Flags = flags;
    this->m_Priority = priority;
}

void MM2::dgPhysManager::SanityCheck(LPCSTR title, bool checkDelayFlag)
{
    for (int i = 0; i < NumActiveMovers; i++)
    {
        auto mover = &this->Table[i];
        auto instance = mover->GetInstance();

        if (instance == nullptr)
        {
            Abortf("dgPhysManager::SanityCheck FAILED at stage %s. nullptr instance in mover table at index %i. Current movers %i", title, i, NumActiveMovers);
        }

        if (mover->GetFlags() & 1)
        {
            auto entity = mover->GetEntity();

            if (entity == nullptr)
            {
                Abortf("dgPhysManager::SanityCheck FAILED at stage %s. Active mover with no physentity. (instance %s), (last collided instances were %s, %s). Current movers %i", title, mover->GetInstance()->GetName(), lastCollidedInstance, lastCollidedInstance2, NumActiveMovers);
            }
        }

        if (mover->GetFlags() & 256 && checkDelayFlag)
        {
            auto entity = mover->GetEntity();

            if (entity == nullptr)
            {
                Abortf("dgPhysManager::SanityCheck FAILED at stage %s. Delayed active mover with no physentity. (instance %s), (last collided instances were %s, %s). Current movers %i", title, mover->GetInstance()->GetName(), lastCollidedInstance, lastCollidedInstance2, NumActiveMovers);
            }
        }
    }        
}

void dgPhysManager::CollisionTableEntry::BindLua(LuaState L) {
    LuaBinding(L).beginClass<dgPhysManager>("dgPhysManager")
        .beginClass<dgPhysManager::CollisionTableEntry>("CollisionTableEntry")
        .addPropertyReadOnly("NumCollidables", &GetCollidablesCount)
        .addPropertyReadOnly("Instance", &GetInstance)
        .addFunction("GetCollidable", &GetCollidable)
        .endClass();
}

/*
    dgPhysManager
*/
declfield(dgPhysManager::TimeDiscrepancy)(0x660F10);

declfield(dgPhysManager::Instance)(0x65D9DC);
declfield(dgPhysManager::Gravity)(0x5C6C1C);

declfield(dgPhysManager::perfObjPairCollecting)(0x660F08);
declfield(dgPhysManager::perfPhysGathering)(0x660F0C);
declfield(dgPhysManager::perfMoverUpdate)(0x655DAC);
declfield(dgPhysManager::perfTotalCollisionTime)(0x655DA8);
declfield(dgPhysManager::perfPostCollision)(0x65D9D8);
declfield(dgPhysManager::perfPhysCollide)(0x655DB0);

declfield(dgPhysManager::PlayerInst)(0x65D9E0);

declfield(dgPhysManager::sm_OpponentOptimization)(0x660F20);

static int physTotalProbes = 0;

int dgPhysManager::collideLua(lua_State* L, Vector3 start, Vector3 end)
{
    lvlSegment segment;
    lvlIntersection isect;
    segment.Set(start, end, 0, nullptr);

    bool collided = dgPhysManager::Collide(segment, &isect, 0, nullptr, 0x20, 0);
    if (collided)
    {
        auto luaResult = LuaRaycastResult(segment, isect);
        LuaState(L).push(luaResult);
    }
    else 
    {
        LuaState(L).push(nullptr);
    }
    return 1;
}
    
PhysicsStats dgPhysManager::GetStats()
{
    // make stats struct
    auto stats = PhysicsStats();
    stats.CarImpactTime = this->CarDamageImpactTime;
    stats.CollisionsTime = dgPhysManager::perfPhysCollide.get();
    stats.MoverVsCollidable = this->MoverVsCollidable;
    stats.MoverVsMover = this->MoverVsMover;
    stats.MoverGatherTime = dgPhysManager::perfPhysGathering.get();
    stats.MoverUpdateTime = dgPhysManager::perfMoverUpdate.get();
    stats.ObjPairCollectingTime = dgPhysManager::perfObjPairCollecting.get();
    stats.ObjTerrainCollisionTime = this->perfObjTerrainCollision;
    stats.ObjTerrainImpactTime = this->perfObjTerrainImpact;
    stats.ObjToObjCollisionTime = this->perfObjObjCollision;
    stats.ObjToObjImpactTime = this->perfObjObjImpact;
    stats.PostCollisionTime = dgPhysManager::perfPostCollision.get();
    stats.TotalCollisionTime = this->perfTotalCollisionTime;
    stats.TotalUpdateTime = this->perfTotalUpdateTime;
    stats.ExternalProbes = this->ExternalProbes;
    stats.ExternalProbeTime = this->ExternalProbeTime;
    stats.TotalProbeTime = this->TotalProbeTime;
    stats.TotalProbes = physTotalProbes;

    // reset stats that accumulate over time (in dev builds this is reset in dgPhysManager::Stats, so I feel this is appropriate here)
    this->ExternalProbes = 0;
    this->ExternalProbeTime = 0.0f;
    this->NumCulledMovers = 0;
    this->TotalProbeTime = 0.0f;
    this->CarDamageImpactTime = 0.0f;
    this->perfObjObjImpact = 0.0f;
    this->perfObjObjCollision = 0.0f;
    this->perfObjTerrainImpact = 0.0f;
    this->perfObjTerrainCollision = 0.0f;
    dgPhysManager::perfMoverUpdate.set(0.0f);
    dgPhysManager::perfPhysGathering.set(0.0f);
    dgPhysManager::perfPhysCollide.set(0.0f);
    physTotalProbes = 0;

    return stats;
}

int dgPhysManager::GetLastActiveMoverCount() const
{
    return this->NumMovers;
}

int dgPhysManager::GetActiveMoverCount() const
{
    return this->NumActiveMovers;
}

int dgPhysManager::GetActiveRoomCount() const
{
    return this->NumActiveRooms;
}

int dgPhysManager::GetActiveRoomId(int index) const
{
    if (index > 20 || index < 0)
        return -1;
    return this->ActiveRoomIds[index];
}

int dgPhysManager::GetNumSamples() const
{
    return this->NumSamples;
}

int dgPhysManager::GetNumCulledMovers() const
{
    return this->NumCulledMovers;
}

int dgPhysManager::GetMaxSamples() const
{
    return this->MaxSamples;
}

void dgPhysManager::SetMaxSamples(int samples)
{
    this->MaxSamples = samples;
}

float dgPhysManager::GetSampleStep() const
{
    return this->SampleStep;
}

void dgPhysManager::SetSampleStep(float step)
{
    this->SampleStep = step;
}

dgPhysManager::CollisionTableEntry* dgPhysManager::GetMover(int num)
{
    if (num < 0 || num >= MAX_MOVERS)
        return nullptr;
    return &this->Table[num];
}

dgPhysManager::CollisionTableEntry* dgPhysManager::FindMover(lvlInstance* instance)
{
    int count = this->GetActiveMoverCount();
    for (int i = 0; i < count; i++)
    {
        auto entry = GetMover(i);
        if (entry->GetInstance() == instance)
            return entry;
    }
    return nullptr;
}

float dgPhysManager::GetGravity(){
    return dgPhysManager::Gravity.get();
}

void dgPhysManager::SetGravity(float gravity)
{
    dgPhysManager::Gravity.set(gravity);
}

void dgPhysManager::ResetTable() {
    for (int i = 0; i < MAX_MOVERS; i++) 
    {
        auto entry = &this->Table[i];
        entry->Reset();
    }
    NumActiveMovers = 0;
}

AGE_API bool MM2::dgPhysManager::CollideTerrain(CollisionTableEntry* entry)
{
    return hook::Thunk<0x469A60>::Call<bool>(this, entry);
}

AGE_API void MM2::dgPhysManager::GatherCollidables(CollisionTableEntry* entry)
{
    hook::Thunk<0x4694E0>::Call<void>(this, entry);
}

AGE_API void MM2::dgPhysManager::IgnoreMover(lvlInstance* instance)
{
    for (int i = 0; i < this->GetActiveMoverCount(); i++) {
        auto entry = GetMover(i);
        if (entry->GetInstance() == instance) {
            entry->SetFlags(0);
            break;
        }
    }
}

AGE_API void MM2::dgPhysManager::NewMover(lvlInstance* instance)
{
    if (instance->GetRoomId() == 0)
    {
        Errorf("PHYS.DeclareMover : mover not even in city");
        return;
    }

    if (NumActiveMovers >= MAX_MOVERS)
    {
        Errorf("PHYS.NewMover, raise MAX_MOVERS past %d", this->NumActiveMovers);
        return;
    }

    // find existing
    for (int i = 0; i < NumActiveMovers; i++)
    {
        auto entry = &Table[i];
        if (entry->GetInstance() == instance) {
            Errorf("PHYS.NewMover, mover's already here");
            return;
        }
    }

    // add new
    auto entry = &Table[NumActiveMovers];
    entry->Set(instance->GetEntity(), instance, 256, 0);
    NumActiveMovers++;
}

AGE_API void MM2::dgPhysManager::NewMover(lvlInstance* instanceA, lvlInstance* instanceB)
{
    if (instanceA->GetRoomId() == 0)
    {
        Errorf("PHYS.DeclareMover : mover not even in city");
        return;
    }

    if (NumActiveMovers >= MAX_MOVERS)
    {
        return;
    }

    // find existing
    for (int i = 0; i < NumActiveMovers; i++) {
        auto entry = &Table[i];
        if (entry->GetInstance() == instanceA) {
            entry->Set(instanceA->GetEntity(), instanceA, 256, 0);
            return;
        }
    }

    // find other entry
    int otherEntryIndex = -1;
    for (int i = 0; i < NumActiveMovers; i++) {
        auto entry = &Table[i];
        if (entry->GetInstance() == instanceB) {
            otherEntryIndex = i;
            break;
        }
    }

    if (otherEntryIndex >= 0) 
    {
        // setup a new mover
        auto entry = &Table[NumActiveMovers];
        entry->Set(instanceA->GetEntity(), instanceA, 256, 0);

        // remove instanceA from instanceB collidable list
        auto otherEntry = &Table[otherEntryIndex];
        for(int i=0; i < otherEntry->GetCollidablesCount(); i++)
        {
            if (otherEntry->GetCollidable(i) == instanceA) 
            {
                otherEntry->SetCollidable(i, nullptr);
            }
        }

        NumActiveMovers++;
    }
    else
    {
        Errorf("PHYS.NewMover - not able to find other's entry");
        return;
    }
}

AGE_API void MM2::dgPhysManager::NewMover(lvlInstance* instanceA, lvlInstance* instanceB, lvlInstance* instanceC)
{
    if (instanceA->GetRoomId() == 0)
    {
        Errorf("PHYS.DeclareMover : mover not even in city");
        return;
    }

    if (NumActiveMovers >= MAX_MOVERS)
    {
        Errorf("PHYS.NewMover, raise MAX_MOVERS past %d", this->NumActiveMovers);
        return;
    }

    // find existing
    for (int i = 0; i < NumActiveMovers; i++)
    {
        auto entry = &Table[i];
        if (entry->GetInstance() == instanceB) {
            Errorf("PHYS.NewMover, mover's already here");
            return;
        }
    }

    // find other entry
    int otherEntryIndex = -1;
    for (int i = 0; i < NumActiveMovers; i++) {
        auto entry = &Table[i];
        if (entry->GetInstance() == instanceC) {
            otherEntryIndex = i;
            break;
        }
    }

    if (otherEntryIndex >= 0)
    {
        // setup a new mover
        auto entry = &Table[NumActiveMovers];
        entry->Set(instanceA->GetEntity(), instanceA, 256, 0);

        // remove instanceB from all collidable lists
        for (int i = 0; i < NumActiveMovers; i++) {
            auto entry = &Table[i];
            for (int j = 0; j < entry->GetCollidablesCount(); j++)
            {
                if (entry->GetCollidable(j) == instanceB)
                {
                    entry->SetCollidable(j, nullptr);
                }
            }
        }

        NumActiveMovers++;
    }
    else
    {
        Errorf("PHYS.NewMover - not able to find other's entry");
        return;
    }
}

void dgPhysManager::DeclareMover(lvlInstance* instance, int priority, int flags)
{
    if (instance->GetRoomId() == 0) {
        Errorf("PHYS.DeclareMover : mover not even in city");
        return;
    }
    
    if (instance->GetEntity() == nullptr && (flags & 1) != 0) {
        Errorf("dgPhysManager::DeclareMover : cant update a mover with no physEntity");
        return;
    }

    // find existing
    for (int i = 0; i < NumActiveMovers; i++) 
    {
        auto entry = &Table[i];
        if (entry->GetInstance() == instance) 
        {
            entry->SetFlags(entry->GetFlags() | flags);
            return;
        }
    }

    // add new if possible
    if (NumActiveMovers >= MAX_MOVERS) 
    {
        return;
    }

    auto entry = &Table[NumActiveMovers];
    entry->Set(instance->GetEntity(), instance, flags, priority);

    if (priority > 2)
    {
        // add active and neighbor rooms to active room list
        int neighborCount = lvlLevel::GetSingleton()->GetNeighborCount(instance->GetRoomId());
        
        int* activeRooms = static_cast<int*>(alloca((neighborCount + 1) * 4));
        activeRooms[0] = instance->GetRoomId();

        int neighborRoomCount = lvlLevel::GetSingleton()->GetNeighbors(&activeRooms[1], instance->GetRoomId());     

        for (int i = 0; i < neighborRoomCount + 1 && GetActiveRoomCount() < MAX_ROOMS; i++) 
        {
            int roomId = activeRooms[i];

            bool alreadyContained = false;
            for (int j = 0; j < this->GetActiveRoomCount() && !alreadyContained; j++) 
            {
                alreadyContained |= this->GetActiveRoomId(j) == roomId;
            }

            if (!alreadyContained) 
            {
                this->ActiveRoomIds[NumActiveRooms++] = roomId;
            }
        }
    }
    if (priority == 4)
    {
        dgPhysManager::PlayerInst = instance;
    }

    NumActiveMovers++;
}

AGE_API bool MM2::dgPhysManager::TrivialCollideInstances(lvlInstance* instanceA, lvlInstance* instanceB)
{
    return hook::Thunk<0x04692E0>::Call<bool>(this, instanceA, instanceB);
}

AGE_API bool MM2::dgPhysManager::CollideInstances(lvlInstance* instanceA, lvlInstance* instanceB)
{
    return hook::Thunk<0x469620>::Call<bool>(this, instanceA, instanceB);
}

AGE_API bool MM2::dgPhysManager::Collide(lvlSegment& segment, lvlIntersection* intersection, int room, lvlInstance* ignoreInstance, int instFlags, int instExcludeFlags)
{
    physTotalProbes++;

    auto probeTimer = Timer::Timer();
    bool res = hook::Thunk<0x468E40>::Call<bool>(this, &segment, intersection, room, ignoreInstance, instFlags, instExcludeFlags);

    float probeTime = (Timer::Ticks() - probeTimer.TickCount) * Timer::TicksToMilliseconds;
    this->TotalProbeTime += probeTime;
    if (this->IsUpdating == FALSE)
    {
        this->ExternalProbes++;
        this->ExternalProbeTime += probeTime;
    }

    return res;
}

void MM2::dgPhysManager::Reset()
{
    hook::Thunk<0x4682D0>::Call<void>(this);
}

void MM2::dgPhysManager::Update()
{
    auto totalUpdateTimer = Timer::Timer();
    auto physObjPairCollectingTimer = Timer::Timer();

    this->IsUpdating = TRUE;

    // detach movers in non active rooms
    for (int i = 0; i < NumActiveMovers; i++) 
    {
        auto entry = &Table[i];
        if (entry->GetPriority() == 1) 
        {
            bool inActiveRoom = false;
            int entryRoom = entry->GetInstance()->GetRoomId();
            for (int j = 0; j < NumActiveRooms && !inActiveRoom; j++)
            {
                inActiveRoom |= (ActiveRoomIds[j] == entryRoom);
            }

            if (!inActiveRoom) 
            {
                NumCulledMovers++;
                entry->SetFlags(0);
                entry->GetInstance()->Detach();
            }
        }
    }

    // update instance flags
    for (int i = 0; i < NumActiveMovers; i++) 
    {
        auto entry = &Table[i];
        auto instance = entry->GetInstance();
        instance->SetFlags(instance->GetFlags() & 32767);
    }

    // pre-update
    for (int i = 0; i < NumActiveMovers; i++) 
    {
        auto entry = &Table[i];
        if (entry->GetFlags() & 1)
        {
            auto entity = entry->GetEntity();
            entity->PreUpdate();
        }   
    }

    dgPhysManager::perfObjPairCollecting.set((Timer::Ticks() - physObjPairCollectingTimer.TickCount) * Timer::TicksToMilliseconds);
    auto totalCollisionTimer = Timer::Timer();

    // compute samples
    int wantedNumSamples = static_cast<int>(ceilf((datTimeManager::Seconds.get() - 0.001f) / this->SampleStep));
    int numSamples = wantedNumSamples;

    if (wantedNumSamples >= MaxSamples)
    {
        numSamples = MaxSamples;
    }

    float stepTime = numSamples * SampleStep;
    TimeDiscrepancy.set(stepTime - datTimeManager::Seconds.get() - TimeDiscrepancy.get());
    datTimeManager::SetTempOversampling(true, numSamples);

    // run samples
    for (int i = 0; i < numSamples; i++)
    {
        // update
        auto moverUpdateTimer = Timer::Timer();
        for (int j = 0; j < NumActiveMovers; j++)
        {
            auto entry = &Table[j];
            if (entry->GetFlags() & 1)
            {
                auto entity = entry->GetEntity();
                entity->Update();
            }
        }
        perfMoverUpdate.set(perfMoverUpdate.get() + ((Timer::Ticks() - moverUpdateTimer.TickCount) * Timer::TicksToMilliseconds));

        // gather collidables
        auto gatheringTimer = Timer::Timer();
        for (int j = 0; j < NumActiveMovers; j++)
        {
            auto entry = &Table[j];
            if (entry->GetFlags() & 0xA)
            {
                entry->ClearCollidables();
                this->GatherCollidables(entry);
            }
        }
        perfPhysGathering.set(perfPhysGathering.get() + ((Timer::Ticks() - gatheringTimer.TickCount) * Timer::TicksToMilliseconds));

        // collide
        auto collisionTimer = Timer::Timer();
        for (int j = 0; j < NumActiveMovers; j++)
        {
            auto entry = &Table[j];
            
            if (entry->GetFlags() & 0x2) 
            {
                lastCollidedInstance = entry->GetInstance()->GetName();
                this->CollideTerrain(entry);
            }

            // collide instances ahead
            if (entry->GetFlags() & 0x10)
            {
                auto collider = entry->GetEntity()->GetCollider();
                auto joint = collider->GetJoint();

                for (int k = j + 1; k < NumActiveMovers; k++)
                {
                    auto entryOther = &Table[k];
                    if (entryOther->GetFlags() & 0x10)
                    {
                        auto colliderOther = entryOther->GetEntity()->GetCollider();
                        auto jointOther = colliderOther->GetJoint();
                        
                        // don't collide instances attached to the same joint
                        if (joint == nullptr || joint->IsBroken() || joint != jointOther)
                        {
                            if (this->TrivialCollideInstances(entry->GetInstance(), entryOther->GetInstance())) 
                            {
                                lastCollidedInstance = entry->GetInstance()->GetName();
                                lastCollidedInstance2 = entryOther->GetInstance()->GetName();
                                this->CollideInstances(entry->GetInstance(), entryOther->GetInstance());
                            }
                        }
                    }
                }
            }

            // collide collidables
            if (entry->GetFlags() & 0xA)
            {
                for (int k = 0; k < entry->GetCollidablesCount(); k++)
                {
                    auto collidable = entry->GetCollidable(k);
                    if (collidable != nullptr)
                    {
                        lastCollidedInstance = entry->GetInstance()->GetName();
                        lastCollidedInstance2 = collidable->GetName();
                        this->CollideInstances(entry->GetInstance(), collidable);
                    }
                }
            }
        }

        perfPhysCollide.set(perfPhysCollide.get() + ((Timer::Ticks() - collisionTimer.TickCount) * Timer::TicksToMilliseconds));

        // update entry flags
        for (int j = 0; j < NumActiveMovers; j++)
        {
            auto entry = &Table[j];
            short flags = entry->GetFlags();
            if (flags & 256)
                flags = 27;
            entry->SetFlags(flags);
        }

        // HACK: bangers do something weird in both the base game and my physmgr
        // working around it but this should really be a proper fix
        for (int j = 0; j < NumActiveMovers; j++)
        {
            auto entry = &Table[j];
            short flags = entry->GetFlags();
            if (flags & 1 && entry->GetEntity() == nullptr) 
            {
                Errorf("dgPhysManager hack fix was triggered for %s", entry->GetInstance()->GetName());
                entry->SetFlags(0);
            }
        }

        // update collider matrix
        for (int j = 0; j < NumActiveMovers; j++)
        {
            auto entry = &Table[j];
            if (entry->GetFlags() & 1) 
            {
                auto collider = entry->GetEntity()->GetCollider();
                collider->UpdateMtx();
            }
        }
    }

    perfTotalCollisionTime.set((Timer::Ticks() - totalCollisionTimer.TickCount) * Timer::TicksToMilliseconds);
    datTimeManager::SetTempOversampling(false, numSamples);

    // post-update
    auto postCollisionTimer = Timer::Timer();
    for (int i = 0; i < NumActiveMovers; i++)
    {
        auto entry = &Table[i];
        if (entry->GetFlags() & 1)
        {
            auto entity = entry->GetEntity();
            entity->PostUpdate();
        }
    }
    perfPostCollision.set((Timer::Ticks() - postCollisionTimer.TickCount) * Timer::TicksToMilliseconds);

    // calculate stats
    this->MoverVsCollidable = 0;
    this->MoverVsMover = 0;

    perfTotalUpdateTime = ((Timer::Ticks() - totalUpdateTimer.TickCount) * Timer::TicksToMilliseconds);
    for (int i = 0; i < NumActiveMovers; i++)
    {
        auto entry = &Table[i];
        if (entry->GetFlags() != 0)
        {
            MoverVsCollidable += entry->GetCollidablesCount();
        }
    }

    // clean-up
    this->NumMovers = this->NumActiveMovers;
    this->NumSamples = numSamples;
    this->NumActiveRooms = 0;
    this->ResetTable();
    this->IsUpdating = FALSE;
}


void dgPhysManager::BindLua(LuaState L) {
    LuaBinding(L).beginClass<dgPhysManager>("dgPhysManager")
        //statics
        .addStaticProperty("Instance", [] { return (dgPhysManager*)Instance; })

        //properties
        .addPropertyReadOnly("Stats", &GetStats)

        .addPropertyReadOnly("NumActiveMovers", &GetActiveMoverCount)
        .addPropertyReadOnly("NumMovers", &GetLastActiveMoverCount)
        .addPropertyReadOnly("NumActiveRooms", &GetActiveRoomCount)
        .addPropertyReadOnly("NumCulledMovers", &GetNumCulledMovers)
        .addPropertyReadOnly("NumSamples", &GetNumSamples)

        .addProperty("MaxSamples", &GetMaxSamples, &SetMaxSamples)
        .addProperty("SampleStep", &GetSampleStep, &SetSampleStep)

        .addStaticProperty("Gravity", &GetGravity, &SetGravity)
        .addStaticProperty("PlayerInst", [] { return (lvlInstance*)dgPhysManager::PlayerInst; })

        //functions
        .addFunction("Collide", &collideLua)
        .addFunction("IgnoreMover", &IgnoreMover)
        .addFunction("DeclareMover", &DeclareMover)

        .addFunction("FindMover", &FindMover)
        .addFunction("GetMover", &GetMover)
        .addFunction("GetActiveRoomID", &GetActiveRoomId)

        .endClass();
}