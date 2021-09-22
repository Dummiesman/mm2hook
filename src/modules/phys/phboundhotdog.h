#pragma once
#include <modules\phys\phmaterial.h>
#include <modules\phys\phbound.h>

namespace MM2
{
    // Forward declarations
    class phBoundHotdog;

    // External declarations
    extern class phMaterial;

    // Class definitions
    class phBoundHotdog : public phBound
    {
    private:
        float Radius;
        float Height;
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

        inline float getHeight()
        {
            return this->Height;
        }

        inline void setHeight(float height)
        {
            this->Height = height;
        }

        static void BindLua(LuaState L) {
            LuaBinding(L).beginExtendClass<phBoundHotdog, phBound>("phBoundHotdog")
                .addProperty("Radius", &getRadius, &setRadius)
                .addProperty("Height", &getHeight, &setHeight)
                .endClass();
        }
    };
    ASSERT_SIZEOF(phBoundHotdog, 0x84);


    // Lua initialization

}