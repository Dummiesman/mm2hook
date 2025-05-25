#pragma once
#include <modules/vector/vector4.h>

namespace MM2
{
    // Forward declarations
    struct gfxMaterial;

    // External declarations


    // Class definitions
    struct gfxMaterial {
    public:
        static hook::Type<gfxMaterial> FlatWhite;
    public:
        Vector4 Diffuse;
        Vector4 Ambient;
        Vector4 Specular;
        Vector4 Emissive;

        float Shininess;

        ColorARGB Color; // 32-bit diffuse color

        gfxMaterial() 
        {
            Reset();
        }

        AGE_API void Reset() 
        {
            Diffuse = Vector4(1.0f, 1.0f, 1.0f, 1.0f);
            Ambient = Vector4(1.0f, 1.0f, 1.0f, 1.0f);
            Specular = Vector4(0.0f, 0.0f, 0.0f, 0.0f);
            Emissive = Vector4(0.0f, 0.0f, 0.0f, 0.0f);
            Shininess = 0.0f;
            Color = 0xFFFFFFFF;
        };

        void Copy(gfxMaterial const& copyFrom)
        {
            Diffuse = copyFrom.Diffuse;
            Ambient = copyFrom.Ambient;
            Specular = copyFrom.Specular;
            Emissive = copyFrom.Emissive;
            Shininess = copyFrom.Shininess;
            Color = copyFrom.Color;
        }

        static void BindLua(LuaState L) {
            LuaBinding(L).beginClass<gfxMaterial>("gfxMaterial")
                .addConstructor(LUA_ARGS())
                .addVariable("Diffuse", &gfxMaterial::Diffuse)
                .addVariable("Ambient", &gfxMaterial::Ambient)
                .addVariable("Specular", &gfxMaterial::Specular)
                .addVariable("Emissive", &gfxMaterial::Emissive)
                .addVariable("Shininess", &gfxMaterial::Shininess)
                .addFunction("Copy", &Copy)
                .addFunction("Reset", &Reset)
                .endClass();
        }
    };

    // Lua initialization

}