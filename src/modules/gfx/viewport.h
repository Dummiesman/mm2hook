#pragma once

namespace MM2
{
    // Forward declarations
    class gfxViewport;

    // External declarations


    // Class definitions
    class gfxViewport
    {
    private:  
        Matrix44 m_Projection;
        Matrix44 m_ViewModel;
        Matrix44 Camera;
        Matrix44 World;
        int field_100;
        int field_104;
        int field_108;
        int field_10C;
        int field_110;
        int field_114;
        int field_118;
        int field_11C;
        int Flags;
        float DefaultAspect;
        int field_128;
        float Near;
        float Far;
        float field_134;
        float field_138;
        float field_13C;
        float float140;
        float float144;
        float float148;
        float float14C;
        float float150;
        float float154;
        float float158;
        D3DVIEWPORT7 m_Viewport;
        float Fov;
        float Aspect;
    public:
        AGE_API void ResetWindow()
        {
            hook::Thunk<0x4B14A0>::Call<void>(this);
        }

        AGE_API void Ortho(float left, float right, float bottom, float top, float nearVal, float farVal) 
        {
            hook::Thunk<0x4B1800>::Call<void>(this, left, right, bottom, top, nearVal, farVal); 
        }

        AGE_API void Perspective(float fov, float aspect, float nearClip, float farClip) 
        {
            hook::Thunk<0x4B1640>::Call<void>(this, fov, aspect, nearClip, farClip);
        }

        AGE_API void SetWindow(int left, int top, int width, int height, float dvMinZ, float dvMaxZ)
        {
            hook::Thunk<0x4B14C0>::Call<void>(this, left, top, width, height, dvMinZ, dvMaxZ);
        }
    };

    ASSERT_SIZEOF(gfxViewport, 0x17C);
}