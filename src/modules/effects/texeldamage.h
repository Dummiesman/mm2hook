#pragma once
#include <mm2_common.h>
#include <modules\model\shader.h>
#include <modules\gfx\texture.h>
#include <modules\model\static2.h>

namespace MM2
{
    // Forward declarations
    struct TexelDamageTri;
    struct fxTexelDamage;

    // External declarations


    // Class definitions
    struct TexelDamageTri
    {
        ushort Indices[3];
        ushort Texture;
    };

    struct fxTexelDamage
    {
        int AdjunctCount;
        int TriCount;
        int TextureCount;
        Vector3 *Positions;
        Vector2 *TexCoords;
        TexelDamageTri *DamageTris;

        // Referenced by vehCarModel::Draw @ 0x4CE0CB, offset 0x18
        modShader *CurrentShaders;
        modShader *CleanShaders;
        gfxTexture **DamageTextures;

        ANGEL_ALLOCATOR

        fxTexelDamage();
        float Dot(Vector3 a, Vector3 b);
        void Barycentric(Vector3 p, Vector3 a, Vector3 b, Vector3 c, float& u, float& v, float& w);
        int Init(modStatic* model, modShader* shaders, int shaderCount);
        void Reset();
        void ApplyDamage(Vector3 const& damagePosition, float maxDist);
        void Kill();
    };

    ASSERT_SIZEOF(TexelDamageTri, 8);

    // Allocated by vehCarModel::Init @ 0x4CD408, size 0x24
    ASSERT_SIZEOF(fxTexelDamage, 0x24);

    // Lua initialization

}