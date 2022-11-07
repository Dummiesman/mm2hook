#pragma once
#include "common.h"
#include "patch.h"
#include "mm2.h"

class BridgeFerryHandler {
public:
    void Cull();
    void Draw(int lod);

    static void Install();
};