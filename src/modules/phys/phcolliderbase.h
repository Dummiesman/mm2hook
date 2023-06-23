#pragma once
#include <modules\vector.h>

namespace MM2
{
    // Forward declarations
    struct InstanceData;
    class phColliderBase;

    // External declarations
    extern class phBound;
    extern class phSleep;
    extern class datCallback;

    // Class definitions
    struct InstanceData
    {
        Matrix34 Matrix;
        int ID;
    };

    // !STILL UNKNOWN IN SIZE!
    class phColliderBase {
    private:
        int dword_04;
        int dword_08;
        char Active;
        InstanceData* pInstanceData;
        datCallback* ImpactCB;
        datCallback* BoundCB;
        datCallback* HitCB;
        phBound* Bound;
        phInertialCS* ICS;
        Matrix34* SomeMtx;
        Matrix34 LastMatrix;
        char JustReset;
        char PeneBarelyMovedIGuess;
        float SomeAngVelThing;
        phSleep* Sleep;
        float dword_68;
        Matrix34* dword_6c;
        Matrix34* dword_70;
    public:
        
        /*
            phColliderBase virtuals
        */
        virtual void GetInvMassMatrix(Vector3 const& a1, Vector3 const& a2, Matrix34& a3, int a4) const
                                                            { hook::Thunk<0x46EA90>::Call<void>(this, &a1, &a2, &a3, a4); }
        virtual void GetInvMassMatrix(Vector3 const& a1, Matrix34& a2, int a3) const
                                                            { hook::Thunk<0x46EA60>::Call<void>(this, &a1, &a2, a3); }
        virtual void Impact(class phImpactBase const* a1, Vector3 const& a2)
                                                            { hook::Thunk<0x46E1B0>::Call<void>(this, a1, &a2); }
        virtual void Impact(class phImpactBase const* a1, Vector3 const& a2, Vector3 const& a3)
                                                            { hook::Thunk<0x46E070>::Call<void>(this, a1, &a2, &a3); }
        virtual void Contact(Vector3 const& a1, class phContact const* a2)
                                                            { hook::Thunk<0x46E3C0>::Call<void>(this, &a1, a2); }
        virtual void Contact(Vector3 const& a1, class phContact const* a2, Matrix34 const& a3)
                                                            { hook::Thunk<0x46E350>::Call<void>(this, &a1, a2, &a3); }
        virtual void Contact(class phImpactBase const* a1, Vector3 const& a2, Vector3 const& a3, Vector3 const& a4, Matrix34 const& a5)
                                                            { hook::Thunk<0x46E2B0>::Call<void>(this, a1, &a2, &a3, &a4, &a5); }

        /*
            phColliderBase members
        */
        AGE_API void UpdateMtx()                            { hook::Thunk<0x46D9E0>::Call<void>(this); }

        int GetID() const
        {
            return this->pInstanceData->ID;
        }

        phInertialCS* GetICS()
        {
            return this->ICS;
        }

        phBound* GetBound()
        {
            return this->Bound;
        }

        static void BindLua(LuaState L) {
            LuaBinding(L).beginClass<phColliderBase>("phColliderBase")
            .addPropertyReadOnly("ID", &GetID)
            .addFunction("GetICS", &GetICS)
            .addFunction("GetBound", &GetBound)
            .endClass();
        }
    };

    // Lua initialization

}