#pragma once
#include <mm2_common.h>

namespace MM2
{
    // Forward declarations
    class asLineSparks;

    // External declarations

    // Class definitions
    class asLineSparks : public Base {
    private:
        byte _buffer[0x5C];
    protected:
        static hook::Field<0x34, int> _numActive;
    public:
        AGE_API asLineSparks()                           { hook::Thunk<0x460690>::Call<void>(this); }

        AGE_API void Init(int count, LPCSTR textureName) { hook::Thunk<0x460740>::Call<void>(this, count, textureName); }
        AGE_API void RadialBlast(int count, Vector3 & position, Vector3 & velocity)
                                                         { hook::Thunk<0x460830>::Call<void>(this, count, &position, &velocity); }

        int GetNumActive() const { return _numActive.get(this); }

        static void BindLua(LuaState L) {
            LuaBinding(L).beginClass<asLineSparks>("asLineSparks")
                .addFunction("Init", &Init)
                .addFunction("RadialBlast", &RadialBlast)
                .addPropertyReadOnly("NumActive", &GetNumActive)
                .endClass();
        }
    };

    ASSERT_SIZEOF(asLineSparks, 0x60);
}