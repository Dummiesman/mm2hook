#pragma once
#include "aiVehiclePhysics.h"

namespace MM2
{
    // Forward declarations
    class mcHookman;

    // External declarations


    // Class definitions

    class mcHookman : public aiVehiclePhysics {
    private:
        byte _buffer[0x100];
    protected:
        static hook::Field<0x9794, short> _id;
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