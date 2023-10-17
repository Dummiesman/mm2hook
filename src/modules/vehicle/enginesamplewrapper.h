#pragma once
#include <modules\audio\sound.h>

namespace MM2
{
    // Forward declarations
    class vehEngineSampleWrapper;

    // External declarations


    // Class definitions
    class vehEngineSampleWrapper
    {
    public:
      AudSoundBase *SoundBasePtr;
      float MinVolume;
      float MaxVolume;
      float MinVolumeBackup;
      float MaxVolumeBackup;
      int dword_14;
      int dword_18;
      float FadeInStartRPM;
      float FadeOutStartRPM;
      float FadeInEndRPM;
      float FadeOutEndRPM;
      float MinPitch;
      float MaxPitch;
      float PitchShiftStartRPM;
      float PitchShiftEndRPM;
      float PitchDivisor;
      bool EchoOn;
      bool UseOldMode;
      float VolumeDivisor;
      float VolInverseRpm;
      int SoundIndex;
    public:
        void DeallocateSoundPtr()                       { hook::Thunk<0x4DF5B0>::Call<void>(this); }
    };

    // Lua initialization

}