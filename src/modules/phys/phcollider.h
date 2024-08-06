#pragma once
#include "phcolliderbase.h"

namespace MM2
{
    // Forward declarations
    class phCollider;

    // External declarations
    extern class phJoint;
    extern class phBound;

    // Class definitions

    class phCollider : public phColliderBase {
    private:
        InstanceData InstanceData;
        int dword_a8;
        phJoint* Joint;
    public:
        void Init(Matrix34* matrixPtr, phBound const* bound)                { hook::Thunk<0x46D870>::Call<void>(this, matrixPtr, bound); }
        void Init(phBound const* bound, phInertialCS* ics, phSleep* sleep)  { hook::Thunk<0x46D740>::Call<void>(this, bound, ics, sleep); }

        phJoint* GetJoint()
        {
            return *getPtr<phJoint*>(this, 0xAC);
        }

        static void BindLua(LuaState L) {
            LuaBinding(L).beginExtendClass<phCollider, phColliderBase>("phCollider")
                .endClass();
        }
    };

    // Lua initialization

}