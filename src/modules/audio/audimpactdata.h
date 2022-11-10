#pragma once
#include <mm2_common.h>
#include "sound.h"

namespace MM2
{
    // Forward declarations
    struct tagImpactSampleWrapper;
    struct AudImpactData;

    // External declarations

    // Class definitions
    struct tagImpactSampleWrapper
    {
        AudSoundBase* Audio;
        int SampleIndex;
        float MinVolume;
        float MaxVolume;
        float LastPlayedVolume;
        float MinForce;
        float MaxForce;
        float ForceToVolumeScale;
        float Frequency;
    };


    struct AudImpactData
    {
        int NumSamples;
        tagImpactSampleWrapper* SampleWrappers;
        int dword_08;
        int dword_0c;
    };
}