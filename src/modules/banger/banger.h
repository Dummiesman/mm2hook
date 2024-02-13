#pragma once
#include <modules\level\inst.h>
#include <modules\banger\data.h>

namespace MM2
{
    // Forward declarations
    class dgBangerInstance;

    class dgUnhitBangerInstance;
    class dgHitBangerInstance;

    class dgUnhitYBangerInstance;
    class dgUnhitMtxBangerInstance;

    class dgBangerManager;

    // External declarations
    extern class lvlInstance;

    // Class definitions
    
    /*
        Base Classes
    */

    // Note
    // The 16 byte gaps are most likely in the correct position
    // as suggested by dgUnhitBangerInstance::SizeOf in TDOR

    class dgBangerInstance : public lvlInstance {
    private:
        short DataPack; // Contains type and variant
    public:
        /*
            dgBangerInstance
        */
        inline dgBangerData* GetData() const                       { return hook::Thunk<0x441AB0>::Call<dgBangerData*>(this); }

        int GetVariant() const 
        {
            return this->DataPack >> 12;
        }

        void SetBangerType(unsigned short type)
        {
            this->DataPack ^= (this->DataPack ^ type) & 0xFFF;
        }

        unsigned short GetBangerType() const
        {
            return this->DataPack & 0xFFF;
        }

        /*
            lvlInstance virtuals
        */
        virtual AGE_API void SetVariant(int a1)       override     { hook::Thunk<0x442A10>::Call<void>(this); }
        virtual AGE_API dgPhysEntity* GetEntity()     override     { return hook::Thunk<0x441AD0>::Call<dgPhysEntity*>(this); }
        virtual AGE_API dgPhysEntity* AttachEntity()  override     { return hook::Thunk<0x441AE0>::Call<dgPhysEntity*>(this); }
        virtual AGE_API Vector3 const & GetVelocity() override     { return hook::Thunk<0x441B20>::Call<Vector3 const &>(this); }
        virtual AGE_API void Draw(int a1)             override     { hook::Thunk<0x4415E0>::Call<void>(this, a1); }
        virtual AGE_API void DrawShadow()             override     { hook::Thunk<0x441990>::Call<void>(this); }
        virtual AGE_API void DrawShadowMap()          override     { hook::Thunk<0x4419A0>::Call<void>(this); }
        virtual AGE_API void DrawGlow()               override     { hook::Thunk<0x441840>::Call<void>(this); }
        virtual AGE_API void DrawReflected(float a1)  override     { hook::Thunk<0x4417B0>::Call<void>(this, a1); }
        virtual AGE_API phBound* GetBound(int a1)     override     { return hook::Thunk<0x442580>::Call<phBound*>(this, a1); }

        /* 
            dgBangerInstance virtuals
        */
        virtual AGE_API int ComputeLod(float a1, float a2)         { return hook::Thunk<0x4415C0>::Call<int>(this, a1, a2); }
        virtual AGE_API int SetupGfxLights(Matrix34 const & a1) const
                                                                   { return hook::Thunk<0x442910>::Call<int>(&a1); }
        virtual AGE_API void Impact(lvlInstance* a1, Vector3* a2)  PURE;
        virtual AGE_API void ImpactCB(dgHitBangerInstance * a1)    PURE;

                                                     
        static void BindLua(LuaState L) {
            LuaBinding(L).beginExtendClass<dgBangerInstance, lvlInstance>("dgBangerInstance")
                //functions
                .addFunction("GetData", &GetData)
                .addProperty("BangerType", &GetBangerType, &SetBangerType)
                .endClass();
        }
    };

    class dgUnhitBangerInstance : public dgBangerInstance {
    private:
        static dgUnhitBangerInstance* requestBangerLua(LPCSTR name, bool fullMatrix)
        {
            return dgUnhitBangerInstance::RequestBanger(name, fullMatrix ? TRUE : FALSE);
        }
    public:
        AGE_API dgUnhitBangerInstance()
        {
            scoped_vtable x(this);
            hook::Thunk<0x541880>::Call<void>(this);
        }

        AGE_API ~dgUnhitBangerInstance()
        {
            scoped_vtable x(this);
            hook::Thunk<0x5418B0>::Call<void>(this);
        }

        /*
            dgUnhitBangerInstance
        */
        AGE_API static dgUnhitBangerInstance* RequestBanger(const char* name, BOOL fullMatrix)
                                                                   { return hook::StaticThunk<0x441D80>::Call<dgUnhitBangerInstance*>(name, fullMatrix); }

        /*
            lvlInstance virtuals
        */
        virtual AGE_API int Init(const char *a1, const Matrix34 &a2, int a3) override
                                                                   { return hook::Thunk<0x441C30>::Call<int>(this, a1, &a2, a3); }
        virtual AGE_API void Reset() override                      { hook::Thunk<0x441EE0>::Call<void>(this); }

        /*
            dgBangerInstance virtuals
        */
        virtual AGE_API void Impact(lvlInstance* a1, Vector3* a2)  { hook::Thunk<0x442010>::Call<void>(this, a1, a2); };
        virtual AGE_API void ImpactCB(dgHitBangerInstance* a1)     { hook::Thunk<0x442AD0>::Call<void>(this, a1); };

        static void BindLua(LuaState L) {
            LuaBinding(L).beginExtendClass<dgUnhitBangerInstance, dgBangerInstance>("dgUnhitBangerInstance")
                .addStaticFunction("RequestBanger", &requestBangerLua)
                .endClass();
        }
    };

