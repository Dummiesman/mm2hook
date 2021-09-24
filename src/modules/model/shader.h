#pragma once
#include <modules\gfx\material.h>

namespace MM2
{
    // Forward declarations
    class modShader;

    // External declarations
    extern class gfxTexture;
    extern struct gfxMaterial;

    // Class definitions
    class modShader {
    private:
        gfxTexture *Texture;
        gfxMaterial *Material;
    public:
        inline void setTexture(gfxTexture* texture)
        {
            this->Texture = texture;
        }

        inline gfxTexture* getTexture()
        {
            return this->Texture;
        }

        inline gfxMaterial* getMaterial()
        {
            return this->Material;
        }

        inline void setMaterial(gfxMaterial* material)
        {
            this->Material = material;
        }
    public:
        static hook::Type<gfxMaterial *> sm_Materials;
        static hook::Type<int> sm_StaticMaterialCount;

        static AGE_API void BeginEnvMap(gfxTexture *tex, const Matrix34 &mtx)
                                                                { return hook::StaticThunk<0x4A41B0>::Call<void>(tex, &mtx); }
        static AGE_API void EndEnvMap(void)                     { return hook::StaticThunk<0x4A4420>::Call<void>(); }
        static AGE_API modShader ** LoadShaderSet(Stream *stream, int *a2, int *a3, bool a4)
                                                                { return hook::StaticThunk<0x4A3F60>::Call<modShader **>(stream, a2, a3, a4); }
        static AGE_API gfxMaterial * AddStaticMaterial(const gfxMaterial &mtl)
                                                                { return hook::StaticThunk<0x4A3930>::Call<gfxMaterial *>(&mtl); }
        static AGE_API void KillAll(void)                       { return hook::StaticThunk<0x4A3B20>::Call<void>(); }
        
        AGE_API void Load(Stream *stream, bool a2)              { return hook::Thunk<0x4A3B30>::Call<void>(this, stream, a2); }
        AGE_API void PreLoad(void)                              { return hook::Thunk<0x4A40C0>::Call<void>(this); }

        static void BindLua(LuaState L) {
            LuaBinding(L).beginClass<modShader>("modShader")
                .addStaticFunction("BeginEnvMap", &BeginEnvMap)
                .addStaticFunction("EndEnvMap", &EndEnvMap)
                .addPropertyReadOnly("Texture", &getTexture)
                .addPropertyReadOnly("Material", &getMaterial)
                .endClass();
        }
    };

    // Lua initialization

}