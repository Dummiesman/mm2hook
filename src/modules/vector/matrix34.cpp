#pragma once
#include "matrix34.h"

namespace MM2
{
    Matrix34::Matrix34() {}
    Matrix34::Matrix34(float m00, float m01, float m02, float m10, float m11, float m12, float m20, float m21, float m22, float m30, float m31, float m32) 
    : m00(m00), m01(m01), m02(m02), m10(m10), m11(m11), m12(m12), m20(m20), m21(m21), m22(m22), m30(m30), m31(m31), m32(m32) {}

    const Matrix34 Matrix34::I = Matrix34(1.0, 0.0, 0.0, 0.0, 1.0, 0.0, 0.0, 0.0, 1.0, 0.0, 0.0, 0.0);

    void Matrix34::MakeRotate(Vector3 const& axis, float angle)
    {
        hook::Thunk<0x4BCFA0>::Call<void>(this, &axis, angle);
    }

    void Matrix34::MakeScale(float xScale, float yScale, float zScale) {
        this->m00 = xScale;
        this->m01 = 0.0;
        this->m02 = 0.0;
        this->m10 = 0.0;
        this->m11 = yScale;
        this->m12 = 0.0;
        this->m20 = 0.0;
        this->m21 = 0.0;
        this->m22 = zScale;
    }

    void Matrix34::MakeScale(float scale) {
        this->m00 = scale;
        this->m01 = 0.0;
        this->m02 = 0.0;
        this->m10 = 0.0;
        this->m11 = scale;
        this->m12 = 0.0;
        this->m20 = 0.0;
        this->m21 = 0.0;
        this->m22 = scale;
    }

    void Matrix34::MakeRotateZ(float angle) {
        this->m02 = 0.0;
        float v2 = sinf(angle);
        float v3 = cosf(angle);
        this->m00 = v3;
        this->m01 = v2;
        this->m10 = -v2;
        this->m12 = 0.0;
        this->m11 = v3;
        this->m20 = 0.0;
        this->m21 = 0.0;
        this->m22 = 1.0;
    }

    void Matrix34::MakeRotateY(float angle) {
        float v2 = cosf(angle);
        this->m01 = 0.0;
        float v3 = sinf(angle);
        this->m00 = v2;
        this->m02 = -v3;
        this->m10 = 0.0;
        this->m11 = 1.0;
        this->m12 = 0.0;
        this->m21 = 0.0;
        this->m20 = v3;
        this->m22 = v2;
    }

    void Matrix34::MakeRotateX(float angle) {
        this->m00 = 1.0;
        this->m01 = 0.0;
        this->m02 = 0.0;
        this->m10 = 0.0;
        float v2 = sinf(angle);
        float v3 = cosf(angle);
        this->m11 = v3;
        this->m12 = v2;
        this->m21 = -v2;
        this->m20 = 0.0;
        this->m22 = v3;
    }

    void Matrix34::Rotate(Vector3 const& axis, float angle)
    {
        hook::Thunk<0x4BCD70>::Call<void>(this, &axis, angle);
    }

    void Matrix34::RotateFull(Vector3 const& axis, float angle)
    {
        hook::Thunk<0x4BCE60>::Call<void>(this, &axis, angle);
    }

    void Matrix34::RotateX(float angle) {
        Matrix34 rotMatrix = Matrix34();
        rotMatrix.MakeRotateX(angle);
        this->Dot3x3(rotMatrix);
    }

    void Matrix34::RotateY(float angle) {
        Matrix34 rotMatrix = Matrix34();
        rotMatrix.MakeRotateY(angle);
        this->Dot3x3(rotMatrix);
    }

    void Matrix34::RotateZ(float angle) {
        Matrix34 rotMatrix = Matrix34();
        rotMatrix.MakeRotateZ(angle);
        this->Dot3x3(rotMatrix);
    }

