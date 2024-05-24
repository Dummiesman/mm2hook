#pragma once
#include <modules\creature.h>

namespace MM2
{
    // Forward declarations
    class dgRagdoll;

    // External declarations
    extern class phInertialCS;

    // Class definitions

    //TODO
    class dgRagdoll {
    private:
        phInertialCS* ICS;
        void* Data; /*dgRagdollData*/
        int dword_08;
        void* Links; /*dgLink*/
        int dword_10;
        bool Active;
    public:
        AGE_API Matrix34* GetMatrix()                       { return hook::Thunk<0x5A7D70>::Call<Matrix34*>(this); }
    };


    // Lua initialization

}