#include "matrix44.h"

namespace MM2
{
    Matrix44::Matrix44() {};
    Matrix44::Matrix44(float m00, float m01, float m02, float m03,
                       float m10, float m11, float m12, float m13,
                       float m20, float m21, float m22, float m23,
                       float m30, float m31, float m32, float m33)
        : m00(m00), m01(m01), m02(m02), m03(m03),
          m10(m10), m11(m11), m12(m12), m13(m13),
          m20(m20), m21(m21), m22(m22), m23(m23),
          m30(m30), m31(m31), m32(m32), m33(m33) {}

    const Matrix44 Matrix44::I = Matrix44(1.0, 0.0, 0.0, 0.0, 0.0, 1.0, 0.0, 0.0, 0.0, 0.0, 1.0, 0.0 ,0.0, 0.0, 0.0, 1.0);
    const Matrix44 Matrix44::ScaleZ = Matrix44(1.0, 0.0, 0.0, 0.0, 0.0, 1.0, 0.0, 0.0, 0.0, 0.0, -1.0, 0.0, 0.0, 0.0, 0.0, 1.0);

    Matrix34 Matrix44::toMatrix34Lua() const
    {
        Matrix34 returnMtx = Matrix34();
        this->ToMatrix34(returnMtx);
        return returnMtx;
    }
   
    Vector4 Matrix44::GetColumn(int column) const
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

    Vector4 Matrix44::GetRow(int row) const
    {
        switch (row)
        {
        case 0:
            return Vector4(this->m00, this->m01, this->m02, this->m03);
        case 1:
            return Vector4(this->m10, this->m11, this->m12, this->m13);
        case 2:
            return Vector4(this->m20, this->m21, this->m22, this->m23);
        case 3:
            return Vector4(this->m30, this->m31, this->m32, this->m33);
        default:
            return Vector4();
        }
    }

    void Matrix44::SetColumn(int column, const Vector4& value)
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

    void Matrix44::SetRow(int row, const Vector4& value)
    {
        switch (row)
        {
        case 0:
            this->m00 = value.X;
            this->m01 = value.Y;
            this->m02 = value.Z;
            this->m03 = value.W;
            break;
        case 1:
            this->m10 = value.X;
            this->m11 = value.Y;
            this->m12 = value.Z;
            this->m13 = value.W;
            break;
        case 2:
            this->m20 = value.X;
            this->m21 = value.Y;
            this->m22 = value.Z;
            this->m23 = value.W;
            break;
        case 3:
            this->m30 = value.X;
            this->m31 = value.Y;
            this->m32 = value.Z;
            this->m33 = value.W;
            break;
        }
    }

    AGE_API void Matrix44::Identity(void)                                            { hook::Thunk<0x4C0730>::Call<void>(this); }
    AGE_API void Matrix44::Transpose(void)                                           { hook::Thunk<0x4C0920>::Call<void>(this); }
    AGE_API void Matrix44::Zero(void)                                                { hook::Thunk<0x4C0770>::Call<void>(this); }
    AGE_API void Matrix44::Hermite(void)                                             { hook::Thunk<0x4C0870>::Call<void>(this); }
    AGE_API void Matrix44::Bezier(void)                                              { hook::Thunk<0x4C0820>::Call<void>(this); }
    AGE_API void Matrix44::CatmullRom(void)                                          { hook::Thunk<0x4C08C0>::Call<void>(this); }
    AGE_API float Matrix44::Determinant(void) const                                  { return hook::Thunk<0x4C134A>::Call<float>(this); }
    AGE_API void Matrix44::Add(const Matrix44& mtx)                                  { hook::Thunk<0x4C0A30>::Call<void>(this, &mtx); }
    AGE_API void Matrix44::AddScaled(const Matrix44& mtx, float scale)               { hook::Thunk<0x4C0CB0>::Call<void>(this, &mtx, scale); }
    AGE_API void Matrix44::Dot(const Matrix44& a1)                                   { hook::Thunk<0x4C0D50>::Call<void>(this, &a1); }
    AGE_API void Matrix44::FastInverse(const Matrix44& a1)                           { hook::Thunk<0x4C1260>::Call<void>(this, &a1); }
    AGE_API void Matrix44::FromMatrix34(const Matrix34 & a1)                         { hook::Thunk<0x4C0970>::Call<void>(this, &a1); }
    AGE_API void Matrix44::InvertTo(Matrix44& a1)                                    { hook::Thunk<0x4C1510>::Call<void>(this, &a1); }
    AGE_API void Matrix44::MakeRotX(float angle)                                     { hook::Thunk<0x4C1C00>::Call<void>(this, angle); }
    AGE_API void Matrix44::MakeRotY(float angle)                                     { hook::Thunk<0x4C1C50>::Call<void>(this, angle); }
    AGE_API void Matrix44::MakeRotZ(float angle)                                     { hook::Thunk<0x4C1CA0>::Call<void>(this, angle); }
    AGE_API void Matrix44::Print(LPCSTR name) const                                  { hook::Thunk<0x4C1CF0>::Call<void>(this, name); }
    AGE_API void Matrix44::Set(const Matrix44* mtx)                                  { hook::Thunk<0x4C07B0>::Call<void>(this, mtx); }
    AGE_API void Matrix44::Subtract(const Matrix44& mtx)                             { hook::Thunk<0x4C0B70>::Call<void>(this, &mtx); }
    AGE_API void Matrix44::ToMatrix34(Matrix34 & a1) const                           { hook::Thunk<0x4C09E0>::Call<void>(this, &a1); }
    AGE_API void Matrix44::Transform4(const Vector3* in, const Vector4* out, int count) 
                                                                                     { hook::Thunk<0x4C1D60>::Call<void>(this, in, out, count); }

