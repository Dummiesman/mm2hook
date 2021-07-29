#pragma once
#include "common.h"
#include "patch.h"
#include "mm2.h"

class mmPlayerHandler {
public:
    static bool playerCanFly;

    void Splash();
    void Zoink();
    void Update();
    void Reset();
    void PlayExplosion();
    void BustPerp();
    void BustOpp();
    static void Install();
};

