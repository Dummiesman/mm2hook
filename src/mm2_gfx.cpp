#include "mm2_gfx.h"
using namespace MM2;

/*
    gfxTextureCachePool
*/
declfield(gfxTextureCachePool::sm_FirstPool)(0x68452C);

/*
    gfxTexture
*/
declfield(gfxTexture::sm_First)                 = 0x684D38;
declfield(gfxTexture::sm_UseInternalCache)      = 0x684524;
declfield(gfxTexture::sm_EnableSetLOD)          = 0x684D34;
declfield(gfxTexture::sm_Allow32)               = 0x684D36;

/*
    gfxMaterial
*/
declfield(gfxMaterial::FlatWhite)               = 0x6856F0;