#pragma once
#include <mm2_common.h>
#include "texture.h"
#include "material.h"

namespace MM2
{
    // Forward declarations
    class gfxRenderStateData;
    class gfxRenderState;
    enum gfxDrawMode;

    // External declarations

    // Class definitions
    // same values as D3DPRIMITIVETYPE
    enum gfxDrawMode {
        DRAWMODE_POINTLIST      = 1,
        DRAWMODE_LINELIST       = 2,
        DRAWMODE_LINESTRIP      = 3,
        DRAWMODE_TRIANGLELIST   = 4,
        DRAWMODE_TRIANGLESTRIP  = 5,
        DRAWMODE_TRIANGLEFAN    = 6,
    };

    class gfxRenderStateData {
        /*
            Don't edit this unless you know what you're doing!
        */
    public:
        bool Clipping;
        bool Lighting;

        byte SrcBlend;              /* D3DBLEND */
        byte DestBlend;             /* D3DBLEND */
        byte VertexBlend;           /* D3DVERTEXBLENDFLAGS */

        int TextureArg;             /* D3DTA_* member */
        int BlendSet;               /* gfxBlendSet */

        float FogStart;             /* D3DRENDERSTATE_FOGSTART */
        float FogEnd;               /* D3DRENDERSTATE_FOGEND */

        byte ZEnable;               /* D3DZBUFFERTYPE */
        byte ShadeMode;             /* D3DSHADEMODE */
        bool ZWriteEnable;
        byte CullMode;              /* D3DCULL */
        byte ZFunc;                 /* D3DCMPFUNC */
        byte AlphaRef;              /* 0x0 - 0xFF */
        byte AlphaFunc;             /* D3DCMPFUNC */
        bool DitherEnable;
        bool AlphaEnable;           /* ALPHABLENDENABLE and ALPHATESTENABLE */
        bool TexturePerspective;
        byte FillMode;              /* D3DSHADEMODE */
        byte ZBias;                 /* 0 - 16 */
        byte AntiAlias;             /* D3DANTIALIASMODE */
        byte FogVertexMode;         /* D3DFOGMODE */

        byte AddrU[2];              /* D3DTSS_ADDRESSU States */
        byte AddrV[2];              /* D3DTSS_ADDRESSV States */
        
        int unk_2C;                 /* unused */
        
        ColorARGB Ambient;

        ColorARGB FogColor;
        float FogDensity;

        byte ColorOp1;
        byte ColorOp2;              /* educated guess -- unused */

        bool SpecularEnable;
        bool FogEnable;
        bool NormalizeNormals;
        bool RangeFogEnable;

        gfxTexture *Texture[2];
        gfxMaterial *Material;
    };

    declhook(0x6854A0, _TypeProxy<gfxRenderStateData>, LASTRSTATE);
    declhook(0x6856A0, _TypeProxy<gfxRenderState>, RSTATE);
    ASSERT_SIZEOF(gfxRenderStateData, 0x50);

    class gfxRenderState {
    public:
        gfxRenderStateData Data;
        gfxMaterial Material;

    private:
        /*
            m_Touched:
            0x01 | State Changed
            0x02 | Texture Changed
            0x04 | Material Changed
            0x08 | Transform Changed
            0x80 | Should Regenerate (gfxRenderState::Regenerate())
        */
        static hook::Type<int> m_Touched;

    private:
        static hook::TypeProxy<Matrix44> sm_Camera;
        static hook::TypeProxy<Matrix44> sm_World;

        static hook::TypeProxy<Matrix44> sm_Composite;
        static hook::TypeProxy<Matrix44> sm_FullComposite;

        static hook::TypeProxy<Matrix44> sm_View;
        static hook::TypeProxy<Matrix44> sm_Modelview;

        // educated guess -- applied to view?
        static hook::TypeProxy<Matrix44> sm_Transform;
    public:
        static void SetCamera(Matrix44 const & mtx)     { hook::StaticThunk<0x4B2A20>::Call<void>(&mtx); }
        static void SetCamera(Matrix34 const & mtx)     { hook::StaticThunk<0x4B2970>::Call<void>(&mtx); }
        static void SetCameraFull(Matrix34 const & mtx) { hook::StaticThunk<0x4B2B50>::Call<void>(&mtx); }
        static void SetView(Matrix34 const & mtx)       { hook::StaticThunk<0x4B2A80>::Call<void>(&mtx); }
    public:
        inline static D3DCULL SetCullMode(D3DCULL cullMode)
        {
            auto original = static_cast<D3DCULL>((&RSTATE->Data)->CullMode);
            if (original != cullMode) {
                (&RSTATE->Data)->CullMode = static_cast<byte>(cullMode);
                gfxRenderState::m_Touched = gfxRenderState::m_Touched | 0x01;
            }
            return original;
        }

        inline static byte SetAlphaRef(byte alphaRef) 
        {
            auto original = (&RSTATE->Data)->AlphaRef;
            if (original != alphaRef) {
                (&RSTATE->Data)->AlphaRef = alphaRef;
                gfxRenderState::m_Touched = gfxRenderState::m_Touched | 0x01;
            }
            return original;
        }

        inline static bool SetLighting(bool enabled)
        {
            auto original = (&RSTATE->Data)->Lighting;
            if (original != enabled) {
                (&RSTATE->Data)->Lighting = enabled;
                gfxRenderState::m_Touched = gfxRenderState::m_Touched | 0x01;
            }
            return original;
        }

