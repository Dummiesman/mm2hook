#pragma once

namespace MM2
{
    // Forward declarations
    class modModel;

    // External declarations

    // Class definitions
    class modModel {
    private:
        byte numMaterials; // Shader + Packet count
        byte numMatrices;
        byte flags;
        byte reserved; // unused (padding)

        modShader *shaders;
        gfxPacket **packets;
    public:
        static AGE_API bool ModelAlreadyLoaded(LPCSTR model)    { return hook::StaticThunk<0x597A20>::Call<bool>(model); }
        static AGE_API void DeleteModelHash(LPCSTR model)       { return hook::StaticThunk<0x597A40>::Call<void>(model); }

        AGE_API void GetBoundingBox(Vector3 &a1, Vector3 &a2, Matrix34 *a3) const
                                                                { return hook::Thunk<0x597FB0>::Call<void>(this, &a1, &a2, a3); }
        AGE_API int GetAdjunctCount(void) const                 { return hook::Thunk<0x598190>::Call<int>(this); }
        AGE_API Vector3 & GetPosition(Vector3 &outPos, int a2) const
                                                                { return hook::Thunk<0x598230>::Call<Vector3 &>(this, &outPos, a2); }
        AGE_API void SetPosition(const Vector3 &pos, int a2) const
                                                                { return hook::Thunk<0x598290>::Call<void>(this, &pos, a2); }
        AGE_API void Draw(const Matrix44 *mtx, modShader *shader, uint a3) const
                                                                { return hook::Thunk<0x597D00>::Call<void>(this, mtx, shader, a3); }
        AGE_API void DrawPlain(const Matrix44 *mtx, uint a2) const
                                                                { return hook::Thunk<0x597F00>::Call<void>(this, mtx, a2); }
        AGE_API void DrawWithTexGenCoords(const Matrix44 *mtx, gfxTexture &tex, uint a3) const
                                                                { return hook::Thunk<0x597EA0>::Call<void>(this, mtx, &tex, a3); }
        AGE_API modShader * CopyShaders(void) const             { return hook::Thunk<0x5981C0>::Call<modShader *>(this); }
    };

    // Lua initialization

}