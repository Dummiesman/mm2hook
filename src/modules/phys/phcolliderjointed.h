#pragma once
#include <modules\phys\phcollider.h>
#include <modules\phys\phjoint.h>

namespace MM2
{
    class phColliderJointed : public phCollider
    {
    public:
        void Attach(phJoint* joint) { hook::Thunk<0x46D4D0>::Call<void>(this, joint); }

        // TODO: Virtuals
    };
}