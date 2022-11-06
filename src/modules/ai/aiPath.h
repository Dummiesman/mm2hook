#pragma once
#include <mm2_common.h>

namespace MM2
{
    // Forward declarations
    class aiPath;

    // External declarations


    // Class definitions

    class aiPath {
    private:
        byte _buffer[0x164];
    protected:
        static hook::Field<0x8, short> _numVerts;
        static hook::Field<0xA, short> _id;
        static hook::Field<0x18, float> _baseSpeedLimit;
    private:
        //lua helper
        std::tuple<int, float> luaIsPosOnRoad(Vector3 const& pos, float margin) const;
    public:
        aiPath(void)                                        DONOTCALL;
        aiPath(const aiPath &&)                             DONOTCALL;

        float GetBaseSpeedLimit() const;
        int GetId() const;

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