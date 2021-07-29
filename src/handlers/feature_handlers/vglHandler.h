#pragma once
#include "common.h"
#include "patch.h"
#include "mm2.h"

class vglHandler {
public:
    static bool insideTunnel;
    static unsigned int vglColor;
    static unsigned int vglCalculatedColor;
    static ColorARGB vglResultColor;

    static void vglBeginShaded(MM2::gfxDrawMode drawMode, int p1);
    static void vglEndShaded();

    static void Install();
};

