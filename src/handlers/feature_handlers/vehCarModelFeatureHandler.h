#pragma once
#include "common.h"
#include "patch.h"
#include "mm2.h"

class vehCarModelFeatureHandler {
public:
    void Draw(int a1);
    void DrawGlow();
    void ModStaticDraw(MM2::modShader * a1);
    void EjectOneShot();
    static void Install();
};

