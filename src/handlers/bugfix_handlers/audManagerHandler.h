#pragma once
#include "common.h"
#include "patch.h"
#include "mm2.h"

class audManagerHandler {
public:
    void Init(int, int, int, char*, short, short);

    void AssignCDVolume(float value);
    void SetupCDAudio(float);

    void SetMixerCDVolume(float value);
    void SetMixerWaveVolume(float value);

    bool MinInstall();

    static void Install();
};