#pragma once
#include "common.h"
#include "patch.h"
#include "mm2.h"

class modShaderHandler {
private:
    const static int MaxStaticMaterials = 2048; // MM2 default is 768
    static int sm_StaticMaterialCount;
    static MM2::gfxMaterial sm_StaticMaterials[MaxStaticMaterials];
public:
    static MM2::gfxMaterial* AddStaticMaterial(MM2::gfxMaterial const& reference);
    static void KillAll();

    static void EndEnvMap();
    static void BeginEnvMap(MM2::gfxTexture* a1, const MM2::Matrix34* a2);
    static void Install();
};