#include "aiMap.h"

namespace MM2
{
    // Profiling Stats
    declfield(aiMap::_fSubwayUpdate)(0x6B2DEC);
    declfield(aiMap::_fCableCarUpdate)(0x6B2DF0);
    declfield(aiMap::_fCTFOppUpdate)(0x6B2DF4);
    declfield(aiMap::_fPedUpdate)(0x6B2DF8);
    declfield(aiMap::_fAmbientUpdate)(0x6B2DFC);
    declfield(aiMap::_fCopUpdate)(0x6B2E00);
    declfield(aiMap::_nPedQty)(0x6B2E08);
    declfield(aiMap::_fOppUpdate)(0x6B2FE4);
    declfield(aiMap::_nAmbientQty)(0x6B2FE8);
    declfield(aiMap::_fTotUpdate)(0x6B2FF4);

    declfield(aiMap::SignalClock)(0x6B31F4);
    declfield(aiMap::Instance)(0x6B2E10);
    
    // Lua Helpers
    static short routeCalcIntersections[1024];
    std::vector<int> aiMap::calcRouteLua(const Matrix34& srcMatrix, const Vector3& destPosition, bool shortestPath)
    {
        Vector3 blank = Vector3();
        short numIntersections = 0;
        this->CalcRoute(srcMatrix, destPosition, blank, routeCalcIntersections, &numIntersections, 0, 0, (shortestPath) ? TRUE : FALSE);

        std::vector<int> returnVec;
        for (int i = 0; i < (int)numIntersections; i++) {
            returnVec.push_back(routeCalcIntersections[i]);
        }
        return returnVec;
    }

    std::tuple<int, int> aiMap::mapComponentTypeLua(int room) {
        int outId;
        int componentType = static_cast<int>(this->MapComponentType(room, &outId));
        return std::make_tuple(componentType, outId);
    }

    std::tuple<int, int> aiMap::mapComponentLua(const Vector3& position, int room) {
        short outId, outType;
        this->MapComponent(position, &outId, &outType, room);
        return std::make_tuple((int)outType, (int)outId);
    }

    std::tuple<int, int, int> aiMap::positionToAIMapCompLua(const Vector3& position) {
        short outId, outType;
        short outRoom = 0;
        auto res = this->PositionToAIMapComp(position, &outId, &outType, &outRoom, -1);

        if (res == FALSE)
            return std::make_tuple((int)aiMapComponentType::None, 0, 0);
        return std::make_tuple((int)outType, (int)outId, (int)outRoom);
    }

    // Instance
    aiMap* aiMap::GetInstance() {
        return Instance.ptr();
    }

    // Properties
    bool aiMap::GetSignalClock()
    {
        return (SignalClock.get() & 8) != 0;
    }

    aiPoliceForce* aiMap::GetPoliceForce() {
        return policeForce;
    }

    int aiMap::GetPoliceCount() {
        return numCops;
    }

    int aiMap::GetHookmanCount() {
        return numHookmen;
    }

    int aiMap::GetPedestrianCount() {
        return numPedestrians;
    }

    int aiMap::GetCtfRacerCount() {
        return numCTFRacers;
    }

    int aiMap::GetPlayerCount() {
        return numPlayers;
    }

    int aiMap::GetOpponentCount() {
        return numOpponents;
    }

    int aiMap::GetAmbientCount() {
        return numAmbientVehicles;
    }

    int aiMap::GetPathsCount() {
        return numPaths;
    }

    int aiMap::GetIntersectionCount() {
        return numIntersections;
    }

    // Functions
    aiMapStats aiMap::GetStats() const {
        aiMapStats stats;
        stats._fSubwayUpdate = *this->_fSubwayUpdate;
        stats._fCableCarUpdate = *this->_fCableCarUpdate;
        stats._fCTFOppUpdate = *this->_fCTFOppUpdate;
        stats._fPedUpdate = *this->_fPedUpdate;
        stats._fAmbientUpdate = *this->_fAmbientUpdate;
        stats._fCopUpdate = *this->_fCopUpdate;
        stats._nPedQty = *this->_nPedQty;
        stats._fOppUpdate = *this->_fOppUpdate;
        stats._nAmbientQty = *this->_nAmbientQty;
        stats._fTotUpdate = *this->_fTotUpdate;
        return stats;
    }

   
    // asNode virtuals
    AGE_API void aiMap::Cull()                { hook::Thunk<0x5374F0>::Call<void>(this); }
    AGE_API void aiMap::Update()              { hook::Thunk<0x536E50>::Call<void>(this); }
    AGE_API void aiMap::UpdatePaused()        { hook::Thunk<0x5374E0>::Call<void>(this); }
    AGE_API void aiMap::Reset()               { hook::Thunk<0x536A30>::Call<void>(this); }
        
