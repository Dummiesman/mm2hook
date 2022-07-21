#pragma once
#include <modules\ai.h>

#include "aiVehiclePhysics.h"

namespace MM2
{
    // Forward declarations
    class mcHookman;

    // External declarations


    // Class definitions

    class mcHookman : public aiVehiclePhysics {
    private:
        hook::Field<0x9794, short> _id;
        byte _buffer[0x100-1];
    public:
        mcHookman(void)                                     DONOTCALL;
        mcHookman(const mcHookman &&)                       DONOTCALL;

        int GetId() const
        {
            return _id.get(this);
        }

        static void BindLua(LuaState L) {
            LuaBinding(L).beginExtendClass<mcHookman, aiVehiclePhysics>("mcHookman")
                .addPropertyReadOnly("ID", &GetId)
                .endClass();
        }
    };

    ASSERT_SIZEOF(mcHookman, 0x9870);
}