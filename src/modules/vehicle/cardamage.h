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

        static void BindLua(LuaState L);
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
        void setGameCallbackLua(LuaRef fn);
    public:
        static hook::Type<asBirthRule*> EngineSmokeRule;
    public:
        AGE_API vehCarDamage();
        AGE_API ~vehCarDamage();
                                                          
        AGE_API void AddDamage(float a1);
        AGE_API void ClearDamage();

        /*
            vehCarDamage virtuals
        */

        virtual AGE_API float GetDamageModifier(phCollider* a1);
        
        /*
            asNode virtuals
        */

        AGE_API void Update() override;
        AGE_API void Reset() override;
        AGE_API void FileIO(datParser& parser) override;
        AGE_API char* GetClassName() override;
        AGE_API char const* GetDirName() override;
        
        /*
            vehCarDamage
        */
        float GetCurDamage() const;
        float GetMedDamage() const;
        float GetMaxDamage() const;
        float GetImpactThreshold() const;
        void SetImpactThreshold(float threshold);
        vehCar* GetCar() const;
        asParticles* GetEngineSmokePtx() const;
        asLineSparks* GetSparkomatic() const;

        // TEMP DEBUG STUFF
        int GetNumActiveImpacts() const;
        const vehDamageImpactInfo* GetImpact(int id) const;

        static void BindLua(LuaState L);
    };

    ASSERT_SIZEOF(vehCarDamage, 0x3A4);

    // Lua initialization

}