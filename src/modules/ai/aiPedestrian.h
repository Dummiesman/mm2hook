#pragma once
#include "aiPedAudio.h"

namespace MM2
{
    // Forward declarations
    class aiPedestrian;

    // External declarations
    extern class aiPedestrianInstance;

    // Class definitions

    class aiPedestrian {
    private:
        byte _buffer[0x134];
    protected:
        static hook::Field<0xC0, aiPedAudio> _audio;
        static hook::Field<0x9C, aiPedestrianInstance *> _instance;
    public:
        AGE_API void Update()                                   { hook::Thunk<0x54B9C0>::Call<void>(this); }

        inline aiPedestrianInstance * GetInstance(void) const {
            return _instance.get(this);
        }

        inline aiPedAudio * GetAudio(void) const {
            return _audio.ptr(this);
        };
    };

    ASSERT_SIZEOF(aiPedestrian, 0x134);
}