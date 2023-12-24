#pragma once

#include "ArrayList.h"

#include <modules\node\node.h>

#include "aiCityData.h"
#include "aiData.h"
#include "aiIntersection.h"
#include "aiPath.h"
#include "aiPedestrian.h"
#include "aiPoliceForce.h"
#include "aiSubway.h"
#include "aiVehicleMGR.h"
#include "aiVehicleAmbient.h"
#include "aiVehiclePlayer.h"

namespace MM2
{
    // Forward declarations
    struct aiMapStats;
    class aiMap;
    
    // External declarations
    extern class mcHookman;
    extern class aiCableCar;
    extern class aiRouteRacer;
    extern class aiCTFRacer; 
    extern class aiPoliceOfficer;

    // Class definitions
    enum class aiMapComponentType
    {
        None = 0x0,
        Road = 0x1,
        Shortcut = 0x2,
        Intersection = 0x3,
    };


    struct aiMapStats {
    public:
        float _fSubwayUpdate;
        float _fCableCarUpdate;
        float _fCTFOppUpdate;
        float _fPedUpdate;
        float _fAmbientUpdate;
        float _fCopUpdate;
        int _nPedQty;
        float _fOppUpdate;
        int _nAmbientQty;
        float _fTotUpdate;

        static void BindLua(LuaState L) {
            LuaBinding(L).beginClass<aiMapStats>("aiMapStats")
                .addVariable("SubwayUpdateTime", &aiMapStats::_fSubwayUpdate, false)
                .addVariable("CableCarUpdateTime", &aiMapStats::_fCableCarUpdate, false)
                .addVariable("CTFOpponentUpdateTime", &aiMapStats::_fCTFOppUpdate, false)
                .addVariable("PedUpdateTime", &aiMapStats::_fPedUpdate, false)
                .addVariable("AmbientUpdateTime", &aiMapStats::_fAmbientUpdate, false)
                .addVariable("CopUpdateTime", &aiMapStats::_fCopUpdate, false)
                .addVariable("OpponentUpdateTime", &aiMapStats::_fOppUpdate, false)
                .addVariable("PedQuantity", &aiMapStats::_nPedQty, false)
                .addVariable("AmbientQuantity", &aiMapStats::_nAmbientQty, false)
                .addVariable("TotalUpdateTime", &aiMapStats::_fTotUpdate, false)
                .endClass();
        }
    };

    struct aiMapComponentListEntry
    {
        int Id;
        aiMapComponentType Type;
    };


    struct aiMapComponentList
    {
        ushort Count;
        aiMapComponentListEntry** Components;
    };


    class aiMap : public asNode {
    public:
        aiVehicleManager *vehicleManager;

        aiRaceData *raceData;
        aiCityData *cityData;

        aiIntersection **intersections;
        int numIntersections;

        aiPath **paths;
        int numPaths;
        int pathDebug; // educated guess (unused)

        aiVehicleAmbient **ambients;
        float trafficDensity;
        int numAmbientVehicles;
        aiVehicleSpline *ambientSpline;

        aiPoliceForce *policeForce;
        aiPoliceOfficer **policeOfficers;
        short numCops;
        short numCopStartPos;
        short copStartPos[10];

        aiRouteRacer *opponents;
        int numOpponents;
        aiCTFRacer *ctfRacers;
        int numCTFRacers;
        mcHookman *hookmen;
        int numHookmen;
        
        aiPedestrian **pedestrians;
        int numPedestrians;
        aiPedestrian *lastPed;
        
        int unk_8C;
        
        int numCableCars;
        aiCableCar **cableCars;
        
        int numSubways;
        aiSubway **subways;
        void *subwayData;

        short unk_A4;
        short unk_A6;

        short gameMode;
        short numLaps;
        short numShortcuts;
        short numPlayers;

        aiVehiclePlayer players[4];
        aiVehiclePlayer *lastPlayer;

        CArrayList **arrayList_1;
        CArrayList **arrayList_2;

        aiPath *ambsPath;
        aiPath *pedsPath;

        void *routingNodes;
        void *lastRoutingNode;
        void *routingStuff;
        aiMapComponentList* componentMap;