    class dgHitBangerInstance : public dgBangerInstance {
    private:
        char gap[0x10]; // unknown use
        Matrix34 Matrix;
    public:
        AGE_API dgHitBangerInstance()
        {
            scoped_vtable x(this);
            hook::Thunk<0x442A60>::Call<void>(this);
        }

        AGE_API ~dgHitBangerInstance()
        {
            scoped_vtable x(this);
            hook::Thunk<0x4429B0>::Call<void>(this);
        }

        /*
            lvlInstance virtuals
        */
        virtual AGE_API const Vector3& GetPosition() override      { return hook::Thunk<0x441B70>::Call<const Vector3&>(this); }
        virtual AGE_API const Matrix34& GetMatrix(Matrix34* a1)    { return hook::Thunk<0x441B60>::Call<const Matrix34&>(this, a1); };
        virtual AGE_API void SetMatrix(const Matrix34& a1) override
                                                                   { hook::Thunk<0x441B40>::Call<void>(this, a1); }
        virtual AGE_API unsigned int SizeOf(void)                  { return hook::Thunk<0x442AB0>::Call<unsigned int>(this); }
        virtual AGE_API void Detach() override                     { return hook::Thunk<0x442680>::Call<void>(this); }

        static void BindLua(LuaState L) {
            LuaBinding(L).beginExtendClass<dgHitBangerInstance, dgBangerInstance>("dgHitBangerInstance")
                .endClass();
        }

    };
    ASSERT_SIZEOF(dgHitBangerInstance, 0x58);


    /*
        Derived
    */
    class dgUnhitYBangerInstance : public dgUnhitBangerInstance {
    private:
        char gap[0x10]; // unknown use
        Vector3 Position;
        float XAxis;
        float ZAxis;
    public:
        /*
            lvlInstance virtuals
        */
        virtual AGE_API const Vector3& GetPosition() override      { return hook::Thunk<0x441FC0>::Call<const Vector3&>(this); }
        virtual AGE_API const Matrix34& GetMatrix(Matrix34& a1)    { return hook::Thunk<0x441F70>::Call<const Matrix34&>(this, &a1); };
        virtual AGE_API void SetMatrix(const Matrix34& a1) override
                                                                   { hook::Thunk<0x441F40>::Call<void>(this, &a1); }
        virtual AGE_API unsigned int SizeOf(void)                  { return hook::Thunk<0x442AE0>::Call<unsigned int>(this); }

        static void BindLua(LuaState L) {
            LuaBinding(L).beginExtendClass<dgUnhitYBangerInstance, dgUnhitBangerInstance>("dgUnhitYBangerInstance")
                .addFactory([]() {
                return new dgUnhitYBangerInstance();
                })
                .endClass();
        }
    };
    ASSERT_SIZEOF(dgUnhitYBangerInstance, 0x3C);


    class dgUnhitMtxBangerInstance : public dgUnhitBangerInstance {
    private:
        char gap[0x10]; // unknown use
        Matrix34 Matrix;
    public:
        /*
            lvlInstance virtuals
        */
        virtual AGE_API const Vector3& GetPosition() override { return hook::Thunk<0x442000>::Call<const Vector3&>(this); }
        virtual AGE_API const Matrix34& GetMatrix(Matrix34& a1) { return hook::Thunk<0x441FF0>::Call<const Matrix34&>(this, &a1); };
        virtual AGE_API void SetMatrix(const Matrix34& a1) override
        {
            hook::Thunk<0x441FD0>::Call<void>(this, a1);
        }
        virtual AGE_API unsigned int SizeOf(void) { return hook::Thunk<0x442AF0>::Call<unsigned int>(this); }

        static void BindLua(LuaState L) {
            LuaBinding(L).beginExtendClass<dgUnhitMtxBangerInstance, dgUnhitBangerInstance>("dgUnhitMtxBangerInstance")
                .addFactory([]() {
                return new dgUnhitMtxBangerInstance();
                })
                .endClass();
        }
    };
    ASSERT_SIZEOF(dgUnhitMtxBangerInstance, 0x58);

    /* 
        Manager
    */
    class dgBangerManager : public asNode {
    private:
        dgHitBangerInstance* HitBangerPool;
        int CurPoolIndex;
        int PoolSize;
    protected:
        static hook::Type<dgBangerManager*> Instance;
    public:
        inline static dgBangerManager * GetInstance()
        {
            return Instance.get();
        }

        /*
            asNode virtuals
        */
        virtual AGE_API void Reset() override                      { hook::Thunk<0x4428A0>::Call<void>(this); }

        /*
            dgBangerManager
        */
        AGE_API dgHitBangerInstance * GetBanger()                  { return hook::Thunk<0x442780>::Call<dgHitBangerInstance*>(this); }
        AGE_API void Init(int poolSize)                            { hook::Thunk<0x4427E0>::Call<void>(this, poolSize); }

        static void BindLua(LuaState L) {
            LuaBinding(L).beginExtendClass<dgBangerManager, asNode>("dgBangerManager")
                .addStaticProperty("Instance", &dgBangerManager::GetInstance)
                .addFunction("GetBanger", &GetBanger)
                .endClass();
        }
    };
    ASSERT_SIZEOF(dgBangerManager, 0x24);
}