#pragma once
#include <mm2_common.h>

namespace MM2
{
    // Forward declarations
    class Vector2;

    // External declarations
    extern class Vector3;

    // Class definitions
    class Vector2 {
    public:
        float X;
        float Y;
    public:
        AGE_API Vector2();
        AGE_API Vector2(float x, float y);

        AGE_API float Dist(const Vector2& vec);
        AGE_API float Mag(void) const;
        AGE_API float Mag2(void) const;
        AGE_API void Set(float x, float y);
        AGE_API void Set(const Vector2& vec);
        bool IsEqual(const Vector2& vec) const;
        AGE_API void Normalize();

        Vector2 operator+(Vector2& vec);
        Vector2 operator-(const Vector2 vec) const;
        Vector2 operator/(float value) const;
        Vector2 operator*(float value) const;
        
        explicit operator Vector3() const;

        static void BindLua(LuaState L);
    };

    ASSERT_SIZEOF(Vector2, 0x8);

    // Lua initialization

}