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

        //operators


        //function
        void MakeRotate(Vector3 const& axis, float angle)
        {
            hook::Thunk<0x4BCFA0>::Call<void>(this, &axis, angle);
        }

        void MakeScale(float xScale, float yScale, float zScale) {
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

        void MakeScale(float scale) {
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

        void MakeRotateZ(float angle) {
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

        void MakeRotateY(float angle) {
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

        void MakeRotateX(float angle) {
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

        void Rotate(Vector3 const& axis, float angle)
        {
            hook::Thunk<0x4BCD70>::Call<void>(this, &axis, angle);
        }

        void RotateFull(Vector3 const& axis, float angle)
        {
            hook::Thunk<0x4BCE60>::Call<void>(this, &axis, angle);
        }

        void RotateX(float angle) {
            Matrix34 rotMatrix = Matrix34();
            rotMatrix.MakeRotateX(angle);
            this->Dot(rotMatrix);
        }

        void RotateY(float angle) {
            Matrix34 rotMatrix = Matrix34();
            rotMatrix.MakeRotateY(angle);
            this->Dot(rotMatrix);
        }

        void RotateZ(float angle) {
            Matrix34 rotMatrix = Matrix34();
            rotMatrix.MakeRotateZ(angle);
            this->Dot(rotMatrix);
        }

        void Add(Matrix34* values) {
            this->m00 = values->m00 + this->m00;
            this->m01 = values->m01 + this->m01;
            this->m02 = values->m02 + this->m02;
            this->m10 = values->m10 + this->m10;
            this->m11 = values->m11 + this->m11;
            this->m12 = values->m12 + this->m12;
            this->m20 = values->m20 + this->m20;
            this->m21 = values->m21 + this->m21;
            this->m22 = values->m22 + this->m22;
            this->m30 = values->m30 + this->m30;
            this->m31 = values->m31 + this->m31;
            this->m32 = values->m32 + this->m32;
        }

        void Normalize() {
            hook::Thunk<0x4BEE90>::Call<void>(this);
        }

        void LookAt(const Vector3& from, const Vector3& to)
        {
            hook::Thunk<0x4BEB20>::Call<void>(this, &from, &to);
        }

        void Zero() {
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

        void Dot(const Matrix34& rhs) {
            float v3 = rhs.m11 * this->m01 + this->m02 * rhs.m21 + rhs.m01 * this->m00;
            float v15 = rhs.m12 * this->m01 + rhs.m22 * this->m02 + rhs.m02 * this->m00;
            float v14 = rhs.m10 * this->m11 + this->m12 * rhs.m20 + this->m10 * rhs.m00;
            float v13 = rhs.m11 * this->m11 + this->m12 * rhs.m21 + rhs.m01 * this->m10;
            float v12 = rhs.m12 * this->m11 + rhs.m02 * this->m10 + rhs.m22 * this->m12;
            float v11 = rhs.m10 * this->m21 + this->m20 * rhs.m00 + rhs.m20 * this->m22;
            float v10 = rhs.m11 * this->m21 + rhs.m01 * this->m20 + rhs.m21 * this->m22;
            float v9 = rhs.m22 * this->m22 + rhs.m12 * this->m21 + rhs.m02 * this->m20;
            float v8 = rhs.m10 * this->m31 + this->m30 * rhs.m00 + this->m32 * rhs.m20 + rhs.m30;
            float v7 = rhs.m11 * this->m31 + rhs.m01 * this->m30 + this->m32 * rhs.m21 + rhs.m31;
            float v6 = rhs.m22 * this->m32 + rhs.m12 * this->m31 + rhs.m02 * this->m30 + rhs.m32;
            float v4 = rhs.m10 * this->m01 + rhs.m00 * this->m00;
            float v5 = this->m02 * rhs.m20;
            this->m02 = v15;
            this->m00 = v4 + v5;
            this->m01 = v3;
            this->m10 = v14;
            this->m11 = v13;
            this->m12 = v12;
            this->m20 = v11;
            this->m21 = v10;
            this->m22 = v9;
            this->m30 = v8;
            this->m31 = v7;
            this->m32 = v6;
        }

        void Scale(float amount) {
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

        void Scale(float xAmount, float yAmount, float zAmount) {
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

        void ScaleFull(float amount) {
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

        void ScaleFull(float xAmount, float yAmount, float zAmount) {
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

        void Identity() {
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

        void Identity3x3() {
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

        void Transform(const Vector3& vector, Vector3& out)
        {
            out.X = this->m00 * vector.X + this->m10 * vector.Y + this->m20 * vector.Z + this->m30;
            out.Y = this->m01 * vector.X + this->m11 * vector.Y + this->m21 * vector.Z + this->m31;
            out.Z = this->m02 * vector.X + this->m12 * vector.Y + this->m22 * vector.Z + this->m32;
        }

        Vector3 Transform(const Vector3& vector)
        {
            Vector3 returnVec;
            Transform(vector, returnVec);
            return returnVec;
        }

        Vector4 GetColumn(int column)
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

        Vector3 GetRow(int row)
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

        void SetColumn(int column, Vector4 value)
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

        void SetRow(int row, Vector3 value)
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

        AGE_API void Set(const Matrix34* a1) { hook::Thunk<0x4BBFB0>::Call<void>(this, a1); }

        static void BindLua(LuaState L) {
            LuaBinding(L).beginClass<Matrix34>("Matrix34")
                .addFactory([](float m00 = 1.0, float m01 = 0.0, float m02 = 0.0,
                    float m10 = 0.0, float m11 = 1.0, float m12 = 0.0,
                    float m20 = 0.0, float m21 = 0.0, float m22 = 1.0,
                    float m30 = 0.0, float m31 = 0.0, float m32 = 0.0) {
                auto mtx = Matrix34();
                mtx.m00 = m00;
                mtx.m01 = m01;
                mtx.m02 = m02;

                mtx.m10 = m10;
                mtx.m11 = m11;
                mtx.m12 = m12;

                mtx.m20 = m20;
                mtx.m21 = m21;
                mtx.m22 = m22;

                mtx.m30 = m30;
                mtx.m31 = m31;
                mtx.m32 = m32;
                return mtx;
            }, LUA_ARGS(_opt<float>, _opt<float>, _opt<float>, _opt<float>,
                _opt<float>, _opt<float>, _opt<float>, _opt<float>,
                _opt<float>, _opt<float>, _opt<float>, _opt<float>))
                .addVariableRef("m00", &Matrix34::m00)
                .addVariableRef("m01", &Matrix34::m01)
                .addVariableRef("m02", &Matrix34::m02)
                .addVariableRef("m10", &Matrix34::m10)
                .addVariableRef("m11", &Matrix34::m11)
                .addVariableRef("m12", &Matrix34::m12)
                .addVariableRef("m20", &Matrix34::m20)
                .addVariableRef("m21", &Matrix34::m21)
                .addVariableRef("m22", &Matrix34::m22)
                .addVariableRef("m30", &Matrix34::m30)
                .addVariableRef("m31", &Matrix34::m31)
                .addVariableRef("m32", &Matrix34::m32)

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
                .addFunction("Transform", static_cast<Vector3(Matrix34::*)(const Vector3&)>(&Matrix34::Transform))
                .addFunction("RotateX", &Matrix34::RotateX)
                .addFunction("RotateY", &Matrix34::RotateY)
                .addFunction("RotateZ", &Matrix34::RotateZ)
                .addFunction("LookAt", &Matrix34::LookAt)

                .addFunction("Set", &Matrix34::Set)
                .endClass();
        }
    };

    ASSERT_SIZEOF(Matrix34, 0x30);

    // Lua initialization

}