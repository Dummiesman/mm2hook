#pragma once
#include <mm2_common.h>

namespace MM2
{
    // Forward declarations
    class Vector3;

    // External declarations
    extern class Matrix34;
    extern class Matrix44;

    // Class definitions

    class Vector3 {
    public:
        float X;
        float Y;
        float Z;

        AGE_API Vector3(void) {}
        AGE_API Vector3(float x, float y, float z) : X(x), Y(y), Z(z) {}

        //operators


        //functions
        AGE_API float InvMag(void) const {
            return 1.0f / this->Mag();
        }
        AGE_API float Mag(void) const {
            return sqrtf(this->Mag2());
        }
        AGE_API float Mag2(void) const {
            return (X * X + Y * Y + Z * Z);
        }
        AGE_API void Set(float x, float y, float z) {
            this->X = x;
            this->Y = y;
            this->Z = z;
        }
        AGE_API void Set(const Vector3& vec) {
            this->X = vec.X;
            this->Y = vec.Y;
            this->Z = vec.Z;
        }

        AGE_API void Add(const Vector3& vec) {
            this->X += vec.X;
            this->Y += vec.Y;
            this->Z += vec.Z;
        }
        AGE_API void AddScaled(const Vector3& vec, float scale) {
            this->X += (vec.X * scale);
            this->Y += (vec.Y * scale);
            this->Z += (vec.Z * scale);
        }
        AGE_API void Subtract(const Vector3& vec) {
            this->X -= vec.X;
            this->Y -= vec.Y;
            this->Z -= vec.Z;
        }
        AGE_API void SubtractScaled(const Vector3& vec, float scale) {
            this->X -= (vec.X * scale);
            this->Y -= (vec.Y * scale);
            this->Z -= (vec.Z * scale);
        }
        AGE_API void Scale(float scale) {
            this->X *= scale;
            this->Y *= scale;
            this->Z *= scale;
        }
        AGE_API void InvScale(float scale) {
            this->X *= 1.0f / scale;
            this->Y *= 1.0f / scale;
            this->Z *= 1.0f / scale;
        }
        AGE_API void RotateX(float angle) {
            hook::Thunk<0x4C0390>::Call<void>(this, angle);
        }
        AGE_API void RotateY(float angle) {
            hook::Thunk<0x4C03D0>::Call<void>(this, angle);
        }
        AGE_API void RotateZ(float angle) {
            hook::Thunk<0x4C0400>::Call<void>(this, angle);
        }
        AGE_API void RotateAboutAxis(float angle, int axis) {
            hook::Thunk<0x4C0020>::Call<void>(this, angle, axis);
        }
        AGE_API float Angle(const Vector3& vec) const {
            return hook::Thunk<0x4C0430>::Call<float>(this, &vec);
        }
        AGE_API float FastAngle(const Vector3& vec) const {
            return hook::Thunk<0x4C0500>::Call<float>(this, &vec);
        }
        AGE_API bool Approach(const Vector3& vec, float p2, float p3) {
            return hook::Thunk<0x4BFF20>::Call<bool>(this, &vec, p2, p3);
        }
        AGE_API void Cross(const Vector3& vec) {
            hook::Thunk<0x45CE80>::Call<void>(this, &vec);
        }
        AGE_API void Cross(const Vector3& vec1, const Vector3& vec2) {
            hook::Thunk<0x4429C0>::Call<void>(this, &vec1, &vec2);
        }
        AGE_API float Dist(const Vector3& vec) const {
            return (vec - *this).Mag();
        }
        AGE_API float FlatDist(const Vector3& vec) const {
            float dX = vec.X - this->X;
            float dY = vec.Y - this->Y;
            return sqrtf(dX * dX + dY * dY);
        }
        AGE_API float Dot(const Vector3& vec) const {
            return hook::Thunk<0x466470>::Call<float>(this, &vec);
        }
        AGE_API void Dot(const Vector3& vec, const Matrix34& mtx) {
            hook::Thunk<0x43BBB0>::Call<void>(this, &vec, &mtx);
        }
        AGE_API void Dot3x3(const Matrix34& mtx) {
            hook::Thunk<0x479560>::Call<void>(this, &mtx);
        }
        AGE_API void Dot3x3Transpose(const Matrix34& mtx) {
            hook::Thunk<0x4795C0>::Call<void>(this, &mtx);
        }
        AGE_API void Lerp(float p1, const Vector3& vec1, const Vector3& vec2) {
            hook::Thunk<0x494C00>::Call<void>(this, p1, &vec1, &vec2);
        }
        AGE_API void Negate(const Vector3& vec) {
            hook::Thunk<0x480AC0>::Call<void>(this, &vec);
        }
        AGE_API void Normalize()
        {
            float len = this->Mag2();
            float mul = (len != 00.f) ? 1.0f / sqrtf(len) : 0.0f;

            this->X *= mul;
            this->Y *= mul;
            this->Z *= mul;
        }

