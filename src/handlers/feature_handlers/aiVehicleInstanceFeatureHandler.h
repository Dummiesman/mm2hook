#pragma once
#include "common.h"
#include "patch.h"
#include "mm2.h"

class aiVehicleInstanceFeatureHandler {
public:
    void Draw(int a1);
    void DrawGlow();
    void DrawShadow();
    void ModStaticDraw(MM2::modShader* a1);
    void AddGeomHook(const char* pkgName, const char* name, int flags);

    void VehicleSpline_DrawId();
    void Ambient_DrawId();

    static void Install();
};

