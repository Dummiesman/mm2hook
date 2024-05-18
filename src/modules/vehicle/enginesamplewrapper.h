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
       AudSoundBase* SoundBasePtr;
       float MinVolume;
       float MaxVolume;
       float MinVolumeBackup; // Used to reassign Min/Max volume after Silence(TRUE)
       float MaxVolumeBackup; // Used to reassign Min/Max volume after Silence(TRUE)
       float FadeInVolDivisor;
       float FadeOutVolDivisor;
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
    private:
        void RecomputeDivs()
        {
            float volumeChange = MaxVolume - MinVolume;
            float pitchChange = MaxPitch - MinPitch;

            float fadeInRpmChange = FadeInEndRPM - FadeInStartRPM;
            float invFadeInRpmChange = (fadeInRpmChange == 0.0f) ? 0.0f : 1.0f / fadeInRpmChange;
            FadeInVolDivisor = volumeChange * invFadeInRpmChange;

            float fadeOutRpmChange = FadeOutEndRPM - FadeOutStartRPM;
            float invFadeOutRpmChange = (fadeOutRpmChange == 0.0f) ? 0.0f : 1.0f / fadeOutRpmChange;
            FadeOutVolDivisor = volumeChange * invFadeOutRpmChange;

            float pitchInChange = PitchShiftEndRPM - PitchShiftStartRPM;
            float invPitchChange = (pitchInChange == 0.0f) ? 0.0f : 1.0f / pitchInChange;
            PitchDivisor = pitchChange * invPitchChange;
        }
    public:
        ANGEL_ALLOCATOR

        vehEngineSampleWrapper()
        {
            hook::Thunk<0x4DF550>::Call<void>(this);
        }

        void SetFadeInRange(float startRpm, float endRpm)
        {
            FadeInStartRPM = startRpm;
            FadeInEndRPM = endRpm;
            RecomputeDivs();
        }

        void SetFadeOutRange(float startRpm, float endRpm)
        {
            FadeOutStartRPM = startRpm;
            FadeOutEndRPM = endRpm;
            RecomputeDivs();
        }

        void SetPitchShiftRange(float startRpm, float endRpm)
        {
            PitchShiftStartRPM = startRpm;
            PitchShiftEndRPM = endRpm;
            RecomputeDivs();
        }

        void SetPitchShiftStartRPM(float rpm)
        {
            PitchShiftStartRPM = rpm;
            RecomputeDivs();
        }

        float GetPitchShiftStartRPM() const
        {
            return PitchShiftStartRPM;
        }

        void SetPitchShiftEndRPM(float rpm)
        {
            PitchShiftEndRPM = rpm;
            RecomputeDivs();
        }

        float GetPitchShiftEndRPM() const
        {
            return PitchShiftEndRPM;
        }

        void SetFadeInStartRPM(float rpm)
        {
            FadeInStartRPM = rpm;
            RecomputeDivs();
        }

        float GetFadeInStartRPM() const
        {
            return FadeInStartRPM;
        }

        void SetFadeInEndRPM(float rpm)
        {
            FadeInEndRPM = rpm;
            RecomputeDivs();
        }

        float GetFadeInEndRPM() const
        {
            return FadeInEndRPM;
        }

        void SetFadeOutStartRPM(float rpm)
        {
            FadeOutStartRPM = rpm;
            RecomputeDivs();
        }

        float GetFadeOutStartRPM() const
        {
            return FadeOutStartRPM;
        }

        void SetFadeOutEndRPM(float rpm)
        {
            FadeOutEndRPM = rpm;
            RecomputeDivs();
        }

        float GetFadeOutEndRPM() const
        {
            return FadeOutEndRPM;
        }

        void SetMaxVolume(float value)
        {
            MaxVolumeBackup = value;
            MaxVolume = value;
            RecomputeDivs();
        }

        float GetMaxVolume() const
        {
            return MaxVolume;
        }

        void SetMinVolume(float value)
        {
            MinVolumeBackup = value;
            MinVolume = value;
            RecomputeDivs();
        }

        float GetMinVolume() const
        {
            return MinVolume;
        }

        void SetMinPitch(float value)
        {
            MinPitch = value;
            RecomputeDivs();
        }

        float GetMinPitch() const
        {
            return MinPitch;
        }

        void SetMaxPitch(float value)
        {
            MaxPitch = value;
            RecomputeDivs();
        }

        float GetMaxPitch() const
        {
            return MaxPitch;
        }

        AudSoundBase* GetSoundPtr() const               { return this->SoundBasePtr; }
        void SetSoundPtr(AudSoundBase* sound)           { hook::Thunk<0x4DF580>::Call<void>(this, sound); }
        void DeallocateSoundPtr()                       { hook::Thunk<0x4DF5B0>::Call<void>(this); }
        void Stop()                                     { hook::Thunk<0x4DF5D0>::Call<void>(this); }
        static void BindLua(LuaState L) {
            LuaBinding(L).beginClass<vehEngineSampleWrapper>("vehEngineSampleWrapper")
                .addProperty("Sound", &GetSoundPtr, &SetSoundPtr)
                .addProperty("MinPitch", &GetMinPitch, &SetMinPitch)
                .addProperty("MinVolume", &GetMinVolume, &SetMinVolume)
                .addProperty("MaxPitch", &GetMaxPitch, &SetMaxPitch)
                .addProperty("MaxVolume", &GetMaxVolume, &SetMaxVolume)
                .addProperty("PitchShiftStartRPM", &GetPitchShiftStartRPM, &SetPitchShiftStartRPM)
                .addProperty("PitchShiftEndRPM", &GetPitchShiftEndRPM, &SetPitchShiftEndRPM)
                .addProperty("FadeInStartRPM", &GetFadeInStartRPM, &SetFadeInStartRPM)
                .addProperty("FadeInEndRPM", &GetFadeInEndRPM, &SetFadeInEndRPM)
                .addProperty("FadeOutStartRPM", &GetFadeOutStartRPM, &SetFadeOutStartRPM)
                .addProperty("FadeOutEndRPM", &GetFadeOutEndRPM, &SetFadeOutEndRPM)
                .addFunction("SetPitchShiftRange", &SetPitchShiftRange)
                .addFunction("SetFadeInRange", &SetFadeInRange)
                .addFunction("SetFadeOutRange", &SetFadeOutRange)
                .addFunction("Stop", &Stop)
                .addFunction("DeallocateSoundPtr", &DeallocateSoundPtr)
                .endClass();
        }
    };

    // Lua initialization

}