#include "aiMap.h"
#include <modules\ai\aiCTFRacer.h>

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

    // Extensions
    std::vector<std::string> aiMap::ctfOpponentNames = { "vpauditt" };
    
    int aiMap::GetCTFOpponentTypeCount()
    {
        return ctfOpponentNames.size();
    }

    LPCSTR aiMap::GetCTFOpponentType(int id)
    {
        return ctfOpponentNames.at(id).c_str();
    }

    void aiMap::SetCTFOpponentTypes(std::vector<std::string> types)
    {
        if (types.size() < 1)
        {
            Errorf("SetCTFOpponentTypes requires at least one type. (got %i types)", types.size());
            return;
        }

        ctfOpponentNames.clear();
        ctfOpponentNames.assign(types.begin(), types.end());
    }

    // Lua Helpers
    static short routeCalcIntersections[1024];
    std::vector<int> aiMap::calcRouteLua(const Matrix34& srcMatrix, const Vector3& destPosition, bool shortestPath)
    {
        Vector3 blank = Vector3();
        short numIntersections = 0;
        this->CalcRoute(srcMatrix, destPosition, blank, routeCalcIntersections, &numIntersections, 0, 0, shortestPath);

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

    std::tuple<aiPath*, bool> aiMap::detRdSegBetweenIntsLua(aiIntersection* intersectionA, aiIntersection* intersectionB)
    {
        bool outArg = false;
        auto path = this->DetRdSegBetweenInts(intersectionA, intersectionB, &outArg);
        return std::make_tuple(path, outArg);
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

    aiRaceData* aiMap::GetRaceData() const {
        return raceData;
    }

    aiCityData* aiMap::GetCityData() const {
        return cityData;
    }

    aiPoliceForce* aiMap::GetPoliceForce() const  {
        return policeForce;
    }

    int aiMap::GetCableCarCount() const
    {
        return numCableCars;
    }

    int aiMap::GetPoliceCount() const  {
        return numCops;
    }

    int aiMap::GetHookmanCount() const  {
        return numHookmen;
    }

    int aiMap::GetPedestrianCount() const  {
        return numPedestrians;
    }

    int aiMap::GetCtfRacerCount() const  {
        return numCTFRacers;
    }

    int aiMap::GetPlayerCount() const  {
        return numPlayers;
    }

    int aiMap::GetOpponentCount() const  {
        return numOpponents;
    }

    int aiMap::GetAmbientCount() const  {
        return numAmbientVehicles;
    }

    int aiMap::GetPathsCount() const  {
        return numPaths;
    }

    int aiMap::GetShortcutCount() const {
        return numShortcuts;
    }
    
    int aiMap::GetIntersectionCount() const {
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
    AGE_API void aiMap::Dump()                                  { hook::Thunk<0x538840>::Call<void>(this); }
    AGE_API void aiMap::TestProbes(BOOL a2)                     { hook::Thunk<0x53B870>::Call<void>(this, a2); }
    AGE_API void aiMap::AddPedestrian(aiPedestrian* ped)        { hook::Thunk<0x539D30>::Call<void>(this, ped); }
    AGE_API aiCableCar* aiMap::CableCar(int num) const          { return hook::Thunk<0x534A30>::Call<aiCableCar *>(this, num); }
    AGE_API mcHookman * aiMap::Hookman(int num) const           { return hook::Thunk<0x5349E0>::Call<mcHookman *>(this, num); }
    AGE_API aiRouteRacer * aiMap::Opponent(int num) const       { return hook::Thunk<0x534940>::Call<aiRouteRacer *>(this, num); }
    
    AGE_API aiCTFRacer* aiMap::CTFOpponent(int num) const
    {
        if (num >= 0 && num < this->numCTFRacers)
            return &this->ctfRacers[num];
        Warningf("Returning a NULL CTFOpponent. Idx: %d", num);
        return nullptr;
    }

    AGE_API aiPoliceOfficer * aiMap::Police(int num) const      { return hook::Thunk<0x5348F0>::Call<aiPoliceOfficer *>(this, num); }
    AGE_API aiVehiclePlayer * aiMap::Player(int num) const      { return hook::Thunk<0x534AF0>::Call<aiVehiclePlayer *>(this, num); }
    AGE_API aiVehicleAmbient * aiMap::Vehicle(int num) const    { return hook::Thunk<0x5348B0>::Call<aiVehicleAmbient *>(this, num); }
    AGE_API aiPedestrian * aiMap::Pedestrian(int num) const     { return hook::Thunk<0x534AB0>::Call<aiPedestrian *>(this, num); }
    AGE_API aiIntersection* aiMap::Intersection(int num) const  { return hook::Thunk<0x534880>::Call<aiIntersection*>(this, num); }
    AGE_API aiPath* aiMap::Path(int num) const                  { return hook::Thunk<0x534850>::Call<aiPath*>(this, num); }
    AGE_API aiPath* aiMap::DetRdSegBetweenInts(aiIntersection* intersectionA, aiIntersection* intersectionB, bool* outRdEndsAtB)
                                                                { return hook::Thunk<0x53A680>::Call<aiPath*>(this, intersectionA, intersectionB, outRdEndsAtB); }
    aiMapComponentType aiMap::MapComponentType(int room, int* outId)
                                                                { return hook::Thunk<0x537600>::Call<aiMapComponentType>(this, room, outId); }
    int aiMap::MapComponent(const Vector3& position, short* outId, short* outType, int room)
                                                                { return hook::Thunk<0x537680>::Call<int>(this, &position, outId, outType, room); }
    BOOL aiMap::PositionToAIMapComp(const Vector3& position, short* outId, short* outType, short* outRoom, short wantedRoadId)
                                                                { return hook::Thunk<0x5377B0>::Call<BOOL>(this, &position, outId, outType, outRoom, wantedRoadId); }
    void aiMap::CalcRoute(const Matrix34& srcMatrix, const Vector3& destPosition, const Vector3& unused, short* outIntersectionIds,
        short* outIntersectionCount, short sourceRoom, short destRoom, bool shortestPath)
                                                                { hook::Thunk<0x53A7A0>::Call<void>(this, &srcMatrix, &destPosition, &unused, outIntersectionIds, outIntersectionCount, sourceRoom, destRoom, shortestPath); }

    void aiMap::BindLua(LuaState L) {
        LuaBinding(L).beginExtendClass<aiMap, asNode>("aiMap")
            .addStaticFunction("SetCTFOpponentTypes", &SetCTFOpponentTypes)
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
            .addFunction("DetRdSegBetweenInts", &detRdSegBetweenIntsLua)
            .addPropertyReadOnly("CityData", &GetCityData)
            .addPropertyReadOnly("RaceData", &GetRaceData)
            .addPropertyReadOnly("PoliceForce", &GetPoliceForce)
            .addPropertyReadOnly("Stats", &GetStats)
            .addPropertyReadOnly("NumAmbientVehicles", &GetAmbientCount)
            .addPropertyReadOnly("NumPaths", &GetPathsCount)
            .addPropertyReadOnly("NumShortcuts", &GetShortcutCount)
            .addPropertyReadOnly("NumIntersections", &GetIntersectionCount)
            .addPropertyReadOnly("NumPedestrians", &GetPedestrianCount)
            .addPropertyReadOnly("NumCableCars", &GetCableCarCount)
            .addPropertyReadOnly("NumHookmen", &GetHookmanCount)
            .addPropertyReadOnly("NumOpponents", &GetOpponentCount)
            .addPropertyReadOnly("NumPlayers", &GetPlayerCount)
            .addPropertyReadOnly("NumCTFRacers", &GetCtfRacerCount)
            .addPropertyReadOnly("NumPolice", &GetPoliceCount)
            .addVariable("ShowHeadlights", &aiMap::showHeadlights)
            .addStaticProperty("Instance", &aiMap::GetInstance)
        .endClass();
    }
}