    void Matrix44::Transform3x3(const Vector3& vector, Vector3& out) const
    {
        out.X = this->m00 * vector.X + this->m10 * vector.Y + this->m20 * vector.Z;
        out.Y = this->m01 * vector.X + this->m11 * vector.Y + this->m21 * vector.Z;
        out.Z = this->m02 * vector.X + this->m12 * vector.Y + this->m22 * vector.Z;
    }

    Vector3 Matrix44::Transform3x3(const Vector3& vector) const
    {
        Vector3 returnVec;
        Transform3x3(vector, returnVec);
        return returnVec;
    }

    AGE_API void Matrix44::Convert(Matrix44& output, const Matrix34& input)
    {
        hook::StaticThunk<0x42FCF0>::Call<void>(&output, &input);
    }

    void Matrix44::BindLua(LuaState L) {
        LuaBinding(L).beginClass<Matrix44>("Matrix44")
            .addFactory([](float m00 = 0.0, float m01 = 0.0, float m02 = 0.0, float m03 = 0.0,
                float m10 = 0.0, float m11 = 0.0, float m12 = 0.0, float m13 = 0.0,
                float m20 = 0.0, float m21 = 0.0, float m22 = 0.0, float m23 = 0.0,
                float m30 = 0.0, float m31 = 0.0, float m32 = 0.0, float m33 = 0.0) {
            auto mtx = Matrix44();
            mtx.m00 = m00;
            mtx.m01 = m01;
            mtx.m02 = m02;
            mtx.m03 = m03;

            mtx.m10 = m10;
            mtx.m11 = m11;
            mtx.m12 = m12;
            mtx.m13 = m13;

            mtx.m20 = m20;
            mtx.m21 = m21;
            mtx.m22 = m22;
            mtx.m23 = m23;

            mtx.m30 = m30;
            mtx.m31 = m31;
            mtx.m32 = m32;
            mtx.m33 = m33;
            return mtx;
        }, LUA_ARGS(_opt<float>, _opt<float>, _opt<float>, _opt<float>,
            _opt<float>, _opt<float>, _opt<float>, _opt<float>,
            _opt<float>, _opt<float>, _opt<float>, _opt<float>,
            _opt<float>, _opt<float>, _opt<float>, _opt<float>))
            .addVariable("m00", &Matrix44::m00)
            .addVariable("m01", &Matrix44::m01)
            .addVariable("m02", &Matrix44::m02)
            .addVariable("m03", &Matrix44::m03)
            .addVariable("m10", &Matrix44::m10)
            .addVariable("m11", &Matrix44::m11)
            .addVariable("m12", &Matrix44::m12)
            .addVariable("m13", &Matrix44::m13)
            .addVariable("m20", &Matrix44::m20)
            .addVariable("m21", &Matrix44::m21)
            .addVariable("m22", &Matrix44::m22)
            .addVariable("m23", &Matrix44::m23)
            .addVariable("m30", &Matrix44::m30)
            .addVariable("m31", &Matrix44::m31)
            .addVariable("m32", &Matrix44::m32)
            .addVariable("m33", &Matrix44::m33)
            .addStaticFunction("Convert", &Convert)

            .addStaticProperty("I", []() -> Matrix44 { return Matrix44::I; })

            .addFunction("GetColumn", &GetColumn)
            .addFunction("GetRow", &GetRow)
            .addFunction("SetColumn", &SetColumn)
            .addFunction("SetRow", &SetRow)

            .addFunction("Identity", &Identity)
            .addFunction("Transpose", &Transpose)
            .addFunction("Add", &Add)
            .addFunction("Subtract", &Subtract)
            .addFunction("Bezier", &Bezier)
            .addFunction("CatmullRom", &CatmullRom)
            .addFunction("Hermite", &Hermite)
            .addFunction("Zero", &Zero)
            .addFunction("AddScaled", &AddScaled)
            .addFunction("Dot", &Dot)
            .addFunction("FastInverse", &FastInverse)
            .addFunction("FromMatrix34", &FromMatrix34)
            .addFunction("ToMatrix34", &toMatrix34Lua)
            .addFunction("InvertTo", &InvertTo)
            .addFunction("MakeRotX", &MakeRotX)
            .addFunction("MakeRotY", &MakeRotY)
            .addFunction("MakeRotZ", &MakeRotZ)
            .addFunction("Print", &Print)
            .addFunction("Set", &Set)
            .addFunction("Transform4", &Transform4)
            .addFunction("Determinant", &Determinant)
            .endClass();
    }

}