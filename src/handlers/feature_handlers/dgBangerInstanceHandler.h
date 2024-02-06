#pragma once
#include "common.h"
#include "patch.h"
#include "mm2.h"

class dgBangerInstanceHandler {
public:
    bool dgBangerInstance_BeginGeom(const char* a1, const char* a2, int a3); // Shadow flag hook
    void dgBangerManager_Init(int poolSize); // Shadow flag hook

    static void tglDrawParticle(const MM2::Vector3& position, float size, const MM2::Vector4& color);
    void DrawGlow();
    void DrawShadow();
    static void Reset();
    static void Install();
};

