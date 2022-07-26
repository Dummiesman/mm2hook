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
        static hook::Field<0x18, float> _baseSpeedLimit;
    private:
        //lua helper
        std::tuple<int, float> luaIsPosOnRoad(Vector3 const & pos, float margin) const
        {
            float outVal = 0.f;
            int posOnRoadVal = this->IsPosOnRoad(pos, margin, &outVal);
            return std::tuple<int, float>(posOnRoadVal, outVal);
        }
    public:
        aiPath(void)                                        DONOTCALL;
        aiPath(const aiPath &&)                             DONOTCALL;

        float GetBaseSpeedLimit() const
        {
            return _baseSpeedLimit.get(this);
        }

        /*
            aiPath
        */
        AGE_API float CenterLength(int startIdx, int endIdx)    { return hook::Thunk<0x547340>::Call<float>(this, startIdx, endIdx); }
        AGE_API void ClearAmbients()                            { hook::Thunk<0x547C00>::Call<void>(this); }
        AGE_API void ClearPeds()                                { hook::Thunk<0x547BD0>::Call<void>(this); }
        AGE_API bool HasCableCarLine(int roadSide) const        { return hook::Thunk<0x549980>::Call<bool>(this, roadSide); }
        AGE_API bool HasSubwayLine(int roadSide) const          { return hook::Thunk<0x5499B0>::Call<bool>(this, roadSide); }
        AGE_API int IsPosOnRoad(Vector3 const & pos, float margin, float* outDistanceFromCenter) const
                                                                { return hook::Thunk<0x548370>::Call<int>(this, &pos, margin, outDistanceFromCenter); }
        AGE_API int NumVerts()                                  { return _numVerts.get(this); }
        AGE_API int Lane(Vector3 & pos, int roadSide) const     { return hook::Thunk<0x547900>::Call<int>(this, &pos, roadSide); }
        AGE_API void UpdatePedestrians()                        { hook::Thunk<0x544150>::Call<void>(this); }

        static void BindLua(LuaState L) {
            LuaBinding(L).beginClass<aiPath>("aiPath")
                .addPropertyReadOnly("NumVerts", &NumVerts)
                .addFunction("CenterLength", &CenterLength)
                .addFunction("ClearAmbients", &ClearAmbients)
                .addFunction("ClearPeds", &ClearPeds)
                .addFunction("HasCableCarLine", &HasCableCarLine)
                .addFunction("HasSubwayLine", &HasSubwayLine)
                .addFunction("IsPosOnRoad", &luaIsPosOnRoad)
                .addFunction("Lane", &Lane)
                .addFunction("UpdatePedestrians", &UpdatePedestrians)
                .endClass();
        }
    };

    ASSERT_SIZEOF(aiPath, 0x164);
}