#pragma once
#include "common.h"
#include "patch.h"
#include "mm2.h"

class TextureVariantHandler {
public:
    static void InitVariantData();
    static void Reset();

    static MM2::gfxImage * LoadTextureVariant(const char *textureName, bool mipmaps);
    static MM2::gfxImage * PrepareTextureVariant(MM2::gfxImage* image, const char *textureName, bool mipmaps);

    static void InstallTextureVariantHandler();

    static void Install();
};

