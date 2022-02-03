#pragma once
#include <mm2_common.h>

namespace MM2
{
    // Forward declarations
    class Matrix444;

    // External declarations    
    extern class Vector4;

    // Class definitions

    class Matrix44 {
    private:
        Matrix34 toMatrix34Lua() const
        {
            Matrix34 returnMtx = Matrix34();
            this->ToMatrix34(returnMtx);
            return returnMtx;
        }
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

        Vector4 GetColumn(int column) const
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

        Vector4 GetRow(int row) const
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

        void SetRow(int row, Vector4 value)
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

        AGE_API void Identity(void)                                            { hook::Thunk<0x4C0730>::Call<void>(this); }
        AGE_API void Transpose(void)                                           { hook::Thunk<0x4C0920>::Call<void>(this); }
        AGE_API void Zero(void)                                                { hook::Thunk<0x4C0770>::Call<void>(this); }
        AGE_API void Hermite(void)                                             { hook::Thunk<0x4C0870>::Call<void>(this); }
        AGE_API void Bezier(void)                                              { hook::Thunk<0x4C0820>::Call<void>(this); }
        AGE_API void CatmullRom(void)                                          { hook::Thunk<0x4C08C0>::Call<void>(this); }
        AGE_API float Determinant(void) const                                  { return hook::Thunk<0x4C134A>::Call<float>(this); }
        AGE_API void Add(const Matrix44* a1)                                   { hook::Thunk<0x4C0A30>::Call<void>(this, a1); }
        AGE_API void AddScaled(const Matrix44* a1, float a2)                   { hook::Thunk<0x4C0CB0>::Call<void>(this, a1, a2); }
        AGE_API void Dot(const Matrix44* a1)                                   { hook::Thunk<0x4C0D50>::Call<void>(this, a1); }
        AGE_API void FastInverse(const Matrix44* a1)                           { hook::Thunk<0x4C1260>::Call<void>(this, a1); }
        AGE_API void FromMatrix34(const Matrix34 & a1)                         { hook::Thunk<0x4C0970>::Call<void>(this, &a1); }
        AGE_API void InvertTo(const Matrix44* a1)                              { hook::Thunk<0x4C1510>::Call<void>(this, a1); }
        AGE_API void MakeRotX(float a1)                                        { hook::Thunk<0x4C1C00>::Call<void>(this, a1); }
        AGE_API void MakeRotY(float a1)                                        { hook::Thunk<0x4C1C50>::Call<void>(this, a1); }
        AGE_API void MakeRotZ(float a1)                                        { hook::Thunk<0x4C1CA0>::Call<void>(this, a1); }
        AGE_API void Print(LPCSTR name) const                                  { hook::Thunk<0x4C1CF0>::Call<void>(this, name); }
        AGE_API void Set(const Matrix44* a1)                                   { hook::Thunk<0x4C07B0>::Call<void>(this, a1); }
        AGE_API void Subtract(const Matrix44* a1)                              { hook::Thunk<0x4C0B70>::Call<void>(this, a1); }
        AGE_API void ToMatrix34(Matrix34 & a1) const                           { hook::Thunk<0x4C09E0>::Call<void>(this, &a1); }
        AGE_API void Transform4(const Vector3* a1, const Vector4* a2, int a3)  { hook::Thunk<0x4C1D60>::Call<void>(this, a1, a2, a3); }
        AGE_API static void Convert(Matrix44* output, const Matrix34* input)
        {
            hook::StaticThunk<0x42FCF0>::Call<void>(output, input);
        }

        static void BindLua(LuaState L) {
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
                .addVariableRef("m00", &Matrix44::m00)
                .addVariableRef("m01", &Matrix44::m01)
                .addVariableRef("m02", &Matrix44::m02)
                .addVariableRef("m03", &Matrix44::m03)
                .addVariableRef("m10", &Matrix44::m10)
                .addVariableRef("m11", &Matrix44::m11)
                .addVariableRef("m12", &Matrix44::m12)
                .addVariableRef("m13", &Matrix44::m13)
                .addVariableRef("m20", &Matrix44::m20)
                .addVariableRef("m21", &Matrix44::m21)
                .addVariableRef("m22", &Matrix44::m22)
                .addVariableRef("m23", &Matrix44::m23)
                .addVariableRef("m30", &Matrix44::m30)
                .addVariableRef("m31", &Matrix44::m31)
                .addVariableRef("m32", &Matrix44::m32)
                .addVariableRef("m33", &Matrix44::m33)
                .addStaticFunction("Convert", &Convert)

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
    };

    ASSERT_SIZEOF(Matrix44, 0x40);

    // Lua initialization

}