#pragma once
#include <mm2_common.h>

namespace MM2
{
    // Forward declarations
    class aiPath;

    // External declarations
    extern class aiIntersection;

    // Class definitions

    class aiPath {
    private:
        byte _buffer[0x164];
    protected:
        static hook::Field<0x8, short> _numSections;
        static hook::Field<0xA, short> _id;
        static hook::Field<0xC, short> _flags;
        static hook::Field<0xE, short> _numRooms;
        static hook::Field<0x10, short*> _rooms;
        static hook::Field<0x14, float> _halfWidth;
        static hook::Field<0x18, float> _baseSpeedLimit;
        static hook::Field<0x24, int> _numSharpTurns;
        static hook::Field<0x74, Vector3*> _lLaneVertices;
        static hook::Field<0x78, Vector3*> _lCableCarVertices;
        static hook::Field<0x7C, Vector3*> _lSubwayVertices;
        static hook::Field<0x80, Vector3*> _lBoundaryVertices;
        static hook::Field<0x38, short> _lNumLanes;
        static hook::Field<0x3E, short> _lNumSidewalks;
        static hook::Field<0xD8, Vector3*> _rLaneVertices;
        static hook::Field<0xDC, Vector3*> _rCableCarVertices;
        static hook::Field<0xE0, Vector3*> _rSubwayVertices;
        static hook::Field<0xE4, Vector3*> _rBoundaryVertices;
        static hook::Field<0x9C, short> _rNumLanes;
        static hook::Field<0xA2, short> _rNumSidewalks;
        static hook::Field<0x104, Vector3*> _sectionVerts;
        static hook::Field<0x108, Vector3*> _sectionOriX;
        static hook::Field<0x10C, Vector3*> _sectionOriY;
        static hook::Field<0x110, Vector3*> _sectionOriZ;
        static hook::Field<0x118, aiIntersection*> _intersectionA;
        static hook::Field<0x13C, aiIntersection*> _intersectionB;
    private:
        //lua helper
        std::tuple<int, float> luaIsPosOnRoad(Vector3 const& pos, float margin) const;
    public:
        aiPath(void)                                        DONOTCALL;
        aiPath(const aiPath &&)                             DONOTCALL;

        float GetBaseSpeedLimit() const;
        int GetId() const;
        int GetFlags() const;
        aiIntersection* GetIntersection(int num) const;
        int GetSidewalkCount(int side) const;
        int GetLaneCount(int side) const;
        Vector3 GetCableCarVertex(int section, int side) const;
        Vector3 GetSubwayVertex(int section, int side) const;
        Vector3 GetLaneVertex(int section, int lane, int side) const;
        Vector3 GetSidewalkVertex(int section, int lane, int side) const;
        Vector3 GetCenterVertex(int section) const;
        Vector3 GetSideDirection(int section) const;
        Vector3 GetUpDirection(int section) const;
        Vector3 GetForwardDirection(int section) const;
        Vector3 GetLeftBoundary(int section) const;
        Vector3 GetRightBoundary(int section) const;
        float GetWidth() const;
        int GetRoomCount() const;
        int GetRoomId(int index) const;

        /*
            aiPath
        */
        //AGE_API float SharpTurnAngle(int turnIndex, bool oppositeWinding) const;
        //AGE_API const Vector3 & SharpTurnCenter(int turnIndex, bool oppositeWinding) const;
        //AGE_API float SharpTurnDir(int turnIndex, bool oppositeWinding) const;
        //AGE_API const Vector3& SharpTurnStartDir(int turnIndex, bool oppositeWinding) const;
        //AGE_API const Vector3& SharpTurnEndDir(int turnIndex, bool oppositeWinding) const;
        

        AGE_API float CenterDist(Vector3 const& pos) const;
        AGE_API float CenterLength(int startIdx, int endIdx) const;
        AGE_API int CenterIndex(float distance) const;
        AGE_API void ClearAmbients();
        AGE_API void ClearPeds();
        AGE_API bool HasCableCarLine(int roadSide) const;
        AGE_API bool HasSubwayLine(int roadSide) const;
        AGE_API int IsPosOnRoad(Vector3 const& pos, float margin, float* outDistanceFromCenter) const;
        AGE_API int NumVerts() const;
        AGE_API int Index(Vector3 const& pos) const;
        AGE_API int Lane(Vector3 const& pos, int roadSide) const;
        AGE_API void UpdatePedestrians();
        AGE_API int RoadVertice(Vector3 const& position, int side) const;
        AGE_API bool Direction(Matrix34 const& matrix) const;
        void Draw() const;
        void DrawNormals() const;
        void DrawId() const;

        static void BindLua(LuaState L);
    };

    ASSERT_SIZEOF(aiPath, 0x164);
}