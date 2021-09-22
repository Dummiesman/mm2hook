#pragma once
#include <modules\phys.h>

namespace MM2
{
    // Forward declarations
    class phBoundSphere;

    // External declarations


    // Class definitions
    class phBoundSphere : public phBound
    {
    private:
        float Radius;
        phMaterial Material;

    public:
        inline float getRadius()
        {
            return this->Radius;
        }

        inline void setRadius(float radius)
        {
            this->Radius = radius;
        }

        static void BindLua(LuaState L) {
            LuaBinding(L).beginExtendClass<phBoundSphere, phBound>("phBoundSphere")
                .addProperty("Radius", &getRadius, &setRadius)
                .endClass();
        }
    };
    ASSERT_SIZEOF(phBoundSphere, 0x80);

    // Lua initialization

}