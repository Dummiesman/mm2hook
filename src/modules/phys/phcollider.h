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
        byte unknown[0xB0];
    public:
        phJoint* GetJoint()
        {
            return *getPtr<phJoint*>(this, 0xAC);
        }
    };

    // Lua initialization

}