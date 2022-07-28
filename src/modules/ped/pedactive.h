#pragma once
#include <modules\ped.h>

namespace MM2
{
    // Forward declarations
    class pedActive;
    class pedActiveData;

    // External declarations
    extern class crSkeleton;
    extern class dgPhysEntity;
    extern class dgRagdoll;

    // Class definitions

    //TODO
    class pedActiveData {};

    class pedActive : public dgPhysEntity {
    private:
        byte _buffer[0x60];
    protected:
        static hook::Field<0x104, crSkeleton *> _skeleton;
        static hook::Field<0x108, dgRagdoll *> _ragdoll;
    public:
        inline crSkeleton * GetSkeleton() const {
            return _skeleton.get(this);
        }

        inline dgRagdoll * GetRagdoll() const {
            return _ragdoll.get(this);
        }
    };
    
    ASSERT_SIZEOF(pedActive, 0x114);
}