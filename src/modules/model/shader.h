#pragma once
#include <modules\gfx\material.h>
#include <modules\gfx\texture.h>

namespace MM2
{
    // Forward declarations
    class modShader;

    // External declarations
    extern struct gfxMaterial;
    extern class Stream;

    // Class definitions
    #define SHADER_NOT_LOADED 1

    class modShader {
    private:
        const static int MAX_STATIC_MATERIALS = 2048; // MM2 default is 768
        static int sm_StaticMaterialCount;
        static MM2::gfxMaterial sm_StaticMaterials[MAX_STATIC_MATERIALS];
    private:
        gfxTexture* Texture; // Not neccessarily a gfxTexture*. If bit 1 is set, its a char* (not loaded)
        gfxMaterial* Material;
    public:
        void SetTexture(gfxTexture* texture);
        gfxTexture* GetTexture() const;
        bool IsPreLoaded() const;
        char* GetTextureName() const;
        gfxMaterial* GetMaterial() const;
        void SetMaterial(gfxMaterial* material);
    public:
        static AGE_API void BeginEnvMap(gfxTexture* tex, const Matrix34& mtx);
        static AGE_API void EndEnvMap();
        static AGE_API modShader** LoadShaderSet(Stream* stream, int* variantCount, int* shadersPerVariant, bool preload);
        static AGE_API gfxMaterial* AddStaticMaterial(const gfxMaterial& mtl);
        static AGE_API void KillAll();
        AGE_API void Load(Stream* stream, bool compact);
        AGE_API void PreLoad();

        static void BindLua(LuaState L);
    };
}