    void Matrix34::RotateFullX(float angle) {
        Matrix34 rotMatrix = Matrix34();
        rotMatrix.MakeRotateX(angle);
        rotMatrix.SetRow(3, Vector3::ORIGIN);
        this->Dot(rotMatrix);
    }

    void Matrix34::RotateFullY(float angle) {
        Matrix34 rotMatrix = Matrix34();
        rotMatrix.MakeRotateY(angle);
        rotMatrix.SetRow(3, Vector3::ORIGIN);
        this->Dot(rotMatrix);
    }

    void Matrix34::RotateFullZ(float angle) {
        Matrix34 rotMatrix = Matrix34();
        rotMatrix.MakeRotateZ(angle);
        rotMatrix.SetRow(3, Vector3::ORIGIN);
        this->Dot(rotMatrix);
    }

    void Matrix34::Add(const Matrix34& values) {
        this->m00 += values.m00;
        this->m01 += values.m01;
        this->m02 += values.m02;
        this->m10 += values.m10;
        this->m11 += values.m11;
        this->m12 += values.m12;
        this->m20 += values.m20;
        this->m21 += values.m21;
        this->m22 += values.m22;
        this->m30 += values.m30;
        this->m31 += values.m31;
        this->m32 += values.m32;
    }

    void Matrix34::Normalize() {
        hook::Thunk<0x4BEE90>::Call<void>(this);
    }

    void Matrix34::LookAt(const Vector3& from, const Vector3& to)
    {
        hook::Thunk<0x4BEB20>::Call<void>(this, &from, &to);
    }

    void Matrix34::Zero() {
        this->m02 = 0.0;
        this->m01 = 0.0;
        this->m00 = 0.0;
        this->m12 = 0.0;
        this->m11 = 0.0;
        this->m10 = 0.0;
        this->m22 = 0.0;
        this->m21 = 0.0;
        this->m20 = 0.0;
        this->m32 = 0.0;
        this->m31 = 0.0;
        this->m30 = 0.0;
    }

    void Matrix34::Dot3x3(const Matrix34& rhs) {
        hook::Thunk<0x4BC970>::Call<void>(this, &rhs);
    }
    void Matrix34::Dot(const Matrix34& rhs) {
        hook::Thunk<0x4BC400>::Call<void>(this, &rhs);
    }

    void Matrix34::Scale(float amount) {
        this->m00 = amount * this->m00;
        this->m01 = amount * this->m01;
        this->m02 = amount * this->m02;
        this->m10 = amount * this->m10;
        this->m11 = amount * this->m11;
        this->m12 = amount * this->m12;
        this->m20 = amount * this->m20;
        this->m21 = amount * this->m21;
        this->m22 = amount * this->m22;
    }

    void Matrix34::Scale(float xAmount, float yAmount, float zAmount) {
        this->m00 = xAmount * this->m00;
        this->m10 = xAmount * this->m10;
        this->m20 = xAmount * this->m20;
        this->m01 = yAmount * this->m01;
        this->m11 = yAmount * this->m11;
        this->m21 = yAmount * this->m21;
        this->m02 = zAmount * this->m02;
        this->m12 = zAmount * this->m12;
        this->m22 = zAmount * this->m22;
    }

    void Matrix34::ScaleFull(float amount) {
        this->m00 = amount * this->m00;
        this->m01 = amount * this->m01;
        this->m02 = amount * this->m02;
        this->m10 = amount * this->m10;
        this->m11 = amount * this->m11;
        this->m12 = amount * this->m12;
        this->m20 = amount * this->m20;
        this->m21 = amount * this->m21;
        this->m22 = amount * this->m22;
        this->m30 = amount * this->m30;
        this->m31 = amount * this->m31;
        this->m32 = amount * this->m32;
    }

    void Matrix34::ScaleFull(float xAmount, float yAmount, float zAmount) {
        this->m00 = xAmount * this->m00;
        this->m10 = xAmount * this->m10;
        this->m20 = xAmount * this->m20;
        this->m30 = xAmount * this->m30;
        this->m01 = yAmount * this->m01;
        this->m11 = yAmount * this->m11;
        this->m21 = yAmount * this->m21;
        this->m31 = yAmount * this->m31;
        this->m02 = zAmount * this->m02;
        this->m12 = zAmount * this->m12;
        this->m22 = zAmount * this->m22;
        this->m32 = zAmount * this->m32;
    }

