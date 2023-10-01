#pragma once
#include <mm2_common.h>

namespace MM2
{
    // Forward declarations
    class Matrix44;

    // External declarations    
    extern class Vector4;

    // Class definitions

    class Matrix44 {
    private:
        Matrix34 toMatrix34Lua() const;
    public:
        float m00;
        float m01;
        float m02;
        float m03;

        float m10;
        float m11;
        float m12;
        float m13;

        float m20;
        float m21;
        float m22;
        float m23;

        float m30;
        float m31;
        float m32;
        float m33;
    public:
        const static Matrix44 I;
        const static Matrix44 ScaleZ; // Not the most appropriate place for it but it's being used in multiple places
    public:
        Matrix44();
        Matrix44(float m00, float m01, float m02, float m03, 
                 float m10, float m11, float m12, float m13, 
                 float m20, float m21, float m22, float m23, 
                 float m30, float m31, float m32, float m33);

        Vector4 GetColumn(int column) const;
        Vector4 GetRow(int row) const;
        void SetColumn(int column, const Vector4& value);
        void SetRow(int row, const Vector4& value);

        AGE_API void Identity(void);
        AGE_API void Transpose(void);
        AGE_API void Zero(void);
        AGE_API void Hermite(void);
        AGE_API void Bezier(void);
        AGE_API void CatmullRom(void);
        AGE_API float Determinant(void) const;
        AGE_API void Add(const Matrix44* a1);
        AGE_API void AddScaled(const Matrix44* a1, float a2);
        AGE_API void Dot(const Matrix44& a1);
        AGE_API void FastInverse(const Matrix44& a1);
        AGE_API void FromMatrix34(const Matrix34& a1);
        AGE_API void InvertTo(const Matrix44* a1);
        AGE_API void MakeRotX(float a1);
        AGE_API void MakeRotY(float a1);
        AGE_API void MakeRotZ(float a1);
        AGE_API void Print(LPCSTR name) const;
        AGE_API void Set(const Matrix44* a1);
        AGE_API void Subtract(const Matrix44* a1);
        AGE_API void ToMatrix34(Matrix34& a1) const;
        AGE_API void Transform4(const Vector3* a1, const Vector4* a2, int a3);
        AGE_API void Transform3x3(const Vector3& vector, Vector3& out) const;
        AGE_API Vector3 Transform3x3(const Vector3& vector) const;
        AGE_API static void Convert(Matrix44& output, const Matrix34& input);

        static void BindLua(LuaState L);
    };

    ASSERT_SIZEOF(Matrix44, 0x40);
}