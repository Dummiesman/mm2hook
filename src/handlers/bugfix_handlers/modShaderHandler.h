#pragma once
#include "common.h"
#include "patch.h"
#include "mm2.h"

class modShaderHandler {
public:
    static MM2::gfxMaterial* AddStaticMaterial(MM2::gfxMaterial const& reference);
    static void KillAll();

    static void EndEnvMap();
    static void BeginEnvMap(MM2::gfxTexture* a1, const MM2::Matrix34& a2);
    static void Install();
};