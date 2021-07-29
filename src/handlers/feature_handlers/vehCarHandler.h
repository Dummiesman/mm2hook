#pragma once
#include "common.h"
#include "patch.h"
#include "mm2.h"

class vehCarHandler : handler_t {
public:
    void InitCar(LPCSTR vehName, int a2, int a3, bool a4, bool a5);
    void InitCarAudio(LPCSTR a1, int a2);
    const MM2::phBound * GetModelBound(int a1);

    void Mm1StyleTransmission();
    void Update();

    static void Install(void);
};

