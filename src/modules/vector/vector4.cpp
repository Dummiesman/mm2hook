#include "vector4.h"

namespace MM2
{
    AGE_API Vector4::Vector4() {}
    AGE_API Vector4::Vector4(float x, float y, float z, float w) : X(x), Y(y), Z(z), W(w) {}

    AGE_API void Vector4::Set(float x, float y, float z, float w) {
        this->X = x;
        this->Y = y;
        this->Z = z;
        this->W = w;
    }
    AGE_API void Vector4::Cross(const Vector4& vec1, const Vector4& vec2)                     { hook::Thunk<0x494C90>::Call<void>(this, &vec1, &vec2); }
    AGE_API void Vector4::Subtract(const Vector3& vec1, const Vector3& vec2)                  { hook::Thunk<0x494C40>::Call<void>(this, &vec1, &vec2); }
    AGE_API float Vector4::Dot(const Vector4& vec) const                                      { return hook::Thunk<0x43DDA0>::Call<float>(this, &vec); }
    AGE_API float Vector4::Dot3(const Vector4& vec) const                                     { return hook::Thunk<0x494C70>::Call<float>(&vec); }
    AGE_API Vector4& Vector4::Dot(const Vector4& vec, Matrix44 const& mtx)                    { return hook::Thunk<0x4C1170>::Call<Vector4&>(this, &vec,& mtx); }
    AGE_API Vector4& Vector4::Dot3x3(const Vector4& vec, Matrix44 const& mtx)                 { return hook::Thunk<0x4C1200>::Call<Vector4&>(this, &vec, &mtx); }
    AGE_API void Vector4::ComputePlane(const Vector3& vec1, const Vector3& vec2, const Vector3& vec3)
                                                                                              { hook::Thunk<0x4C52D0>::Call<void>(this, &vec1, &vec2, &vec3); }
    AGE_API void Vector4::ComputePlane(const Vector3& vec1, const Vector3& vec2)              {  hook::Thunk<0x4C53B0>::Call<void>(this, &vec1, &vec2); }
    AGE_API void Vector4::Min(const Vector4& vec1, const Vector4& vec2)                       { hook::Thunk<0x4C53F0>::Call<void>(this, &vec1, &vec2); }
    AGE_API void Vector4::Max(const Vector4& vec1, const Vector4& vec2)                       { hook::Thunk<0x4C5460>::Call<void>(this, &vec1, &vec2); }

    void Vector4::BindLua(LuaState L) {
        LuaBinding(L).beginClass<Vector4>("Vector4")
            .addFactory([](float x = 0.0, float y = 0.0, float z = 0.0, float w = 0.0) {
            auto vec = Vector4();
            vec.X = x;
            vec.Y = y;
            vec.Z = z;
            vec.W = w;
            return vec;
        }, LUA_ARGS(_opt<float>, _opt<float>, _opt<float>, _opt<float>))
            .addVariableRef("x", &Vector4::X)
            .addVariableRef("y", &Vector4::Y)
            .addVariableRef("z", &Vector4::Z)
            .addVariableRef("w", &Vector4::W)
            .endClass();
    }
}