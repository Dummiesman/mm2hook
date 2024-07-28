#pragma once
#include "common.h"
#include "patch.h"
#include "mm2.h"

class mmMirrorHandler {
public:
    static bool IsDrawing;
    static void Clear(DWORD flags, int color, D3DVALUE dvZ, DWORD dwStencil);
    void Cull();
    static void Install();
};