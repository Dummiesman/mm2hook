#pragma once
#include "common.h"
#include "patch.h"
#include "mm2.h"

class vehCableCarInstanceHandler {
public:
    void DrawShadow();
    void DrawGlow();
    bool BeginGeom(const char* a1, const char* a2, int a3);
    static void Install();
};

