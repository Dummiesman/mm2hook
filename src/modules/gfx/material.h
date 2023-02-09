#pragma once

namespace MM2
{
    // Forward declarations
    struct gfxMaterial;

    // External declarations


    // Class definitions
    struct gfxMaterial {
        Vector4 Diffuse;
        Vector4 Ambient;
        Vector4 Specular;
        Vector4 Emissive;

        float Shininess;

        ColorARGB Color; // 32-bit diffuse color

        AGE_API void Reset(void) {
            hook::Thunk<0x4B1BB0>::Call<void>(this);
        };

        static void BindLua(LuaState L) {
            LuaBinding(L).beginClass<gfxMaterial>("gfxMaterial")
                .addVariable("Diffuse", &gfxMaterial::Diffuse)
                .addVariable("Ambient", &gfxMaterial::Ambient)
                .addVariable("Specular", &gfxMaterial::Specular)
                .addVariable("Emissive", &gfxMaterial::Emissive)
                .addVariable("Shininess", &gfxMaterial::Shininess)
                .endClass();
        }
    };

    // Lua initialization

}