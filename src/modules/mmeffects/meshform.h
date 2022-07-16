#pragma once
#include <mm2_common.h>
#include <modules\node\node.h>

namespace MM2
{
    // Forward declarations
    class asMeshSetForm;

    // External declarations
    extern class modStatic;
    extern class modShader;

    // Class definitions
    class asMeshSetForm : public asNode {
    private:
        modStatic* ModStatic;
        int VariantCount;
        modShader** Shaders;
        modShader* ChosenShaderSet;
        int dword_28;
        int Flags;
        Matrix34 Matrix;
    public:
        Matrix34 GetMatrix();
        void SetMatrix(Matrix34 matrix);
        int GetVariant();
        void SetVariant(int variant);
        int GetVariantCount();
        Vector3 GetPosition();
        void SetPosition(Vector3 position);
    public:
        AGE_API asMeshSetForm();
        AGE_API ~asMeshSetForm();

        /*
            asNode virtuals
        */
        virtual AGE_API void Cull() override;
        virtual AGE_API void Update() override;
    
        AGE_API void SetShape(LPCSTR modelName, LPCSTR dirName, bool useFullVertex = true);
        AGE_API void SetZRead(bool enable);
        AGE_API void SetZWrite(bool enable);
        AGE_API void EnableLighting(bool enable);
        AGE_API void EnableAlpha(bool enable);

        static void BindLua(LuaState L);
    };

    // Lua initialization

}