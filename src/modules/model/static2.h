#pragma once
#include <mm2_common.h>

namespace MM2
{
    // Forward declarations
    class modStatic;

    // External declarations
    extern class gfxPacket;
    extern class gfxPacketList;
    extern class gfxTexture;
    extern class modShader;
    AGE_EXT modStatic * modGetStatic(LPCSTR file, void(*a2)(Vector3 &, void *), void *a3, bool a4);

    // Class definitions
    class modStatic {
    public:
        uint8_t PacketCount;
        uint8_t Flags;
        uint16_t FvfFlags;
        uint8_t *ShaderIndices;
        gfxPacket **ppPackets;
        gfxPacketList **ppPacketLists;
    public:
        AGE_API int GetTriCount(void) const;
        AGE_API int GetAdjunctCount(void) const;
        AGE_API void CopyFrom(const modStatic *mod);
        AGE_API modStatic * Clone(void) const;
        AGE_API void Optimize(modShader *shaders);
        AGE_API void Draw(modShader *shaders) const;
        AGE_API void DrawShadowed(modShader* shaders, float intensity) const;
        AGE_API void DrawColored(modShader* shaders, const Vector4& color) const;
        AGE_API void DrawNoAlpha(modShader *shaders) const;
        AGE_API void DrawEnvMapped(modShader *shaders, gfxTexture *envMap, float intensity) const;
        AGE_API void DrawOrthoMapped(modShader *shaders, gfxTexture *tex, float a3, uint a4) const;
        AGE_API void DrawWithTexGenAndTexMatrix(void) const;

        int GetPacketCount() const;
        gfxPacket* GetPacket(int num);
        int GetShaderIndex(int num) const;

        static void BindLua(LuaState L);
    };
}