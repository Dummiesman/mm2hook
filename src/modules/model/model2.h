#pragma once
#include <mm2_common.h>

namespace MM2
{
    // Forward declarations
    class modModel;

    // External declarations
    extern class modShader;
    extern class gfxPacket;
    extern class gfxTexture;

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
        static AGE_API bool ModelAlreadyLoaded(LPCSTR model);
        static AGE_API void DeleteModelHash(LPCSTR model);
        AGE_API void GetBoundingBox(Vector3 &a1, Vector3 &a2, Matrix34 *a3) const;
        AGE_API int GetAdjunctCount(void) const;
        AGE_API Vector3 &GetPosition(Vector3 &outPos, int a2) const;
        AGE_API void SetPosition(const Vector3 &pos, int a2) const;
        AGE_API void Draw(const Matrix44 *mtx, modShader *shader, uint a3) const;
        AGE_API void DrawShadowed(const Matrix44 *mtx, modShader *shader, uint a3, float intensity) const;
        AGE_API void DrawPlain(const Matrix44 *mtx, uint a2) const;
        AGE_API void DrawWithTexGenCoords(const Matrix44 *mtx, gfxTexture &tex, uint a3) const;
        AGE_API modShader *CopyShaders(void) const;
    };

    // Lua initialization

}