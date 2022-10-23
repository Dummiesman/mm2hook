#pragma once

namespace MM2
{
    // Forward declarations
    class phCollider;

    // External declarations
    extern class phJoint;

    // Class definitions

    class phCollider {
    private:
        byte unknown[0x30];
    public:
        int colliderId; //?

        phJoint* GetJoint()
        {
            return *getPtr<phJoint*>(this, 0xAC);
        }
    };

    // Lua initialization

}