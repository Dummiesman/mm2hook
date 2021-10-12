#pragma once
#include <modules\level\inst.h>
#include <modules\banger\data.h>

namespace MM2
{
    // Forward declarations
    class dgBangerInstance;
    class dgUnhitBangerInstance;

    // External declarations
    extern class lvlInstance;

    // Class definitions

    //TODO : find out inheritance / virtual stuff!!!
    class dgBangerInstance : public lvlInstance {
    public:
        /*
            dgBangerInstance
        */
        dgBangerData* GetData()             { return hook::Thunk<0x441AB0>::Call<dgBangerData*>(this); }

        /*
            lvlInstance virtuals
        */
        virtual AGE_API void SetVariant(int a1)       override     { hook::Thunk<0x442A10>::Call<void>(this); }
        virtual AGE_API dgPhysEntity* GetEntity()     override     { return hook::Thunk<0x441AD0>::Call<dgPhysEntity*>(this); }
        virtual AGE_API dgPhysEntity* AttachEntity()  override     { return hook::Thunk<0x441AE0>::Call<dgPhysEntity*>(this); }
        virtual AGE_API Vector3 const * GetVelocity() override     { return hook::Thunk<0x441B20>::Call<Vector3 const *>(this); }
        virtual AGE_API void Draw(int a1)             override     { hook::Thunk<0x4415E0>::Call<void>(this, a1); }
        virtual AGE_API void DrawShadow()             override     { hook::Thunk<0x441990>::Call<void>(this); }
        virtual AGE_API void DrawShadowMap()          override     { hook::Thunk<0x4419A0>::Call<void>(this); }
        virtual AGE_API void DrawGlow()               override     { hook::Thunk<0x441840>::Call<void>(this); }
        virtual AGE_API void DrawReflected(float a1)  override     { hook::Thunk<0x4417B0>::Call<void>(this, a1); }
        virtual AGE_API phBound* GetBound(int a1)     override     { return hook::Thunk<0x441AD0>::Call<phBound*>(this, a1); }
                                                     
        static void BindLua(LuaState L) {
            LuaBinding(L).beginExtendClass<dgBangerInstance, lvlInstance>("dgBangerInstance")
                //functions
                .addFunction("GetData", &GetData)
                .endClass();
        }
    };

    class dgUnhitBangerInstance : public dgBangerInstance {
    public:

        /*
            dgUnhitBangerInstance
        */
        AGE_API static dgUnhitBangerInstance* RequestBanger(const char* a1, BOOL a2)
                                                                   { return hook::StaticThunk<0x441D80>::Call<dgUnhitBangerInstance*>(a1, a2); }

        /*
            lvlInstance virtuals
        */
        virtual AGE_API int Init(const char *a1, const Matrix34 &a2, int a3) override
                                                                   { return hook::Thunk<0x441C30>::Call<int>(this, a1, &a2, a3); }
        virtual AGE_API void Reset() override                      { hook::Thunk<0x441EE0>::Call<void>(this); }
    };

    // Lua initialization

}