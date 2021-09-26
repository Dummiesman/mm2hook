#pragma once

namespace MM2
{
    // Forward declarations
    class gizPathspline;

    // External declarations
    extern class dgPath;

    // Class definitions
    class gizPathspline {
    private:
        byte buffer[0x48];
    protected:
        hook::Field<0x04, float> _speed;
    public:
        gizPathspline() {
            hook::Thunk<0x57A050>::Call<void>(this);
        }

        ~gizPathspline() {
            hook::Thunk<0x57A080>::Call<void>(this);
        }

        inline float getSpeed(void) const {
            return _speed.get(this);
        };

        AGE_API void Init(dgPath* path, float speed)        { hook::Thunk<0x57A0E0>::Call<void>(this, path, speed); }
        AGE_API void Reset()                                { hook::Thunk<0x57A090>::Call<void>(this); }
        AGE_API void SetSpeed(float speed)                  { hook::Thunk<0x57A760>::Call<void>(this, speed); }
        AGE_API void Update(Vector3& pos, Vector3& fwd, float t)
                                                            { hook::Thunk<0x57A410>::Call<void>(this, &pos, &fwd, t); }
    };
}