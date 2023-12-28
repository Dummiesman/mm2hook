#pragma once
#include <modules\gfx.h>

namespace MM2
{
    // Forward declarations
    struct vglVCT1_t;
    struct vglVNT1_t;
    enum rglToken;

    // External declarations
    extern void vglTexCoord2f(Vector2 uv);
    extern void vglTexCoord2f(float u, float v);
    extern void vglVertex3f(float x, float y, float z);
    extern void vglVertex3f(const Vector3 position);
    extern void vgl_VERTEX_VCT1(float x, float y, float z);
    extern void vgl_VERTEX_VNT1(float x, float y, float z);
    extern void vglSetFormat(uint p1, uint p2, uint p3, uint p4);
    extern void vglBegin(gfxDrawMode drawMode, int p2);
    extern void vglEnd(void);
    extern void vglBeginBatch(void);
    extern void vglEndBatch(void);
    extern void vglDrawTexture(unsigned int index);
    extern void vglBindTexture(gfxTexture *texture);
    extern void vglDrawLabel(const Vector3 &position, const char *text);

    extern void vglDrawLabelf(const Vector3 &position, const char *format, ...);

    extern void vglSetCloudMap(const char *texture);
    extern void vglSetOffset(float x, float y, float z);
    extern bool rglEnableDisable(rglToken token, bool value);
    extern bool rglIsEnabled(rglToken token);
    extern void rglWorldMatrix(const Matrix34 &mtx);
    extern void rglWorldIdentity(void);
    extern void rglCard(const Vector3 &position);
    extern void rglBindTexture2(const gfxTexture *texture);
    extern void tglDrawParticle(const Vector3 &position, float size, const Vector4& color);
    extern void tglDrawParticleClipAdjusted(const Vector3& position, float size, const Vector4& color, float anticlip);
    extern void tglDrawRotatedParticle(const Vector3 &p1, float p2, float p3, const Vector4 &p4);

    extern uint mkfrgba(float r, float g, float b, float a);

    extern void rglOpenPipe(int p1, int p2, bool p3);
    extern void rglClosePipe(void);
    extern void rglBeginFrame(uint p1);
    extern void rglEndFrame(void);
    extern void rglCameraMatrix(const Matrix34 &p1);
    extern int rglAlive(void);
    extern void rglDrawBox(const Vector3 &p1, const Matrix34 &p2);
    extern void rglDrawSolidBox(const Vector3 &p1, const Matrix34 &p2, const Vector3 &p3);
    extern void rglDrawAxis(float p1, const Matrix34 &p2);
    extern void rglDrawSphere(float p1, int p2);
    extern void rglDrawSphere(float p1, const Matrix34 &p2, int p3);
    extern void rglDrawEllipsoid(const Vector3 &p1, const Matrix34 &p2, int p3);
    extern void rglDrawParticle(const Vector3 &p1, float p2, const Vector4 &p3);
    extern void rglResetDrawTexture(void);
    extern void rglDrawTexture(gfxTexture *texture);
    extern void rglPushMatrix(void);
    extern void rglPopMatrix(void);
    extern void rglMultMatrix(Matrix34 &p1);

    extern hook::Type<uint>              vglCloudMapEnable;

    extern hook::Type<void(*)(float, float, float)>
                                         vglVertex;

    extern hook::Type<uint>              vglCurrentColor;
    extern hook::Type<vglVCT1_t *>       vglVCT1;
    extern hook::Type<float>             vglCurrentNZ;
    extern hook::Type<float>             vglCurrentNX;
    extern hook::Type<float>             vglCurrentNY;
    extern hook::Type<int>               vglCount;
    extern hook::Type<float>             vglCurrentT;
    extern hook::Type<float>             vglCurrentS;
    extern hook::Type<vglVNT1_t *>       vglVNT1;
    extern hook::Type<gfxTexture *>      vglCloudMapTexture;

    extern hook::Type<Matrix44 *>        mtxstack;

    /*
        Lua helper
    */
    extern void luaSetColor(const Vector4 & color);
    extern void luaGfxDrawFont(double x, double y, const char* text);
    extern void vglUnbindTexture();

    // Class definitions
    enum rglToken {
        RGL_DEPTH_TEST = 0,
        RGL_CULL_FACE = 1,
        RGL_LIGHTING = 2,
        RGL_BLEND = 3,
    };

    /* Custom implementation of D3D[T]LVERTEX */
    struct vglVCT1_t {
        Vector3 position;

        DWORD reserved;

        DWORD color;
        DWORD specular;

        Vector2 texCoords;
    };

    /* Custom implementation of D3DVERTEX */
    struct vglVNT1_t {
        Vector3 position;
        Vector3 normals;

        Vector2 texCoords;
    };

    // Lua initialization

}