        AGE_API void HSVtoRGB() {
            hook::Thunk<0x4C0270>::Call<void>(this);
        }

        AGE_API void RGBtoHSV() {
            hook::Thunk<0x4C0160>::Call<void>(this);
        }

        AGE_API bool IsEqual(const Vector3& vec) const {
            return vec.X == this->X && vec.Y == this->Y && vec.Z == this->Z;
        }

        AGE_API void operator*=(const Vector3& vec) {
            hook::Thunk<0x43DD50>::Call<void>(this, &vec);
        }
        AGE_API void operator+=(const Vector3& vec) {
            hook::Thunk<0x45CCD0>::Call<void>(this, &vec);
        }
        AGE_API void operator-=(const Vector3& vec) {
            hook::Thunk<0x45CD00>::Call<void>(this, &vec);
        }
        AGE_API void operator+=(Vector3& vec) {
            hook::Thunk<0x479520>::Call<void>(this, &vec);
        }
        AGE_API void operator-=(Vector3& vec) {
            hook::Thunk<0x470180>::Call<void>(this, &vec);
        }

        AGE_API Vector3 operator+(Vector3& vec) {
            return Vector3(this->X + vec.X, this->Y + vec.Y, this->Z + vec.Z);
        }
        AGE_API Vector3 operator-(const Vector3& vec) const {
            return hook::Thunk<0x491860>::Call<Vector3>(this, &vec);
        }
        AGE_API Vector3 operator/(float value) const {
            return Vector3(this->X / value, this->Y / value, this->Z / value);
        }
        AGE_API Vector3 operator*(float value) const {
            return Vector3(this->X * value, this->Y * value, this->Z * value);
        }
        AGE_API Vector3 operator%(const Vector3& vec) const {
            return hook::Thunk<0x541770>::Call<Vector3>(this, &vec);
        }

        static void BindLua(LuaState L) {
            LuaBinding(L).beginClass<Vector3>("Vector3")
                .addFactory([](float x = 0.0, float y = 0.0, float z = 0.0) {
                auto vec = Vector3();
                vec.X = x;
                vec.Y = y;
                vec.Z = z;
                return vec;
            }, LUA_ARGS(_opt<float>, _opt<float>, _opt<float>))
                .addVariableRef("x", &Vector3::X)
                .addVariableRef("y", &Vector3::Y)
                .addVariableRef("z", &Vector3::Z)

                .addFunction("__add", &Vector3::operator+)
                .addFunction("__sub", &Vector3::operator-)
                .addFunction("__div", &Vector3::operator/)
                .addFunction("__mul", &Vector3::operator*)
                .addFunction("__eq", &Vector3::IsEqual)

                .addFunction("Cross", static_cast<void(Vector3::*)(const Vector3&)>(&Vector3::Cross))
                .addFunction("Normalize", &Normalize)
                .addFunction("Mag", &Mag)
                .addFunction("Mag2", &Mag2)
                .addFunction("InvMag", &InvMag)
                .addFunction("RotateX", &RotateX)
                .addFunction("RotateY", &RotateY)
                .addFunction("RotateZ", &RotateZ)
                .addFunction("RotateAboutAxis", &RotateAboutAxis)
                .addFunction("Lerp", &Lerp)
                .addFunction("Dist", &Dist)
                .addFunction("Angle", &Angle)
                .addFunction("FastAngle", &FastAngle)
                .addFunction("FlatDist", &FlatDist)
                .addFunction("Approach", &Approach)
                .addFunction("Negate", &Negate)


                .endClass();
        }
    };

    ASSERT_SIZEOF(Vector3, 0xC);

    // Lua initialization

}