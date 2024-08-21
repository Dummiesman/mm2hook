#pragma once
#include "common.h"
#include "patch.h"
#include "mm2.h"

class modStaticHandler {
public:
    void DrawOrthoMapped(MM2::modShader* shader, MM2::gfxTexture* tex, float scale, uint texFlagMask);
    static void Install();
};

