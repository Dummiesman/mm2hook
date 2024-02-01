#include "model2.h"
#include <modules\gfx\rstate.h>
#include <modules\gfx\packet.h>
#include <modules\model\shader.h>

using namespace MM2;

static gfxMaterial ShadowMaterial;

AGE_API bool modModel::ModelAlreadyLoaded(LPCSTR model)
{
    return hook::StaticThunk<0x597A20>::Call<bool>(model);
}

AGE_API void modModel::DeleteModelHash(LPCSTR model)
{
    return hook::StaticThunk<0x597A40>::Call<void>(model);
}

AGE_API void modModel::GetBoundingBox(Vector3 &a1, Vector3 &a2, Matrix34* a3) const
{
    return hook::Thunk<0x597FB0>::Call<void>(this, &a1, &a2, a3);
}

AGE_API int modModel::GetAdjunctCount(void) const
{
    return hook::Thunk<0x598190>::Call<int>(this);
}

AGE_API Vector3 &modModel::GetPosition(Vector3 &outPos, int a2) const
{
    return hook::Thunk<0x598230>::Call<Vector3&>(this, &outPos, a2);
}

AGE_API void modModel::SetPosition(const Vector3 &pos, int a2) const
{
    return hook::Thunk<0x598290>::Call<void>(this, &pos, a2);
}

AGE_API void modModel::Draw(const Matrix44 *mtx, modShader *shader, uint a3) const
{
    return hook::Thunk<0x597D00>::Call<void>(this, mtx, shader, a3);
}

AGE_API void modModel::DrawShadowed(const Matrix44 *mtx, modShader *shader, uint a3, float intensity) const
{
    gfxRenderState::FlushMasked();
    bool lastAlphaEnable = gfxRenderState::GetAlphaEnabled();

    ShadowMaterial.Diffuse = Vector4(0.0f, 0.0f, 0.0f, intensity);
    ShadowMaterial.Ambient = Vector4(0.0f, 0.0f, 0.0f, intensity);
    ShadowMaterial.Emissive = Vector4(0.0f, 0.0f, 0.0f, 0.0f);
    ShadowMaterial.Specular = Vector4(0.0f, 0.0f, 0.0f, 0.0f);
    ShadowMaterial.Shininess = 0.0f;
    gfxRenderState::SetMaterial(&ShadowMaterial);

    for (int i = 0; i < this->numMaterials; i++)
    {
        if (shader != nullptr)
        {
            gfxRenderState::SetTexture(0, shader->GetTexture());
            gfxRenderState::FlushMasked();
        }

        for (gfxPacket* j = this->packets[i]; j; j = j->Next)
        {
            mtx != nullptr ? j->Draw(mtx, a3) : j->Draw(a3);
        }
    }

    gfxRenderState::SetAlphaEnabled(lastAlphaEnable);
}

AGE_API void modModel::DrawPlain(const Matrix44 *mtx, uint a2) const
{
    return hook::Thunk<0x597F00>::Call<void>(this, mtx, a2);
}

AGE_API void modModel::DrawWithTexGenCoords(const Matrix44 *mtx, gfxTexture &tex, uint a3) const
{
    return hook::Thunk<0x597EA0>::Call<void>(this, mtx, &tex, a3);
}

AGE_API modShader* modModel::CopyShaders(void) const
{
    return hook::Thunk<0x5981C0>::Call<modShader*>(this);
}