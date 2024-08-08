#pragma once
#include "vector3.h"

namespace MM2
{
    AGE_API Vector3::Vector3(void) {}
    AGE_API Vector3::Vector3(float x, float y, float z) : X(x), Y(y), Z(z) {}

    const Vector3 Vector3::ORIGIN = Vector3(0.0f, 0.0f, 0.0f);
    const Vector3 Vector3::XAXIS = Vector3(1.0f, 0.0f, 0.0f);
    const Vector3 Vector3::YAXIS = Vector3(0.0f, 1.0f, 0.0f);
    const Vector3 Vector3::ZAXIS = Vector3(0.0f, 0.0f, 1.0f);

    AGE_API float Vector3::InvMag(void) const {
        return 1.0f / this->Mag();
    }
    AGE_API float Vector3::Mag(void) const {
        return sqrtf(this->Mag2());
    }
    AGE_API float Vector3::Mag2(void) const {
        return (X * X + Y * Y + Z * Z);
    }
    AGE_API void Vector3::Set(float x, float y, float z) {
        this->X = x;
        this->Y = y;
        this->Z = z;
    }
    AGE_API void Vector3::Set(const Vector3& vec) {
        this->X = vec.X;
        this->Y = vec.Y;
        this->Z = vec.Z;
    }
    AGE_API void Vector3::Add(const Vector3& vec) {
        this->X += vec.X;
        this->Y += vec.Y;
        this->Z += vec.Z;
    }
    AGE_API void Vector3::AddScaled(const Vector3& vec, float scale) {
        this->X += (vec.X * scale);
        this->Y += (vec.Y * scale);
        this->Z += (vec.Z * scale);
    }
    AGE_API void Vector3::Subtract(const Vector3& vec) {
        this->X -= vec.X;
        this->Y -= vec.Y;
        this->Z -= vec.Z;
    }
    AGE_API void Vector3::SubtractScaled(const Vector3& vec, float scale) {
        this->X -= (vec.X * scale);
        this->Y -= (vec.Y * scale);
        this->Z -= (vec.Z * scale);
    }
    AGE_API void Vector3::Scale(float scale) {
        this->X *= scale;
        this->Y *= scale;
        this->Z *= scale;
    }
    AGE_API void Vector3::InvScale(float scale) {
        this->X *= 1.0f / scale;
        this->Y *= 1.0f / scale;
        this->Z *= 1.0f / scale;
    }
    AGE_API void Vector3::RotateX(float angle) {
        float tmp = cosf(angle) * this->Y - sinf(angle) * this->Z;
        this->Z = cosf(angle) * this->Z + sinf(angle) * this->Y;
        this->Y = tmp;
    }
    AGE_API void Vector3::RotateY(float angle) {
        float tmp = cosf(angle) * this->Z - sinf(angle) * this->X;
        this->X = sinf(angle) * this->Z + cosf(angle) * this->X;
        this->Z = tmp;
    }
    AGE_API void Vector3::RotateZ(float angle) {
        float tmp = cosf(angle) * this->X - sinf(angle) * this->Y;
        this->Y = cosf(angle) * this->Y + sinf(angle) * this->X;
        this->X = tmp;
    }
    AGE_API void Vector3::RotateAboutAxis(float angle, int axis) {
        float tmp;
        switch (axis)
        {
        case 'x':
            tmp = cosf(angle) * this->Y - sinf(angle) * this->Z;
            this->Z = cosf(angle) * this->Z + sinf(angle) * this->Y;
            this->Y = tmp;
            break;
        case 'y':
            tmp = cosf(angle) * this->Z - sinf(angle) * this->X;
            this->X = sinf(angle) * this->Z + cosf(angle) * this->X;
            this->Z = tmp;
            break;
        case 'z':
            tmp = cosf(angle) * this->X - sinf(angle) * this->Y;
            this->Y = cosf(angle) * this->Y + sinf(angle) * this->X;
            this->X = tmp;
            break;
        }
    }
    AGE_API float Vector3::Angle(const Vector3& vec) const {
        float mag = sqrtf(vec.Mag2() * this->Mag2());
        float dotDivMag = this->Dot(vec) / mag;
        if (dotDivMag > 0.99999988)
            return 0.0;
        if (dotDivMag >= -1.0)
            return acosf(dotDivMag);
        return 3.1415927f;
    }
    AGE_API float Vector3::FastAngle(const Vector3& vec) const {
        return acosf(vec.Z * this->Z + vec.Y * this->Y + vec.X * this->X);
    }
    AGE_API bool Vector3::Approach(const Vector3& vec, float rate, float time) {
        return hook::Thunk<0x4BFF20>::Call<bool>(this, &vec, rate, time);
    }
    AGE_API void Vector3::Cross(const Vector3& vec) {
        hook::Thunk<0x45CE80>::Call<void>(this, &vec);
    }
    AGE_API void Vector3::Cross(const Vector3& vec1, const Vector3& vec2) {
        hook::Thunk<0x4429C0>::Call<void>(this, &vec1, &vec2);
    }
    AGE_API float Vector3::Dist(const Vector3& vec) const {
        return (vec - *this).Mag();
    }
    AGE_API float Vector3::FlatDist(const Vector3& vec) const {
        float dX = vec.X - this->X;
        float dZ = vec.Z - this->Z;
        return sqrtf(dX * dX + dZ * dZ);
    }
    AGE_API float Vector3::Dot(const Vector3& vec) const {
        return vec.Z * this->Z + vec.Y * this->Y + vec.X * this->X;
    }
    AGE_API void Vector3::Dot(const Vector3& vec, const Matrix34& mtx) {
        hook::Thunk<0x43BBB0>::Call<void>(this, &vec, &mtx);
    }
    AGE_API void Vector3::Dot3x3(const Matrix34& mtx) {
        hook::Thunk<0x479560>::Call<void>(this, &mtx);
    }
    AGE_API void Vector3::Dot3x3Transpose(const Matrix34& mtx) {
        hook::Thunk<0x4795C0>::Call<void>(this, &mtx);
    }
    AGE_API void Vector3::Lerp(float t, const Vector3& vec1, const Vector3& vec2) {
        this->X = (vec2.X - vec1.X) * t + vec1.X;
        this->Y = (vec2.Y - vec1.Y) * t + vec1.Y;
        this->Z = (vec2.Z - vec1.Z) * t + vec1.Z;
    }
    AGE_API void Vector3::Negate()
    {
        this->X = -this->X;
        this->Y = -this->Y;
        this->Z = -this->Z;
    }
    AGE_API void Vector3::Negate(const Vector3& vec) {
        this->X = -vec.X;
        this->Y = -vec.Y;
        this->Z = -vec.Z;
    }
    AGE_API void Vector3::Normalize()
    {
        float len = this->Mag2();
        float mul = (len != 0.0f) ? 1.0f / sqrtf(len) : 0.0f;

        this->X *= mul;
        this->Y *= mul;
        this->Z *= mul;
    }