    void Matrix34::Identity() {
        this->m00 = 1.0;
        this->m01 = 0.0;
        this->m02 = 0.0;
        this->m10 = 0.0;
        this->m11 = 1.0;
        this->m12 = 0.0;
        this->m20 = 0.0;
        this->m21 = 0.0;
        this->m22 = 1.0;
        this->m30 = 0.0;
        this->m31 = 0.0;
        this->m32 = 0.0;
    }

    void Matrix34::Identity3x3() {
        this->m00 = 1.0;
        this->m01 = 0.0;
        this->m02 = 0.0;
        this->m10 = 0.0;
        this->m11 = 1.0;
        this->m12 = 0.0;
        this->m20 = 0.0;
        this->m21 = 0.0;
        this->m22 = 1.0;
    }

    void Matrix34::Transform(const Vector3& vector, Vector3& out) const
    {
        out.X = this->m00 * vector.X + this->m10 * vector.Y + this->m20 * vector.Z + this->m30;
        out.Y = this->m01 * vector.X + this->m11 * vector.Y + this->m21 * vector.Z + this->m31;
        out.Z = this->m02 * vector.X + this->m12 * vector.Y + this->m22 * vector.Z + this->m32;
    }

    Vector3 Matrix34::Transform(const Vector3& vector) const
    {
        Vector3 returnVec;
        Transform(vector, returnVec);
        return returnVec;
    }

    void Matrix34::Transform3x3(const Vector3& vector, Vector3& out) const
    {
        out.X = this->m00 * vector.X + this->m10 * vector.Y + this->m20 * vector.Z;
        out.Y = this->m01 * vector.X + this->m11 * vector.Y + this->m21 * vector.Z;
        out.Z = this->m02 * vector.X + this->m12 * vector.Y + this->m22 * vector.Z;
    }

    Vector3 Matrix34::Transform3x3(const Vector3& vector) const
    {
        Vector3 returnVec;
        Transform3x3(vector, returnVec);
        return returnVec;
    }

    Vector4 Matrix34::GetColumn(int column) const
    {
        switch (column)
        {
        case 0:
            return Vector4(this->m00, this->m10, this->m20, this->m30);
        case 1:
            return Vector4(this->m01, this->m11, this->m21, this->m31);
        case 2:
            return Vector4(this->m02, this->m12, this->m22, this->m32);
        default:
            return Vector4();
        }
    }

    Vector3 Matrix34::GetRow(int row) const
    {
        switch (row)
        {
        case 0:
            return Vector3(this->m00, this->m01, this->m02);
        case 1:
            return Vector3(this->m10, this->m11, this->m12);
        case 2:
            return Vector3(this->m20, this->m21, this->m22);
        case 3:
            return Vector3(this->m30, this->m31, this->m32);
        default:
            return Vector3();
        }
    }

    void Matrix34::SetColumn(int column, const Vector4& value)
    {
        switch (column)
        {
        case 0:
            this->m00 = value.X;
            this->m10 = value.Y;
            this->m20 = value.Z;
            this->m30 = value.W;
            break;
        case 1:
            this->m01 = value.X;
            this->m11 = value.Y;
            this->m21 = value.Z;
            this->m31 = value.W;
            break;
        case 2:
            this->m02 = value.X;
            this->m12 = value.Y;
            this->m22 = value.Z;
            this->m32 = value.W;
            break;
        }
    }

