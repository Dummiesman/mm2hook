#pragma once
#include <modules\audio\sound.h>

namespace MM2
{
    // Forward declarations
    struct vehSurfaceAudioSkidData;
    class vehSurfaceAudioData;

    // External declarations


    // Class definitions
    struct vehSurfaceAudioSkidData
    {
    public:
        ANGEL_ALLOCATOR;
        int MinSlippage;
        int MaxSlippage;
        AudSoundBase* SoundBasePtr;
        int SampleIndex;
    };

    class vehSurfaceAudioData
    {
    public:
        AudSoundBase* SurfaceSoundBasePtr;
        vehSurfaceAudioSkidData* SkidWaveData;
        int SampleIndex;
        int NumSkidSamples;
        int MaxSpeed;
        int MinSurfaceVolume;
        int MaxSurfaceVolume;
        int dword_1c;
        int MinSkidVolume;
        int MaxSkidVolume;
        int dword_28;
        int MinSurfacePitch;
        int MaxSurfacePitch;
        int dword_34;
    };


    // Lua initialization

}