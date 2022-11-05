#pragma once
#include <modules\node.h>
#include <modules\vector.h>

namespace MM2
{
    // Forward declarations
    class mmMirror;

    // External declarations
    extern class gfxViewport;

    // Class definitions
    class mmMirror : public asNode
    {
    private:
        Matrix34 matrix;
        Vector2 Size;
        gfxViewport* Viewport;
        float Fov;
        float Aspect;
        float NearClip;
        float FarClip;
    public:
        Vector3 GetPosition() const;
        void SetPosition(const Vector3& position);

        Vector2 GetSize() const;
        void SetSize(const Vector2& size);

        float GetFov() const;
        void SetFov(float fov);

        float GetAspect() const;
        void SetAspect(float aspect);

        float GetNearClip() const;
        float GetFarClip() const;
        void SetNearClip(float clip);
        void SetFarClip(float clip);

        static void BindLua(LuaState L);
    };
}