    void Matrix34::SetRow(int row, const Vector3& value)
    {
        switch (row)
        {
        case 0:
            this->m00 = value.X;
            this->m01 = value.Y;
            this->m02 = value.Z;
            break;
        case 1:
            this->m10 = value.X;
            this->m11 = value.Y;
            this->m12 = value.Z;
            break;
        case 2:
            this->m20 = value.X;
            this->m21 = value.Y;
            this->m22 = value.Z;
            break;
        case 3:
            this->m30 = value.X;
            this->m31 = value.Y;
            this->m32 = value.Z;
            break;
        }
    }

    AGE_API void Matrix34::Set(const Matrix34 &a1) { hook::Thunk<0x4BBFB0>::Call<void>(this, &a1); }

    void Matrix34::BindLua(LuaState L) {
        LuaBinding(L).beginClass<Matrix34>("Matrix34")
            .addFactory([](float m00 = 1.0, float m01 = 0.0, float m02 = 0.0,
                float m10 = 0.0, float m11 = 1.0, float m12 = 0.0,
                float m20 = 0.0, float m21 = 0.0, float m22 = 1.0,
                float m30 = 0.0, float m31 = 0.0, float m32 = 0.0) {
            return Matrix34(m00, m01, m02, m10, m11, m12, m20, m21, m22, m30, m31, m32);
        }, LUA_ARGS(_opt<float>, _opt<float>, _opt<float>, _opt<float>,
            _opt<float>, _opt<float>, _opt<float>, _opt<float>,
            _opt<float>, _opt<float>, _opt<float>, _opt<float>))
            .addVariable("m00", &Matrix34::m00)
            .addVariable("m01", &Matrix34::m01)
            .addVariable("m02", &Matrix34::m02)
            .addVariable("m10", &Matrix34::m10)
            .addVariable("m11", &Matrix34::m11)
            .addVariable("m12", &Matrix34::m12)
            .addVariable("m20", &Matrix34::m20)
            .addVariable("m21", &Matrix34::m21)
            .addVariable("m22", &Matrix34::m22)
            .addVariable("m30", &Matrix34::m30)
            .addVariable("m31", &Matrix34::m31)
            .addVariable("m32", &Matrix34::m32)

            .addFunction("GetColumn", &GetColumn)
            .addFunction("GetRow", &GetRow)
            .addFunction("SetColumn", &SetColumn)
            .addFunction("SetRow", &SetRow)

            .addFunction("Identity", &Matrix34::Identity)
            .addFunction("Identity3x3", &Matrix34::Identity3x3)
            .addFunction("Scale", static_cast<void(Matrix34::*)(float, float, float)>(&Matrix34::Scale))
            .addFunction("Normalize", &Matrix34::Normalize)
            .addFunction("Dot", &Matrix34::Dot)
            .addFunction("Rotate", &Matrix34::Rotate)
            .addFunction("RotateFull", &Matrix34::RotateFull)

            .addFunction("Zero", &Matrix34::Zero)
            .addFunction("MakeRotateX", &Matrix34::MakeRotateX)
            .addFunction("MakeRotateY", &Matrix34::MakeRotateY)
            .addFunction("MakeRotateZ", &Matrix34::MakeRotateZ)
            .addFunction("MakeScale", static_cast<void(Matrix34::*)(float, float, float)>(&Matrix34::MakeScale))
            .addFunction("MakeRotate", &Matrix34::MakeRotate)
            .addFunction("Transform", static_cast<Vector3(Matrix34::*)(const Vector3&)const>(&Matrix34::Transform))
            .addFunction("Transform3x3", static_cast<Vector3(Matrix34::*)(const Vector3&)const>(&Matrix34::Transform3x3))
            .addFunction("RotateX", &Matrix34::RotateX)
            .addFunction("RotateY", &Matrix34::RotateY)
            .addFunction("RotateZ", &Matrix34::RotateZ)
            .addFunction("RotateFullX", &Matrix34::RotateFullX)
            .addFunction("RotateFullY", &Matrix34::RotateFullY)
            .addFunction("RotateFullZ", &Matrix34::RotateFullZ)
            .addFunction("LookAt", &Matrix34::LookAt)

            .addFunction("Set", &Matrix34::Set)
            .endClass();
    }
}