    // aiMap
    AGE_API void aiMap::Dump()                              { hook::Thunk<0x538840>::Call<void>(this); }
    AGE_API void aiMap::TestProbes(BOOL a2)                     { hook::Thunk<0x53B870>::Call<void>(this, a2); }
    AGE_API mcHookman * aiMap::Hookman(int num) const           { return hook::Thunk<0x5349E0>::Call<mcHookman *>(this, num); }
    AGE_API aiRouteRacer * aiMap::Opponent(int num) const       { return hook::Thunk<0x534940>::Call<aiRouteRacer *>(this, num); }
    AGE_API aiCTFRacer * aiMap::CTFOpponent(int num) const      { return hook::Thunk<0x534990>::Call<aiCTFRacer *>(this, num); }
    AGE_API aiPoliceOfficer * aiMap::Police(int num) const      { return hook::Thunk<0x5348F0>::Call<aiPoliceOfficer *>(this, num); }
    AGE_API aiVehiclePlayer * aiMap::Player(int num) const      { return hook::Thunk<0x534AF0>::Call<aiVehiclePlayer *>(this, num); }
    AGE_API aiVehicleAmbient * aiMap::Vehicle(int num) const    { return hook::Thunk<0x5348B0>::Call<aiVehicleAmbient *>(this, num); }
    AGE_API aiPedestrian * aiMap::Pedestrian(int num) const     { return hook::Thunk<0x534AB0>::Call<aiPedestrian *>(this, num); }
    AGE_API aiIntersection* aiMap::Intersection(int num) const  { return hook::Thunk<0x534880>::Call<aiIntersection*>(this, num); }
    AGE_API aiPath* aiMap::Path(int num) const                  { return hook::Thunk<0x534850>::Call<aiPath*>(this, num); }

    aiMapComponentType aiMap::MapComponentType(int room, int* outId)
                                                                { return hook::Thunk<0x537600>::Call<aiMapComponentType>(this, room, outId); }
    int aiMap::MapComponent(const Vector3& position, short* outId, short* outType, int room)
                                                                { return hook::Thunk<0x537680>::Call<int>(this, &position, outId, outType, room); }
    BOOL aiMap::PositionToAIMapComp(const Vector3& position, short* outId, short* outType, short* outRoom, short wantedRoadId)
                                                                { return hook::Thunk<0x5377B0>::Call<BOOL>(this, &position, outId, outType, outRoom, wantedRoadId); }
    void aiMap::CalcRoute(const Matrix34& srcMatrix, const Vector3& destPosition, const Vector3& unused, short* outIntersectionIds,
        short* outIntersectionCount, short sourceRoom, short destRoom, BOOL shortestPath)
                                                                { hook::Thunk<0x53A7A0>::Call<void>(this, &srcMatrix, &destPosition, &unused, outIntersectionIds, outIntersectionCount, sourceRoom, destRoom, shortestPath); }

    void aiMap::BindLua(LuaState L) {
        LuaBinding(L).beginExtendClass<aiMap, asNode>("aiMap")
            .addFunction("CalcRoute", &calcRouteLua)
            .addFunction("MapComponentType", &mapComponentTypeLua)
            .addFunction("MapComponent", &mapComponentLua)
            .addFunction("PositionToAIMapComp", &positionToAIMapCompLua)
            .addFunction("Dump", &Dump)
            .addFunction("TestProbes", &TestProbes, LUA_ARGS(bool))
            .addFunction("Pedestrian", &Pedestrian)
            .addFunction("Path", &Path)
            .addFunction("Police", &Police)
            .addFunction("Player", &Player)
            .addFunction("Hookman", &Hookman)
            .addFunction("Opponent", &Opponent)
            .addFunction("CTFOpponent", &CTFOpponent)
            .addFunction("Vehicle", &Vehicle)
            .addFunction("Intersection", &Intersection)
            .addPropertyReadOnly("PoliceForce", &GetPoliceForce)
            .addPropertyReadOnly("Stats", &GetStats)
            .addPropertyReadOnly("NumAmbientVehicles", &GetAmbientCount)
            .addPropertyReadOnly("NumPaths", &GetPathsCount)
            .addPropertyReadOnly("NumIntersections", &GetIntersectionCount)
            .addPropertyReadOnly("NumPedestrians", &GetPedestrianCount)
            .addPropertyReadOnly("NumHookmen", &GetHookmanCount)
            .addPropertyReadOnly("NumOpponents", &GetOpponentCount)
            .addPropertyReadOnly("NumPlayers", &GetPlayerCount)
            .addPropertyReadOnly("NumCTFRacers", &GetCtfRacerCount)
            .addPropertyReadOnly("NumPolice", &GetPoliceCount)
            .addVariableRef("ShowHeadlights", &aiMap::showHeadlights)
            .addStaticProperty("Instance", [] { return  &aiMap::Instance; })
        .endClass();
    }
}