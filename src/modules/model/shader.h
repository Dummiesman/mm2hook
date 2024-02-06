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
        gfxTexture* Texture; // Not neccessarily a gfxTexture*. If bit 1 is set, its a char* (not loaded)
        gfxMaterial* Material;
    public:
        void SetTexture(gfxTexture* texture)
        {
            this->Texture = texture;
        }

        gfxTexture* GetTexture() const
        {
            if (IsPreLoaded())
                return this->Texture;
            else
                return nullptr;
        }

        bool IsPreLoaded() const
        {
            return ((int)Texture & SHADER_NOT_LOADED) != SHADER_NOT_LOADED;
        }

        char* GetTextureName() const
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

        gfxMaterial* GetMaterial() const
        {
            return this->Material;
        }

        void SetMaterial(gfxMaterial* material)
        {
            this->Material = material;
        }
    public:
        static hook::Type<gfxMaterial *> sm_Materials;
        static hook::Type<int> sm_StaticMaterialCount;

        static AGE_API void BeginEnvMap(gfxTexture *tex, const Matrix34 &mtx)
                                                                { return hook::StaticThunk<0x4A41B0>::Call<void>(tex, &mtx); }
        static AGE_API void EndEnvMap(void)                     { return hook::StaticThunk<0x4A4420>::Call<void>(); }
        static AGE_API modShader ** LoadShaderSet(Stream *stream, int *variantCount, int *shadersPerVariant, bool preload)
                                                                { return hook::StaticThunk<0x4A3F60>::Call<modShader **>(stream, variantCount, shadersPerVariant, preload); }
        static AGE_API gfxMaterial * AddStaticMaterial(const gfxMaterial &mtl)
                                                                { return hook::StaticThunk<0x4A3930>::Call<gfxMaterial *>(&mtl); }
        static AGE_API void KillAll(void)                       { return hook::StaticThunk<0x4A3B20>::Call<void>(); }
        
        AGE_API void Load(Stream *stream, bool a2)              { return hook::Thunk<0x4A3B30>::Call<void>(this, stream, a2); }
        AGE_API void PreLoad(void)                              { return hook::Thunk<0x4A40C0>::Call<void>(this); }

        static void BindLua(LuaState L) {
            LuaBinding(L).beginClass<modShader>("modShader")
                .addStaticFunction("BeginEnvMap", &BeginEnvMap)
                .addStaticFunction("EndEnvMap", &EndEnvMap)
                .addProperty("Texture", &GetTexture, &SetTexture)
                .addProperty("Material", &GetMaterial, &SetMaterial)
                .endClass();
        }
    };

    // Lua initialization

}