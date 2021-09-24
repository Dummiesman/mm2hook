#pragma once

namespace MM2
{
    // Forward declarations
    struct gfxLight;

    // External declarations


    // Class definitions
    /* Basically a 1:1 copy of D3DLIGHT */
    struct gfxLight {
    public:
        D3DLIGHTTYPE Type;

        Vector4 Diffuse;
        Vector4 Specular;
        Vector4 Ambient;

        Vector3 Position;
        Vector3 Direction;

        float Range;
        float Falloff;

        float Attenuation0;
        float Attenuation1;
        float Attenuation2;

        float Theta;
        float Phi;

        void Reset() {
            hook::Thunk<0x4B1C00>::Call<void>(this);
        };
    };

    // Lua initialization

}