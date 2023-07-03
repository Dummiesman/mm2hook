#pragma once
#include "aisubwayaudio.h"

using namespace MM2;

AGE_API void aiSubwayAudio::Activate(int index)
{
    if (index < 0)
    {
        for (int i = 0; i < this->GetSoundCount(); i++)
        {
            this->ActivateSound(i);
        }
    }
    else if (index < this->GetSoundCount())
    {
        this->ActivateSound(index);
    }
}

AGE_API void aiSubwayAudio::Deactivate(int index)
{
    if (index < 0)
    {
        for (int i = 0; i < this->GetSoundCount(); i++)
        {
            this->DeactivateSound(i);
        }
    }
    else if (index < this->GetSoundCount())
    {
        this->DeactivateSound(index);
    }
}

AGE_API void aiSubwayAudio::Update(float speed)
{
    // States
    // 0 - Moving
    // 1 - Stopped
    // 2 - Accelerating
    // 3 - Braking

    if (speed > 0.0f)
    {
        if (State == 1)
        {
            Deactivate();
            Activate(0);
            State = 2; 
        }
        else if (State == 2)
        {
            if (speed >= 50.0f)
            {
                State = 0;
            }
        }
        else if (State == 0)
        {
            if (speed < 50.0f)
            {
                Activate(2);
                State = 3;
            }
        }
        else if (State == 3)
        {
            // braking state, nothing to do here
        }
    }
    else
    {
        if (State != 1)
        {
            Deactivate();
            Activate(1);
            State = 1;
        }
    }
    Aud3DAmbientObject::Update(speed);
}