        inline static bool SetZWriteEnabled(bool enabled)
        {
            auto original = (&RSTATE->Data)->ZWriteEnable;
            if (original != enabled) {
                (&RSTATE->Data)->ZWriteEnable = enabled;
                gfxRenderState::m_Touched = gfxRenderState::m_Touched | 0x01;
            }
            return original;
        }

        inline static D3DZBUFFERTYPE SetZEnabled(D3DZBUFFERTYPE enabled)
        {
            auto original = static_cast<D3DZBUFFERTYPE>((&RSTATE->Data)->ZEnable);
            if (original != enabled) {
                (&RSTATE->Data)->ZEnable = static_cast<byte>(enabled);
                gfxRenderState::m_Touched = gfxRenderState::m_Touched | 0x01;
            }
            return original;
        }

        inline static bool SetAlphaEnabled(bool enabled)
        {
            auto original = (&RSTATE->Data)->AlphaEnable;
            if (original != enabled) {
                (&RSTATE->Data)->AlphaEnable = enabled;
                gfxRenderState::m_Touched = gfxRenderState::m_Touched | 0x01;
            }
            return original;
        }

        inline static bool SetClipping(bool enabled)
        {
            auto original = (&RSTATE->Data)->Clipping;
            if (original != enabled) {
                (&RSTATE->Data)->Clipping = enabled;
                gfxRenderState::m_Touched = gfxRenderState::m_Touched | 0x01;
            }
            return original;
        }

        inline static bool SetFogEnabled(bool enabled)
        {
            auto original = (&RSTATE->Data)->FogEnable;
            if (original != enabled) {
                (&RSTATE->Data)->FogEnable = enabled;
                gfxRenderState::m_Touched = gfxRenderState::m_Touched | 0x01;
            }
            return original;
        }

        inline static D3DCMPFUNC SetAlphaFunc(D3DCMPFUNC func) 
        {
            auto original = static_cast<D3DCMPFUNC>((&RSTATE->Data)->AlphaFunc);
            if (original != func) {
                (&RSTATE->Data)->AlphaFunc = static_cast<byte>(func);
                gfxRenderState::m_Touched = gfxRenderState::m_Touched | 0x01;
            }
            return original;
        }

        inline static float SetFogStart(float value)
        {
            auto original = (&RSTATE->Data)->FogStart;
            if (original != value) {
                (&RSTATE->Data)->FogStart = value;
                gfxRenderState::m_Touched = gfxRenderState::m_Touched | 0x01;
            }
            return original;
        }

        inline static float SetFogEnd(float value)
        {
            auto original = (&RSTATE->Data)->FogEnd;
            if (original != value) {
                (&RSTATE->Data)->FogEnd = value;
                gfxRenderState::m_Touched = gfxRenderState::m_Touched | 0x01;
            }
            return original;
        }

        inline static ColorARGB SetFogColor(ColorARGB color)
        {
            auto original = (&RSTATE->Data)->FogColor;
            if (original != color) {
                (&RSTATE->Data)->FogColor = color;
                gfxRenderState::m_Touched = gfxRenderState::m_Touched | 0x01;
            }
            return original;
        }

        inline static D3DFOGMODE SetFogVertexMode(D3DFOGMODE func)
        {
            auto original = static_cast<D3DFOGMODE>((&RSTATE->Data)->FogVertexMode);
            if (original != func) {
                (&RSTATE->Data)->FogVertexMode = static_cast<byte>(func);
                gfxRenderState::m_Touched = gfxRenderState::m_Touched | 0x01;
            }
            return original;
        }

        inline static D3DCULL GetCullMode()
        {
            return static_cast<D3DCULL>((&RSTATE->Data)->CullMode);
        }

        inline static byte GetAlphaRef()
        {
            return (&RSTATE->Data)->AlphaRef;
        }

        inline static const Matrix44 & GetCameraMatrix()
        {
            return gfxRenderState::sm_Camera;
        }

        inline static Matrix34 GetViewMatrix()
        {
            Matrix44 fullView = gfxRenderState::sm_View;
            Matrix34 converted = Matrix34();

            fullView.ToMatrix34(converted);
            return converted;
        }

        inline static const Matrix44 & GetWorldMatrix()
        {
            return gfxRenderState::sm_World;
        }

        inline static void SetWorldMatrix(const Matrix44& matrix)
        {
            gfxRenderState::m_Touched = gfxRenderState::m_Touched | 0x88;
            memcpy(&gfxRenderState::sm_World, &matrix, sizeof(Matrix44));
        }

        inline static void SetWorldMatrix(const Matrix34& matrix)
        {
            gfxRenderState::m_Touched = gfxRenderState::m_Touched | 0x88;
            Matrix44::Convert(gfxRenderState::sm_World, matrix);
        }

        inline static byte SetFillMode(byte fillMode)
        {
            auto original = (&RSTATE->Data)->FillMode;
            if (original != fillMode) {
                (&RSTATE->Data)->FillMode = fillMode;
                gfxRenderState::m_Touched = gfxRenderState::m_Touched | 0x88;
            }
            return original;
        }

        static void BindLua(LuaState L) {
            LuaBinding(L).beginClass<gfxRenderState>("gfxRenderState")
                .addStaticProperty("WorldMatrix", &GetWorldMatrix, static_cast<void(*)(const Matrix44 &)>(&gfxRenderState::SetWorldMatrix))
                .addStaticProperty("ViewMatrix", &GetViewMatrix, &SetView)
                .addStaticProperty("CameraMatrix", &GetCameraMatrix, static_cast<void(*)(const Matrix44&)>(&gfxRenderState::SetCamera))
                .endClass();
        }
    };
    ASSERT_SIZEOF(gfxRenderState, 0x98);
}