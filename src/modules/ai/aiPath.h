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
        static hook::Field<0x14, float> _halfWidth;
        static hook::Field<0x18, float> _baseSpeedLimit;
        static hook::Field<0x118, aiIntersection*> _intersectionA;
        static hook::Field<0x13C, aiIntersection*> _intersectionB;
        static hook::Field<0x74, Vector3*> _lLaneVertices;
        static hook::Field<0x38, short> _lNumLanes;
        static hook::Field<0x3E, short> _lNumSidewalks;
        static hook::Field<0xD8, Vector3*> _rLaneVertices;
        static hook::Field<0x9C, short> _rNumLanes;
        static hook::Field<0xA2, short> _rNumSidewalks;
        static hook::Field<0x104, Vector3*> _sectionVerts;
        static hook::Field<0x108, Vector3*> _sectionOriX;
        static hook::Field<0x10C, Vector3*> _sectionOriY;
        static hook::Field<0x110, Vector3*> _sectionOriZ;
    private:
        //lua helper
        std::tuple<int, float> luaIsPosOnRoad(Vector3 const& pos, float margin) const;
    public:
        aiPath(void)                                        DONOTCALL;
        aiPath(const aiPath &&)                             DONOTCALL;

        float GetBaseSpeedLimit() const;
        int GetId() const;
        aiIntersection* GetIntersection(int num) const;
        int GetSidewalkCount(int side) const;
        int GetLaneCount(int side) const;
        Vector3 GetLaneVertex(int section, int lane, int side) const;
        Vector3 GetSidewalkVertex(int section, int lane, int side) const;
        Vector3 GetCenterVertex(int section) const;
        Vector3 GetSideDirection(int section) const;
        Vector3 GetUpDirection(int section) const;
        Vector3 GetForwardDirection(int section) const;
        float GetWidth() const;

        /*
            aiPath
        */
        AGE_API float CenterLength(int startIdx, int endIdx) const;
        AGE_API void ClearAmbients();
        AGE_API void ClearPeds();
        AGE_API bool HasCableCarLine(int roadSide) const;
        AGE_API bool HasSubwayLine(int roadSide) const;
        AGE_API int IsPosOnRoad(Vector3 const& pos, float margin, float* outDistanceFromCenter) const;
        AGE_API int NumVerts() const;
        AGE_API int Lane(Vector3& pos, int roadSide) const;
        AGE_API void UpdatePedestrians();

        static void BindLua(LuaState L);
    };

    ASSERT_SIZEOF(aiPath, 0x164);
}