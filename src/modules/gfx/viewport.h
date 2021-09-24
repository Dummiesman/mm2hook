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
        void Ortho(float a1, float a2, float a3, float a4, float a5, float a6) { hook::Thunk<0x4B1800>::Call<void>(this, a1, a2, a3, a4, a5, a6); }
    };

    // Lua initialization

}