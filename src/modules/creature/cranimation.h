#pragma once
#include <modules\creature.h>
#include <modules\data.h>

namespace MM2
{
    // Forward declarations
    class crAnimFrame;
    class crAnimation;

    // External declarations
    extern class datRefCount;

    // Class definitions
    class crAnimFrame
    {
    private:
        int ChannelCount;
        float* Buffer;
    public:
        crAnimFrame(bool bufferless)
        {
            hook::Thunk<0x57DDA0>::Call<void>(this, bufferless);
        }

        crAnimFrame(crAnimFrame const& copyFrom)
        {
            hook::Thunk<0x57DDE0>::Call<void>(this, &copyFrom);
        }

        ~crAnimFrame()
        {
            hook::Thunk<0x57DE13>::Call<void>(this);
        }

        void Blend(float fraction, crAnimFrame* first, crAnimFrame* second, int startFrame = -1, int endFrame = -1)
        {
            hook::Thunk<0x57E110>::Call<void>(this, fraction, first, second, startFrame, endFrame);
        }

        float* GetBuffer()     { return Buffer; }
        int GetChannelCount()  { return ChannelCount; }

        void Print()           { hook::Thunk<0x57DFA0>::Call<void>(this); }
    };

    class crAnimation : public datRefCount {
    private:
        int NumFrames;
        int NumChannels;
        int dword_10;
        crAnimFrame* Frames;
        float dword_18;
        char char_1c;
        char char_1d;
    public:
        int GetFrameCount() const                   { return NumFrames; }
        int GetChannelCount() const                 { return NumChannels; }
        crAnimFrame* GetFrame(int num) const        { return &Frames[num]; }

        static void OutputAnimationList()           { hook::StaticThunk<0x57D0F0>::Call<void>(); }
    };

    // Lua initialization

}