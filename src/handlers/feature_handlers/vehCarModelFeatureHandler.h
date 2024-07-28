#pragma once
#include "common.h"
#include "patch.h"
#include "mm2.h"

class vehCarModelFeatureHandler {
public:
    void Draw(int a1);
    void DrawGlow();
    void DrawShadow();
    void ModStaticDraw(MM2::modShader * a1);
    void ApplyImpact(MM2::vehDamageImpactInfo* a1);
    void SetVariant(int a1);
    const MM2::Matrix34& GetMatrix(MM2::Matrix34* a1);
    static void Install();
};

