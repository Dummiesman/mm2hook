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
        AGE_API bool IsEqual(const Vector2& vec) const;
        AGE_API void Normalize();
        AGE_API void Lerp(float t, const Vector2& vec1, const Vector2& vec2);
        AGE_API void Negate(const Vector2& vec);
        AGE_API void Negate();
        AGE_API void Min(const Vector2& vec1, const Vector2& vec2);
        AGE_API void Max(const Vector2& vec1, const Vector2& vec2);
        AGE_API void Print() const;
        AGE_API void Print(LPCSTR caption) const;

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