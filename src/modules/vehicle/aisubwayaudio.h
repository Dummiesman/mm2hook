#pragma once
#include <modules\audio\aud3dambientobject.h>

namespace MM2
{
    // Forward declarations
    class aiSubwayAudio;

    // External declarations


    // Class definitions
    class aiSubwayAudio : public Aud3DAmbientObject
    {
    private:
        int State;
    public:
        AGE_API void Activate(int index = -1);
        AGE_API void Deactivate(int index = -1);
        AGE_API void Update(float speed);
    };

    // Lua initialization

}