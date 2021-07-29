#pragma once
#include "common.h"
#include "patch.h"
#include "mm2.h"

class vehTrailerInstanceFeatureHandler {
public:
    void Draw(int a1);
    void DrawTwhl4(int a1, int a2, MM2::Matrix34* a3, MM2::modShader* a4);
    void DrawTwhl5(int a1, int a2, MM2::Matrix34* a3, MM2::modShader* a4);
    void DrawGlow();
    void DrawPart(int a1, int a2, MM2::Matrix34* a3, MM2::modShader* a4);
    void DrawPartReflections(MM2::modStatic* a1, MM2::Matrix34* a2, MM2::modShader* a3);
    void AddGeomHook(const char* pkgName, const char* name, int flags);
    static void Install();
};