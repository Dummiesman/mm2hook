#pragma once
#include <mm2_common.h>

namespace MM2
{
    // Forward declarations
    class Vector4;

    // External declarations
    extern class Vector3;    

    // Class definitions

    class Vector4 {
    public:
        float X;
        float Y;
        float Z;
        float W;

        AGE_API Vector4() {}
        AGE_API Vector4(float x, float y, float z, float w) : X(x), Y(y), Z(z), W(w) {}

        AGE_API void Set(float x, float y, float z, float w) {
            this->X = x;
            this->Y = y;
            this->Z = z;
            this->W = w;
        }
        AGE_API void Cross(const Vector4& vec1, const Vector4& vec2)                     { hook::Thunk<0x494C90>::Call<void>(this, &vec1, &vec2); }
        AGE_API void Subtract(const Vector3& vec1, const Vector3& vec2)                  { hook::Thunk<0x494C40>::Call<void>(this, &vec1, &vec2); }
        AGE_API float Dot(const Vector4& vec) const                                      { return hook::Thunk<0x43DDA0>::Call<float>(this, &vec); }
        AGE_API float Dot3(const Vector4& vec) const                                     { return hook::Thunk<0x494C70>::Call<float>(&vec); }
        AGE_API Vector4& Dot(const Vector4& vec, Matrix44 const& mtx)                    { return hook::Thunk<0x4C1170>::Call<Vector4&>(this, &vec,& mtx); }
        AGE_API Vector4& Dot3x3(const Vector4& vec, Matrix44 const& mtx)                 { return hook::Thunk<0x4C1200>::Call<Vector4&>(this, &vec, &mtx); }
        AGE_API void ComputePlane(const Vector3& vec1, const Vector3& vec2, const Vector3& vec3) 
                                                                                         { hook::Thunk<0x4C52D0>::Call<void>(this, &vec1, &vec2, &vec3); }
        AGE_API void ComputePlane(const Vector3& vec1, const Vector3& vec2)              {  hook::Thunk<0x4C53B0>::Call<void>(this, &vec1, &vec2); }
        AGE_API void Min(const Vector4& vec1, const Vector4& vec2)                       { hook::Thunk<0x4C53F0>::Call<void>(this, &vec1, &vec2); }
        AGE_API void Max(const Vector4& vec1, const Vector4& vec2)                       { hook::Thunk<0x4C5460>::Call<void>(this, &vec1, &vec2); }

        static void BindLua(LuaState L) {
            LuaBinding(L).beginClass<Vector4>("Vector4")
                .addFactory([](float x = 0.0, float y = 0.0, float z = 0.0, float w = 0.0) {
                auto vec = new Vector4;
                vec->X = x;
                vec->Y = y;
                vec->Z = z;
                vec->W = w;
                return vec;
            }, LUA_ARGS(_opt<float>, _opt<float>, _opt<float>, _opt<float>))
                .addVariableRef("x", &Vector4::X)
                .addVariableRef("y", &Vector4::Y)
                .addVariableRef("z", &Vector4::Z)
                .addVariableRef("w", &Vector4::W)
                .endClass();
        }
    };

    ASSERT_SIZEOF(Vector4, 0x10);

    // Lua initialization

}