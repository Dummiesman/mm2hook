#pragma once
#include <modules\level\inst.h>

namespace MM2
{
    struct lvlMultiRoomState
    {
        int LastSample;
        phBound* Bound;
        Matrix34* Matrix;
    };

    class lvlMultiRoomInstance : public lvlInstance
    {
    private:
        lvlInstance* m_SrcInstance;
        lvlMultiRoomState* m_State;
    public:
        static void Create(lvlInstance* srcInstance, int room)
        {
            hook::StaticThunk<0x467C40>::Call<void>(srcInstance, room);
        }
    };
}