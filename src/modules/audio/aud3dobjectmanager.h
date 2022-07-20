#pragma once
#include <mm2_common.h>
#include <modules\node\node.h>

namespace MM2
{
    // Forward declarations
    class Aud3DObjectManager;

    // External declarations

    // Class definitions
    class Aud3DObjectManager : public asNode {
    private:
        byte _buffer[0x180];
    public:
        ANGEL_ALLOCATOR

        /*
            Aud3DObjectManager
        */

        static void BindLua(LuaState L) {
            LuaBinding(L).beginClass<Aud3DObjectManager>("Aud3DObjectManager")
                .endClass();
        }
    };

    ASSERT_SIZEOF(Aud3DObjectManager, 0x198);
}