        bool showHeadlights;

        float avoidThing_1;
        float avoidThing_2;

        Vector3 racerThing;
        Vector3 curWaypoint;

        short drawMap;
        short drawAppRoads;
        short drawPathIds;
        short unk_1BE;
        short unk_1C0;
        short unk_1C2;
        short pathThing_1; // passed to AllwaysGreen parameter
        short pathThing_2; // passed to AllwaysRed parameter
        short unk_1C8;

        short numAmbients;
    private:
        static std::vector<std::string> ctfOpponentNames; // extension to allow specificying ctf opp names
        std::vector<int> calcRouteLua(const Matrix34& srcMatrix, const Vector3& destPosition, bool shortestPath);
        std::tuple<int, int> mapComponentTypeLua(int room);
        std::tuple<int, int> mapComponentLua(const Vector3& position, int room);
        std::tuple<int, int, int> positionToAIMapCompLua(const Vector3& position);
    public:
        static int GetCTFOpponentTypeCount();
        static LPCSTR GetCTFOpponentType(int id);
        static void SetCTFOpponentTypes(std::vector<std::string> types);

        aiCityData* GetCityData() const;
        aiRaceData* GetRaceData() const;
        aiPoliceForce* GetPoliceForce() const;
        int GetCableCarCount() const;
        int GetPoliceCount() const;
        int GetHookmanCount() const;
        int GetPedestrianCount() const;
        int GetCtfRacerCount() const;
        int GetPlayerCount() const;
        int GetOpponentCount() const;
        int GetAmbientCount() const;
        int GetPathsCount() const;
        int GetShortcutCount() const;
        int GetIntersectionCount() const;

        aiMapStats GetStats() const;
    protected:
        static hook::Type<aiMap> Instance;
    private:
        static hook::Type<int> SignalClock;
        // Profiling Stats
        static hook::Type<float> _fSubwayUpdate;
        static hook::Type<float> _fCableCarUpdate;
        static hook::Type<float> _fCTFOppUpdate;
        static hook::Type<float> _fPedUpdate;
        static hook::Type<float> _fAmbientUpdate;
        static hook::Type<float> _fCopUpdate;
        static hook::Type<int>   _nPedQty;
        static hook::Type<float> _fOppUpdate;
        static hook::Type<int>   _nAmbientQty;
        static hook::Type<float> _fTotUpdate;
    public:
        static aiMap* GetInstance();

        /*
            asNode virtuals
        */

        AGE_API void Cull() override;
        AGE_API void Update() override;
        AGE_API void UpdatePaused() override;
        AGE_API void Reset() override;
        
        /*
            aiMap
        */
        static bool GetSignalClock();
        AGE_API void Dump();
        AGE_API void TestProbes(BOOL a2);
        AGE_API void AddPedestrian(aiPedestrian* ped);
        AGE_API aiCableCar* CableCar(int num) const;
        AGE_API mcHookman* Hookman(int num) const;
        AGE_API aiRouteRacer* Opponent(int num) const;
        AGE_API aiCTFRacer* CTFOpponent(int num) const;
        AGE_API aiPoliceOfficer* Police(int num) const;
        AGE_API aiVehiclePlayer* Player(int num) const;
        AGE_API aiVehicleAmbient* Vehicle(int num) const;
        AGE_API aiPedestrian* Pedestrian(int num) const;
        AGE_API aiIntersection* Intersection(int num) const;
        AGE_API aiPath* Path(int num) const;
        aiMapComponentType MapComponentType(int room, int* outId);
        int MapComponent(const Vector3& position, short* outId, short* outType, int room);
        BOOL PositionToAIMapComp(const Vector3& position, short* outId, short* outType, short* outRoom, short wantedRoadId);
        void CalcRoute(const Matrix34& srcMatrix, const Vector3& destPosition, const Vector3& unused, short* outIntersectionIds,
                       short* outIntersectionCount, short sourceRoom, short destRoom, bool shortestPath);

        static void BindLua(LuaState L);
    };

    ASSERT_SIZEOF(aiMap, 0x1CC);

    // Lua initialization

}