#pragma once
#include "phcollider.h"
#include "phjoint.h"

namespace MM2
{
    class phColliderJointed : public phCollider
    {
    public:

        /*
            phColliderBase virtuals
        */
        virtual void GetInvMassMatrix(Vector3 const& a1, Vector3 const& a2, Matrix34& a3, int a4) const override
                                                            { hook::Thunk<0x46D5F0>::Call<void>(this, &a1, &a2, &a3, a4); }
        virtual void GetInvMassMatrix(Vector3 const& a1, Matrix34& a2, int a3) const override
                                                            { hook::Thunk<0x46D580>::Call<void>(this, &a1, &a2, a3); }
        virtual void Impact(class phImpactBase const* a1, Vector3 const& a2) override
                                                            { hook::Thunk<0x46D500>::Call<void>(this, a1, &a2); }
        virtual void Impact(class phImpactBase const* a1, Vector3 const& a2, Vector3 const& a3) override
                                                            { hook::Thunk<0x46D4E0>::Call<void>(this, a1, &a2, &a3); }
        virtual void Contact(Vector3 const& a1, class phContact const* a2) override
                                                            { hook::Thunk<0x46D560>::Call<void>(this, &a1, a2); }
        virtual void Contact(Vector3 const& a1, class phContact const* a2, Matrix34 const& a3) override
                                                            { hook::Thunk<0x46D540>::Call<void>(this, &a1, a2, &a3); }
        virtual void Contact(class phImpactBase const* a1, Vector3 const& a2, Vector3 const& a3, Vector3 const& a4, Matrix34 const& a5) override
                                                            { hook::Thunk<0x46D520>::Call<void>(this, a1, &a2, &a3, &a4, &a5); }

        /*
            phColliderJointed members
        */
        void Attach(phJoint* joint) { hook::Thunk<0x46D4D0>::Call<void>(this, joint); }

        static void BindLua(LuaState L) {
            LuaBinding(L).beginExtendClass<phColliderJointed, phCollider>("phColliderJointed")
                .endClass();
        }
    };
}