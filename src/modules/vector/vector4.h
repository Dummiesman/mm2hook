#pragma once
#include <mm2_common.h>

namespace MM2
{
    // Forward declarations
    class Vector4;

    // External declarations
    extern class Vector3;    
    extern class Matrix44;

    // Class definitions

    class Vector4 {
    public:
        float X;
        float Y;
        float Z;
        float W;
    public:
        AGE_API Vector4();
        AGE_API Vector4(float x, float y, float z, float w);

        AGE_API void Set(float x, float y, float z, float w);
        AGE_API void Cross(const Vector4& vec1, const Vector4& vec2);
        AGE_API void Subtract(const Vector3& vec1, const Vector3& vec2);
        AGE_API float Dot(const Vector4& vec) const;
        AGE_API float Dot3(const Vector4& vec) const;
        AGE_API Vector4& Dot(const Vector4& vec, Matrix44 const& mtx);
        AGE_API Vector4& Dot3x3(const Vector4& vec, Matrix44 const& mtx);
        AGE_API void ComputePlane(const Vector3& vec1, const Vector3& vec2, const Vector3& vec3);
        AGE_API void ComputePlane(const Vector3& vec1, const Vector3& vec2);
        AGE_API void Min(const Vector4& vec1, const Vector4& vec2);
        AGE_API void Max(const Vector4& vec1, const Vector4& vec2);

        static void BindLua(LuaState L);
    };

    ASSERT_SIZEOF(Vector4, 0x10);
}