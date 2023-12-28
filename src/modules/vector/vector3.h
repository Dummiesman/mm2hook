#pragma once
#include <mm2_common.h>

namespace MM2
{
    // Forward declarations
    class Vector3;

    // External declarations
    extern class Vector4;
    extern class Vector2;
    extern class Matrix34;
    extern class Matrix44;

    // Class definitions
    class Vector3 {
    public:
        float X;
        float Y;
        float Z;
    public:
        const static Vector3 ORIGIN;
        const static Vector3 XAXIS;
        const static Vector3 YAXIS;
        const static Vector3 ZAXIS;
    public:
        AGE_API Vector3(void);
        AGE_API Vector3(float x, float y, float z);

        AGE_API float InvMag(void) const;
        AGE_API float Mag(void) const;
        AGE_API float Mag2(void) const;
        AGE_API void Set(float x, float y, float z);
        AGE_API void Set(const Vector3& vec);
        AGE_API void Add(const Vector3& vec);
        AGE_API void AddScaled(const Vector3& vec, float scale);
        AGE_API void Subtract(const Vector3& vec);
        AGE_API void SubtractScaled(const Vector3& vec, float scale);
        AGE_API void Scale(float scale);
        AGE_API void InvScale(float scale);
        AGE_API void RotateX(float angle);
        AGE_API void RotateY(float angle);
        AGE_API void RotateZ(float angle);
        AGE_API void RotateAboutAxis(float angle, int axis);
        AGE_API float Angle(const Vector3& vec) const;
        AGE_API float FastAngle(const Vector3& vec) const;
        AGE_API bool Approach(const Vector3& vec, float rate, float time);
        AGE_API void Cross(const Vector3& vec);
        AGE_API void Cross(const Vector3& vec1, const Vector3& vec2);
        AGE_API float Dist(const Vector3& vec) const;
        AGE_API float FlatDist(const Vector3& vec) const;
        AGE_API float Dot(const Vector3& vec) const;
        AGE_API void Dot(const Vector3& vec, const Matrix34& mtx);
        AGE_API void Dot3x3(const Matrix34& mtx);
        AGE_API void Dot3x3Transpose(const Matrix34& mtx);
        AGE_API void Lerp(float p1, const Vector3& vec1, const Vector3& vec2);
        AGE_API void Negate(const Vector3& vec);
        AGE_API void Normalize();
        AGE_API void HSVtoRGB();
        AGE_API void RGBtoHSV();
        AGE_API bool IsEqual(const Vector3& vec) const;
        AGE_API void Print() const;
        AGE_API void Print(LPCSTR caption);
        AGE_API void operator*=(const Vector3& vec);
        AGE_API void operator+=(const Vector3& vec);
        AGE_API void operator-=(const Vector3& vec);
        AGE_API void operator+=(Vector3& vec);
        AGE_API void operator-=(Vector3& vec);
        AGE_API Vector3 operator+(const Vector3& vec) const;
        AGE_API Vector3 operator-(const Vector3& vec) const;
        AGE_API Vector3 operator/(float value) const;
        AGE_API Vector3 operator*(float value) const;
        AGE_API Vector3 operator%(const Vector3& vec) const;

        explicit operator Vector2() const;
        explicit operator Vector4() const;

        static void BindLua(LuaState L);
    };

    ASSERT_SIZEOF(Vector3, 0xC);
}