#pragma once
#include <mm2_common.h>

namespace MM2
{
    // Forward declarations
    class Vector2;

    // External declarations

    // Class definitions
    class Vector2 {
    public:
        float X;
        float Y;

        AGE_API Vector2() {}
        AGE_API Vector2(float x, float y) : X(x), Y(y) {}

        AGE_API float Mag(void) const {
            return sqrtf(X * X + Y * Y);
        }
        AGE_API float Mag2(void) const {
            return X * X + Y * Y;
        }

        AGE_API void Set(float x, float y) {
            X = x;
            Y = y;
        }

        bool IsEqual(const Vector2& vec) const {
            return this->X == vec.X && this->Y == vec.Y;
        }

        AGE_API void Normalize()
        {
            float len = this->Mag2();
            float mul = (len != 00.f) ? 1.0f / sqrtf(len) : 0.0f;

            this->X *= mul;
            this->Y *= mul;
        }

        Vector2 operator+(Vector2& vec) {
            return Vector2(this->X + vec.X, this->Y + vec.Y);
        }
        Vector2 operator-(const Vector2 vec) const {
            return Vector2(vec.X - this->X, vec.Y - this->Y);
        }
        Vector2 operator/(float value) const {
            return Vector2(this->X / value, this->Y / value);
        }
        Vector2 operator*(float value) const {
            return Vector2(this->X * value, this->Y * value);
        }

        static void BindLua(LuaState L) {
            LuaBinding(L).beginClass<Vector2>("Vector2")
                .addFactory([](float x = 0.0, float y = 0.0) {
                auto vec = Vector2();
                vec.X = x;
                vec.Y = y;
                return vec;
            }, LUA_ARGS(_opt<float>, _opt<float>))
                .addVariableRef("x", &Vector2::X)
                .addVariableRef("y", &Vector2::Y)

                .addFunction("__add", &Vector2::operator+)
                .addFunction("__sub", &Vector2::operator-)
                .addFunction("__div", &Vector2::operator/)
                .addFunction("__mul", &Vector2::operator*)
                .addFunction("__eq", &Vector2::IsEqual)

                .addFunction("Mag", &Mag)
                .addFunction("Mag2", &Mag2)
                .addFunction("Normalize", &Normalize)
                .endClass();
        }
    };

    ASSERT_SIZEOF(Vector2, 0x8);

    // Lua initialization

}