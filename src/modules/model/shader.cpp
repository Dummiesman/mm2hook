#include "shader.h"
#include <modules/gfx/rstate.h>

using namespace MM2;

gfxMaterial modShader::sm_StaticMaterials[modShader::MAX_STATIC_MATERIALS];
int modShader::sm_StaticMaterialCount = 0;
float oldFogStart, oldFogEnd;

void modShader::SetTexture(gfxTexture* texture)
{
    this->Texture = texture;
}

gfxTexture* modShader::GetTexture() const
{
    if (IsPreLoaded())
        return this->Texture;
    else
        return nullptr;
}

bool modShader::IsPreLoaded() const
{
    return ((int)Texture & SHADER_NOT_LOADED) != SHADER_NOT_LOADED;
}

char* modShader::GetTextureName() const
{
    char* textureName = nullptr;
    if (!IsPreLoaded())
    {
        textureName = ((char*)Texture - 1);
    }
    else if(Texture != nullptr)
    {
        textureName = Texture->GetName();
    }
    return textureName;
}

gfxMaterial* modShader::GetMaterial() const
{
    return this->Material;
}

void modShader::SetMaterial(gfxMaterial* material)
{
    this->Material = material;
}

void modShader::BeginEnvMap(gfxTexture* tex, const Matrix34& mtx)
{
    // Disable fog so it's not blended with reflections
    oldFogStart = gfxRenderState::SetFogStart(9999.f);
    oldFogEnd = gfxRenderState::SetFogEnd(10000.f);

    hook::StaticThunk<0x4A41B0>::Call<void>(tex, &mtx); //call original
}

void modShader::EndEnvMap()             
{
    // Restore last fog settings
    gfxRenderState::SetFogStart(oldFogStart);
    gfxRenderState::SetFogEnd(oldFogEnd);

    hook::StaticThunk<0x4A4420>::Call<void>(); //call original
}

modShader ** modShader::LoadShaderSet(Stream *stream, int *variantCount, int *shadersPerVariant, bool preload)
                                            { return hook::StaticThunk<0x4A3F60>::Call<modShader **>(stream, variantCount, shadersPerVariant, preload); }
gfxMaterial* modShader::AddStaticMaterial(const gfxMaterial& mtl)
{
    // find our material in the list
    for (int i = 0; i < sm_StaticMaterialCount; i++)
    {
        auto& compareTo = sm_StaticMaterials[i];
        if (compareTo.Ambient == mtl.Ambient &&
            compareTo.Diffuse == mtl.Diffuse &&
            compareTo.Specular == mtl.Specular &&
            compareTo.Emissive == mtl.Emissive &&
            compareTo.Shininess == mtl.Shininess)
        {
            return &sm_StaticMaterials[i];
        }
    }

    // if it's not found, add a new one
    if (sm_StaticMaterialCount >= modShader::MAX_STATIC_MATERIALS)
    {
        Quitf("Out of static materials!");
    }

    sm_StaticMaterials[sm_StaticMaterialCount] = mtl;
    sm_StaticMaterialCount++;
    return &sm_StaticMaterials[sm_StaticMaterialCount - 1];
}

void modShader::KillAll(void)
{
    sm_StaticMaterialCount = 0;
}

void modShader::Load(Stream *stream, bool a2)              { return hook::Thunk<0x4A3B30>::Call<void>(this, stream, a2); }
void modShader::PreLoad(void)                              { return hook::Thunk<0x4A40C0>::Call<void>(this); }

void modShader::BindLua(LuaState L) {
    LuaBinding(L).beginClass<modShader>("modShader")
        .addStaticFunction("AddStaticMaterial", &AddStaticMaterial)
        .addStaticFunction("BeginEnvMap", &BeginEnvMap)
        .addStaticFunction("EndEnvMap", &EndEnvMap)
        .addProperty("Texture", &GetTexture, &SetTexture)
        .addProperty("Material", &GetMaterial, &SetMaterial)
        .endClass();
}
