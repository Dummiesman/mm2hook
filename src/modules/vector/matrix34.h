#pragma once
#include <mm2_common.h>

namespace MM2
{
    // Forward declarations
    class Matrix34;

    // External declarations
    extern class Vector3;
    extern class Vector4;

    // Class definitions
    class Matrix34 {
    public:
        float m00;
        float m01;
        float m02;

        float m10;
        float m11;
        float m12;

        float m20;
        float m21;
        float m22;

        float m30;
        float m31;
        float m32;
    public:
        const static Matrix34 I;
    public:
        Matrix34(void);
        Matrix34(float m00, float m01, float m02, float m10, float m11, float m12, float m20, float m21, float m22, float m30, float m31, float m32);

        void MakeRotate(Vector3 const& axis, float angle);
        void MakeScale(float xScale, float yScale, float zScale);
        void MakeScale(float scale);
        void MakeRotateZ(float angle);
        void MakeRotateY(float angle);
        void MakeRotateX(float angle);
        void Rotate(Vector3 const& axis, float angle);
        void RotateFull(Vector3 const& axis, float angle);
        void RotateX(float angle);
        void RotateY(float angle);
        void RotateZ(float angle);
        void RotateFullX(float angle);
        void RotateFullY(float angle);
        void RotateFullZ(float angle);
        void Add(const Matrix34& values);
        void Normalize();
        void LookAt(const Vector3& from, const Vector3& to);
        void Zero();
        void Dot(const Matrix34& rhs);
        void Dot3x3(const Matrix34& rhs);
        void Scale(float amount);
        void Scale(float xAmount, float yAmount, float zAmount);
        void ScaleFull(float amount);
        void ScaleFull(float xAmount, float yAmount, float zAmount);
        void Identity();
        void Identity3x3();
        void Transform(const Vector3& vector, Vector3& out) const;
        Vector3 Transform(const Vector3& vector) const;
        void Transform3x3(const Vector3& vector, Vector3& out) const;
        Vector3 Transform3x3(const Vector3& vector) const;
        Vector4 GetColumn(int column) const;
        Vector3 GetRow(int row) const;
        void SetColumn(int column, const Vector4& value);
        void SetRow(int row, const Vector3& value);
        AGE_API void Set(const Matrix34* a1);

        static void BindLua(LuaState L);
    };

    ASSERT_SIZEOF(Matrix34, 0x30);

    // Lua initialization

}