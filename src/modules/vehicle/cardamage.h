#pragma once
#include <mm2_common.h>
#include <modules\node\node.h>
#include <modules\effects\birth.h>
#include <modules\effects\ptx.h>
#include <modules\effects\shard.h>
#include <modules\effects\linespark.h>
#include <modules\data\callback.h>
#include <modules\vehicle\car.h>

namespace MM2
{
    // Forward declarations
    struct vehDamageImpactInfo;
    class vehCarDamage;

    // External declarations
    extern class asBirthRule;
    extern class asParticles;
    extern class fxShardManager;
    extern class asLineSparks;
    extern class phCollider;

    // Class definitions
    struct vehDamageImpactInfo
    {
        phCollider* OtherCollider;
        Vector3 LocalPosition;
        Vector3 WorldPosition;
        Vector3 Normal;
        Vector3 Impulse; //unconfirmed
        float Damage;
        float TotalDamage;
        float RelaxTime;

        static void BindLua(LuaState L) {
            LuaBinding(L).beginClass<vehDamageImpactInfo>("vehDamageImpactInfo")
                .addVariable("LocalPosition", &vehDamageImpactInfo::LocalPosition, false)
                .addVariable("WorldPosition", &vehDamageImpactInfo::WorldPosition, false)
                .addVariable("Normal", &vehDamageImpactInfo::Normal, false)
                .addVariable("Impulse", &vehDamageImpactInfo::Impulse, false)
                .addVariable("Damage", &vehDamageImpactInfo::Damage, false)
                .addVariable("TotalDamage", &vehDamageImpactInfo::TotalDamage, false)
                .addVariable("RelaxTime", &vehDamageImpactInfo::RelaxTime, false)
                .addVariableRef("OtherCollider", &vehDamageImpactInfo::OtherCollider, false)
                .endClass();
        }
    };

    class vehCarDamage : public asNode {
    private:
        datCallback* ImpactCB;
        datCallback* GameCallback;
        vehCar* vehCarPtr;
        bool DoublePivot;
        bool MirrorPivot;
        byte pad[2];
        int m_CurrentPivot;
        byte m_HasParticleTexture;
        bool EnableDamage;
        byte pad0[2];
        float CurrentDamage;
        float MaxDamage;
        float MedDamage;
        float ImpactThreshold;
        float RegenerateRate;
        vehDamageImpactInfo ImpactsTable[12];
        float unk_344;
        Vector3 SmokeOffset;
        Vector3 SmokeOffset2;
        asParticles* Particles;
        byte ExhaustFlags;
        byte pad1[3];
        Vector3 Exhaust2Pos;
        Vector3 Exhaust1Pos;
        float SparkMultiplier;
        asLineSparks* Sparks;
        float TextelDamageRadius;
        fxShardManager* ShardMgr;
        float unk_390;
        bool m_TexelDamageFlag;
        byte pad2[3];
        Vector3 LastImpactPos;
    private:
        void setGameCallbackLua(LuaRef fn)
        {
            fn.checkFunction();
            fn.pushToStack();
            int m_ref = luaL_ref(fn.state(), LUA_REGISTRYINDEX);

            delete GameCallback;

            GameCallback = new datCallback([this, m_ref](void* param) {
                auto state = MM2Lua::GetState();
                state->getRef(m_ref);

                auto ref = state->popValue<LuaRef>();
                if (ref.isValid() && ref.isFunction())
                    ref.call(this, (vehDamageImpactInfo*)param);
            });
        }
    public:
        static hook::Type<asBirthRule*> EngineSmokeRule;
    public:
        AGE_API vehCarDamage()                              { hook::Thunk<0x4CA380>::Call<void>(this); }
        AGE_API ~vehCarDamage()                             { hook::Thunk<0x4CA530>::Call<void>(this); }

        AGE_API void AddDamage(float a1)                    { hook::Thunk<0x4CAEC0>::Call<void>(this, a1); }
        AGE_API void ClearDamage()                          { hook::Thunk<0x4CAE80>::Call<void>(this); }

        /*
            vehCarDamage virtuals
        */

        virtual AGE_API float GetDamageModifier(phCollider* a1) 
                                                            { return hook::Thunk<0x4CB650>::Call<float>(this, a1); }
        
        /*
            asNode virtuals
        */

        AGE_API void Update() override                      { hook::Thunk<0x5B2C30>::Call<void>(this); }
        AGE_API void Reset() override                       { hook::Thunk<0x4CAE60>::Call<void>(this); }
        AGE_API void FileIO(datParser &parser) override
                                                            { hook::Thunk<0x4CB400>::Call<void>(this); }
        AGE_API char* GetClassName() override               { return hook::Thunk<0x4CB640>::Call<char*>(this); }
        AGE_API char const* GetDirName() override           { return hook::Thunk<0x4CA5F0>::Call<char const*>(this); }
        
        /*
            vehCarDamage
        */
        float GetCurDamage() const {
            return this->CurrentDamage;
        }

        float GetMedDamage() const {
            return this->MedDamage;
        }

        float GetMaxDamage() {
            return this->MaxDamage;
        }

        float GetImpactThreshold() const {
            return this->ImpactThreshold;
        }

        void SetImpactThreshold(float threshold) {
            this->ImpactThreshold = threshold;
        }

        vehCar* GetCar() const {
            return this->vehCarPtr;
        }

        asParticles* GetEngineSmokePtx() const {
            return this->Particles;
        }

        asLineSparks* GetSparkomatic() const {
            return this->Sparks;
        }

        static void BindLua(LuaState L) {
            LuaBinding(L).beginExtendClass<vehCarDamage, asNode>("vehCarDamage")
                .addFunction("Reset", &Reset)
                .addFunction("AddDamage", &AddDamage)
                .addFunction("ClearDamage", &ClearDamage)
                .addFunction("SetGameCallback", &setGameCallbackLua)

                .addVariable("DamageAmount", &vehCarDamage::CurrentDamage)

                .addVariable("Enabled", &vehCarDamage::EnableDamage)
                .addVariable("MedDamage", &vehCarDamage::MedDamage)
                .addVariable("MaxDamage", &vehCarDamage::MaxDamage)
                .addVariable("ImpactThreshold", &vehCarDamage::ImpactThreshold)
                .addVariable("RegenerateRate", &vehCarDamage::RegenerateRate)
                .addVariable("SmokeOffset", &vehCarDamage::SmokeOffset)
                .addVariable("SmokeOffset2", &vehCarDamage::SmokeOffset2)
                .addVariable("TextelDamageRadius", &vehCarDamage::TextelDamageRadius)
                .addVariable("DoublePivot", &vehCarDamage::DoublePivot)
                .addVariable("MirrorPivot", &vehCarDamage::MirrorPivot)

                .addPropertyReadOnly("Car", &GetCar)
                .addPropertyReadOnly("EngineSmokeParticles", &GetEngineSmokePtx)
                .addPropertyReadOnly("Sparks", &GetSparkomatic)
                .addPropertyReadOnly("Sparkomatic", &GetSparkomatic)
                .addStaticProperty("EngineSmokeRule", [] { return EngineSmokeRule.get(); })
            .endClass();
        }
    };

    ASSERT_SIZEOF(vehCarDamage, 0x3A4);

    // Lua initialization

}