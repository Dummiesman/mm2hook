#pragma once
#include "common.h"
#include "patch.h"
#include "mm2.h"

class aiVehicleInstanceFeatureHandler {
public:
    void Draw(int lod);
    void DrawGlow();
    void DrawShadow();
    void AddGeomHook(const char* pkgName, const char* name, int flags);

    void VehicleSpline_DrawId();
    void Ambient_DrawId();

    static void Install();
};

