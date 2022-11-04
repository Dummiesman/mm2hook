#include "bridge.h"

namespace MM2
{
    /*
        gizBridge
    */
    AGE_API gizBridge::gizBridge() {
        scoped_vtable x(this);
        hook::Thunk<0x577410>::Call<void>(this);
    }

    AGE_API gizBridge::~gizBridge() {
        hook::Thunk<0x577490>::Call<void>(this);
    }

    declfield(gizBridge::LiftSpeed)(0x5DBFA4);
    declfield(gizBridge::GoalAngle)(0x5DBFA8);
    declfield(gizBridge::DownInterval)(0x5DBFAC);
    declfield(gizBridge::UpInterval)(0x5DBFB0);
    declfield(gizBridge::ProximityDist2)(0x5DBFB4);

    /*
        lvlInstance virtuals
    */
    AGE_API void gizBridge::Reset() {
        hook::Thunk<0x5774F0>::Call<void>(this);
    }

    AGE_API void gizBridge::Draw(int lod) {
        hook::Thunk<0x578240>::Call<void>(this, lod);
    }
        
    AGE_API unsigned int gizBridge::SizeOf() {
        return hook::Thunk<0x578250>::Call<unsigned int>(this);
    }

    /*
        gizBridge
    */
    AGE_API void gizBridge::Init(LPCSTR name, Matrix34 const & matrix) {
        hook::Thunk<0x577530>::Call<void>(this, name, &matrix);
    }

    int gizBridge::GetState() const {
        return _state.get(this);
    };

    gizBridgeType gizBridge::GetType() const { return static_cast<gizBridgeType>(_type.get(this)); }
    void gizBridge::SetType(gizBridgeType type) { _type.set(this, static_cast<int>(type)); }

    float gizBridge::GetLiftSpeed() { return gizBridge::LiftSpeed.get(); }
    void gizBridge::SetLiftSpeed(float speed) { gizBridge::LiftSpeed.set(speed); }

    float gizBridge::GetGoalAngle() { return gizBridge::GoalAngle.get(); }
    void gizBridge::SetGoalAngle(float angle) { gizBridge::GoalAngle.set(angle); }

    float gizBridge::GetDownInterval() { return gizBridge::DownInterval.get(); }
    void gizBridge::SetDownInterval(float interval) { gizBridge::DownInterval.set(interval); }

    float gizBridge::GetUpInterval() { return gizBridge::UpInterval.get(); }
    void gizBridge::SetUpInterval(float interval) { gizBridge::UpInterval.set(interval); }

    float gizBridge::GetProximityDist() { return sqrtf(gizBridge::ProximityDist2.get()); }
    void gizBridge::SetProximityDist(float dist) { gizBridge::ProximityDist2.set(dist * dist); }

    void gizBridge::BindLua(LuaState L) {
        LuaBinding(L).beginExtendClass<gizBridge, dgUnhitMtxBangerInstance>("gizBridge")
            .addStaticProperty("LiftSpeed", &GetLiftSpeed, &SetLiftSpeed)
            .addStaticProperty("GoalAngle", &GetGoalAngle, &SetGoalAngle)
            .addStaticProperty("DownInterval", &GetDownInterval, &SetDownInterval)
            .addStaticProperty("UpInterval", &GetUpInterval, &SetUpInterval)
            .addStaticProperty("ProximityDist", &GetProximityDist, &SetProximityDist)
            .addPropertyReadOnly("State", &GetState)
            .addProperty("Type", &GetType, &SetType)
            .endClass();
    }

    /*
        gizBridgeMgr
    */
    int gizBridgeMgr::GetBridgeCount() const
    {
        return this->BridgeCount;
    }

    gizBridge* gizBridgeMgr::GetBridge(int num)
    {
        if (num < 0 || num >= this->GetBridgeCount())
            return nullptr;
        return &this->Bridges[num];
    }

    void gizBridgeMgr::BindLua(LuaState L) {
        LuaBinding(L).beginExtendClass<gizBridgeMgr, asNode>("gizBridgeMgr")
            .addPropertyReadOnly("NumBridges", &GetBridgeCount)
            .addFunction("GetBridge", &GetBridge)
            .endClass();
    }
}