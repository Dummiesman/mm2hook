#pragma once
#include <mm2_common.h>
#include "list.h"
#include "manager.h"

namespace MM2
{
    // Forward declarations
    struct audObjectMethods;
    class audObject;

    // External declarations

    // Class definitions
    struct audObjectMethods
    {
        bool (*Update)(void);
        int (*GetStatus)(void);
        bool (*Play)(void);
        bool (*Stop)(void);
        bool (*SetVolume)(float);
        bool (*SetPitch)(float);
        bool (*SetPan)(float);
        bool (*PauseResume)(void);
    };


    class audObject  {
    private:
        char _buf[0xAC]; // verified
    public:
        int GetSamplesPerSec() const        { return hook::Thunk<0x5A2F60>::Call<int>(this); }
        int GetTotalSize() const            { return hook::Thunk<0x5A2F80>::Call<int>(this); }
        int GetNumChannels() const          { return hook::Thunk<0x5A2F50>::Call<int>(this); }
        int GetBitsPerSample() const        { return hook::Thunk<0x5A2F70>::Call<int>(this); }
        float GetDuration() const           { return static_cast<float>(this->GetTotalSize()) 
                                                     / static_cast<float>(this->GetSamplesPerSec())
                                                     / static_cast<float>(this->GetNumChannels()); }

    };
    ASSERT_SIZEOF(audObject, 0xAC);
}