    AGE_API void Vector3::HSVtoRGB() {
        hook::Thunk<0x4C0270>::Call<void>(this);
    }

    AGE_API void Vector3::RGBtoHSV() {
        hook::Thunk<0x4C0160>::Call<void>(this);
    }

    AGE_API void Vector3::Min(const Vector3& vec1, const Vector3& vec2)
    {
        this->X = (vec1.X < vec2.X) ? vec1.X : vec2.X;
        this->Y = (vec1.Y < vec2.Y) ? vec1.Y : vec2.Y;
        this->Z = (vec1.Z < vec2.Z) ? vec1.Z : vec2.Z;
    }

    AGE_API void Vector3::Max(const Vector3& vec1, const Vector3& vec2)
    {
        this->X = (vec1.X > vec2.X) ? vec1.X : vec2.X;
        this->Y = (vec1.Y > vec2.Y) ? vec1.Y : vec2.Y;
        this->Z = (vec1.Z > vec2.Z) ? vec1.Z : vec2.Z;
    }

    AGE_API bool Vector3::IsEqual(const Vector3& vec) const {
        return vec.X == this->X && vec.Y == this->Y && vec.Z == this->Z;
    }

    AGE_API Vector3 Vector3::Multiply(const Vector3& vec) const {
        return Vector3(this->X * vec.X, this->Y * vec.Y, this->Z * vec.Z);
    }

    AGE_API void Vector3::Print() const
    {
        Printf("%f,%f,%f", this->X, this->Y, this->Z);
    }

    AGE_API void Vector3::Print(LPCSTR caption) const
    {
        Printf("%s: %f,%f,%f", caption, this->X, this->Y, this->Z);
    }

