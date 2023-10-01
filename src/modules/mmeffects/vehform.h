#pragma once
#include <mm2_common.h>
#include <modules\node\node.h>

namespace MM2
{
    // Forward declarations
    struct mmDamage;
    class asMeshSetForm;
    
    // External declarations
    extern class modStatic;
    extern class modShader;
    
    // Class definitions
    struct mmDamage {
    public:
        modStatic* model;
        void* unused;
    };

    class mmVehicleForm : public asNode {
    private:
        int BodyModelPtr;
        int ShadowModelPtr;
        modStatic* WheelModels;
        modShader** pShaders;
        Vector3** WheelPositions;
        int MiscModelCount;
        modStatic** MiscModels;
        Vector3* MiscModelPositions;
        int* Variant;
        int* pVariant;
        int ShadingColor;
        int ShadersPerVariant;
        mmDamage Damage;
        int dword_50;
        int dword_54;
        int dword_58;
        int dword_5c;
        int dword_60;
        int dword_64;
    public:
        Matrix34 GetMatrix();
        void SetMatrix(Matrix34 matrix);
        int GetVariant();
        void SetVariant(int variant);
        int GetVariantCount();
        Vector3 GetPosition();
        void SetPosition(Vector3 position);
    public:
        AGE_API mmVehicleForm();
        AGE_API ~mmVehicleForm();

        /*
            asNode virtuals
        */
        virtual AGE_API void Cull() override;
        virtual AGE_API void Update() override;
    
        AGE_API void SetShape(LPCSTR basename, LPCSTR lodName, LPCSTR shadowModelName, Vector3* unknown);

        static void BindLua(LuaState L);
    };

    // Lua initialization

}