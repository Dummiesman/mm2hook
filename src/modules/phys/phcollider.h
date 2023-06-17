#pragma once
#include "phcolliderbase.h"

namespace MM2
{
    // Forward declarations
    class phCollider;

    // External declarations
    extern class phJoint;

    // Class definitions

    class phCollider : public phColliderBase {
    private:
        InstanceData InstanceData;
        int dword_a8;
        phJoint* Joint;
    public:
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