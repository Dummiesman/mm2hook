#pragma once

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
        AGE_API int GetTriCount(void) const	                    { return hook::Thunk<0x4A4DE0>::Call<int>(this); }
        AGE_API int GetAdjunctCount(void) const	                { return hook::Thunk<0x4A4DB0>::Call<int>(this); }
        AGE_API void CopyFrom(const modStatic *mod)	            { return hook::Thunk<0x4A4D60>::Call<void>(this, mod); }
        AGE_API modStatic * Clone(void) const	                { return hook::Thunk<0x4A4CA0>::Call<modStatic *>(this); }
        AGE_API void Optimize(modShader *shader)	            { return hook::Thunk<0x4A49A0>::Call<void>(this, shader); }
        AGE_API void Draw(modShader *shader) const	            { return hook::Thunk<0x4A4550>::Call<void>(this, shader); }
        AGE_API void DrawNoAlpha(modShader *shader) const	    { return hook::Thunk<0x4A4A20>::Call<void>(this, shader); }
        AGE_API void DrawEnvMapped(modShader *shader, gfxTexture *tex, float a3) const
                                                                { return hook::Thunk<0x4A4A50>::Call<void>(this, shader, tex, a3); }
        AGE_API void DrawOrthoMapped(modShader *shader, gfxTexture *tex, float a3, uint a4) const
                                                                { return hook::Thunk<0x4A4B30>::Call<void>(this, shader, tex, a3, a4); }
        AGE_API void DrawWithTexGenAndTexMatrix(void) const	    { return hook::Thunk<0x4A4C50>::Call<void>(this); }

        static void BindLua(LuaState L) {
            LuaBinding(L).beginClass<modStatic>("modStatic")
                //functions
                .addFunction("GetTriCount", &GetTriCount)
                .addFunction("GetAdjunctCount", &GetAdjunctCount)
                .endClass();
        }
    };

    // Lua initialization

}