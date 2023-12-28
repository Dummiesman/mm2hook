#pragma once
#include "common.h"
#include "patch.h"
#include "mm2.h"

class dgBangerInstanceHandler {
public:
    static void tglDrawParticle(const MM2::Vector3& position, float size, const MM2::Vector4& color);
    void DrawGlow();
    static void Reset();
    static void Install();
};