    AGE_API void Vector3::operator*=(const Vector3& vec) {
        hook::Thunk<0x43DD50>::Call<void>(this, &vec);
    }
    AGE_API void Vector3::operator+=(const Vector3& vec) {
        this->X += vec.X;
        this->Y += vec.Y;
        this->Z += vec.Z;
    }
    AGE_API void Vector3::operator-=(const Vector3& vec) {
        this->X -= vec.X;
        this->Y -= vec.Y;
        this->Z -= vec.Z;
    }
    AGE_API void Vector3::operator+=(Vector3& vec) {
        this->X += vec.X;
        this->Y += vec.Y;
        this->Z += vec.Z;
    }
    AGE_API void Vector3::operator-=(Vector3& vec) {
        this->X -= vec.X;
        this->Y -= vec.Y;
        this->Z -= vec.Z;
    }

    AGE_API bool Vector3::operator!=(const Vector3& vec)
    {
        return !operator==(vec);
    }

    AGE_API bool Vector3::operator==(const Vector3& vec)
    {
        return vec.X == this->X && vec.Y == this->Y && vec.Z == this->Z;
    }

    AGE_API Vector3 Vector3::operator+(const Vector3& vec) const {
        return Vector3(this->X + vec.X, this->Y + vec.Y, this->Z + vec.Z);
    }
    AGE_API Vector3 Vector3::operator-(const Vector3& vec) const {
        return Vector3(this->X - vec.X, this->Y - vec.Y, this->Z - vec.Z);
    }
    AGE_API Vector3 Vector3::operator/(float value) const {
        return Vector3(this->X / value, this->Y / value, this->Z / value);
    }
    AGE_API Vector3 Vector3::operator*(float value) const {
        return Vector3(this->X * value, this->Y * value, this->Z * value);
    }
    AGE_API Vector3 Vector3::operator%(const Vector3& vec) const {
        return hook::Thunk<0x541770>::Call<Vector3>(this, &vec);
    }

    Vector3::operator Vector2() const
    {
        return Vector2(this->X, this->Y);
    }

    Vector3::operator Vector4() const
    {
        return Vector4(this->X, this->Y, this->Z, 0.0f);
    }

    void Vector3::BindLua(LuaState L) {
        LuaBinding(L).beginClass<Vector3>("Vector3")
            .addFactory([](float x = 0.0, float y = 0.0, float z = 0.0) {
            auto vec = Vector3(x, y, z);
            return vec;
        }, LUA_ARGS(_opt<float>, _opt<float>, _opt<float>))
            .addVariable("x", &Vector3::X)
            .addVariable("y", &Vector3::Y)
            .addVariable("z", &Vector3::Z)

            .addFunction("__add", &Vector3::operator+)
            .addFunction("__sub", &Vector3::operator-)
            .addFunction("__div", &Vector3::operator/)
            .addFunction("__mul", &Vector3::operator*)
            .addFunction("__eq", &Vector3::IsEqual)

            .addStaticProperty("ORIGIN", []() -> Vector3 { return Vector3::ORIGIN; })
            .addStaticProperty("XAXIS", []() -> Vector3 { return Vector3::XAXIS; })
            .addStaticProperty("YAXIS", []() -> Vector3 { return Vector3::YAXIS; })
            .addStaticProperty("ZAXIS", []() -> Vector3 { return Vector3::ZAXIS; })
            .addFunction("Cross", static_cast<void(Vector3::*)(const Vector3&)>(&Vector3::Cross))
            .addFunction("Normalize", &Normalize)
            .addFunction("Mag", &Mag)
            .addFunction("Mag2", &Mag2)
            .addFunction("InvMag", &InvMag)
            .addFunction("RotateX", &RotateX)
            .addFunction("RotateY", &RotateY)
            .addFunction("RotateZ", &RotateZ)
            .addFunction("RotateAboutAxis", &RotateAboutAxis)
            .addFunction("Set", static_cast<void(Vector3::*)(const Vector3&)>(&Vector3::Set))
            .addFunction("Lerp", &Lerp)
            .addFunction("Dist", &Dist)
            .addFunction("Dot", static_cast<float(Vector3::*)(const Vector3&) const>(&Vector3::Dot))
            .addFunction("Angle", &Angle)
            .addFunction("FastAngle", &FastAngle)
            .addFunction("FlatDist", &FlatDist)
            .addFunction("Approach", &Approach)
            .addFunction("Negate", static_cast<void(Vector3::*)(const Vector3&)>(&Vector3::Negate))
            .addFunction("Min", &Min)
            .addFunction("Max", &Max)
            .addFunction("Print", static_cast<void(Vector3::*)(LPCSTR)const>(&Vector3::Print))

            .addFunction("RGBtoHSV", &RGBtoHSV)
            .addFunction("HSVtoRGB", &HSVtoRGB)

            .endClass();
    }
}