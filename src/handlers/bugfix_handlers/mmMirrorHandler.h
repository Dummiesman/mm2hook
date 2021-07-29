#pragma once
#include "common.h"
#include "patch.h"
#include "mm2.h"

class mmMirrorHandler {
public:
    static bool IsDrawing;
    void Cull();
    static void Install();
};