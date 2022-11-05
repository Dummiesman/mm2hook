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
    };
}