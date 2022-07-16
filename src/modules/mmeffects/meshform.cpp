#include "meshform.h"


namespace MM2
{
    AGE_API asMeshSetForm::asMeshSetForm(void) {
        scoped_vtable x(this);
        hook::Thunk<0x533600>::Call<void>(this);
    }

    AGE_API asMeshSetForm::~asMeshSetForm(void) {
        scoped_vtable x(this);
        hook::Thunk<0x5339D0>::Call<void>(this);
    }

    Matrix34 asMeshSetForm::GetMatrix() {
        return this->Matrix;
    }

    void asMeshSetForm::SetMatrix(Matrix34 a1) {
        this->Matrix = a1;
    }

    int asMeshSetForm::GetVariant() {
        if (this->Shaders == nullptr)
            return -1;

        //compare shader sets to our current
        for (int i = 0; i < this->VariantCount; i++) {
            if (this->ChosenShaderSet == this->Shaders[i])
                return i;
        }

        return -1;
    }

    void asMeshSetForm::SetVariant(int variant) {
        if (this->Shaders == nullptr || variant >= this->VariantCount || variant < 0)
            return;
        this->ChosenShaderSet = Shaders[variant];
    }

    int asMeshSetForm::GetVariantCount() {
        return this->VariantCount;
    }

    Vector3 asMeshSetForm::GetPosition() {
        return this->Matrix.GetRow(3);
    }

    void asMeshSetForm::SetPosition(Vector3 position) {
        this->Matrix.SetRow(3, position);
    }


    /*
        asNode virtuals
    */

    AGE_API void asMeshSetForm::Cull() { hook::Thunk<0x533810>::Call<void>(this); };
    AGE_API void asMeshSetForm::Update() { hook::Thunk<0x5337F0>::Call<void>(this); };

    //Last arg is never used, so I've set it to nullptr. It's a Vector3 reference, which was meant for offset I guess.
    AGE_API void asMeshSetForm::SetShape(LPCSTR modelName, LPCSTR dirName, bool useFullVertex)
    {
        hook::Thunk<0x533660>::Call<void>(this, modelName, dirName, useFullVertex, nullptr);
    }
    AGE_API void asMeshSetForm::SetZRead(bool enable) { hook::Thunk<0x533770>::Call<void>(this, enable); }
    AGE_API void asMeshSetForm::SetZWrite(bool enable) { hook::Thunk<0x533790>::Call<void>(this, enable); }
    AGE_API void asMeshSetForm::EnableLighting(bool enable) { hook::Thunk<0x5337B0>::Call<void>(this, enable); }
    AGE_API void asMeshSetForm::EnableAlpha(bool enable) { hook::Thunk<0x5337D0>::Call<void>(this, enable); }

    void asMeshSetForm::BindLua(LuaState L) {
        LuaBinding(L).beginExtendClass<asMeshSetForm, asNode>("asMeshSetForm")
            .addConstructor(LUA_ARGS())
            .addFunction("GetMatrix", &GetMatrix)
            .addFunction("SetMatrix", &SetMatrix)
            .addFunction("GetPosition", &GetPosition)
            .addFunction("SetPosition", &SetPosition)

            .addProperty("Variant", &GetVariant, &SetVariant)
            .addPropertyReadOnly("NumVariants", &GetVariantCount)

            .addFunction("SetShape", &SetShape, LUA_ARGS(LPCSTR, LPCSTR, _opt<bool>))
            .addFunction("SetZRead", &SetZRead)
            .addFunction("SetZWrite", &SetZWrite)
            .addFunction("EnableLighting", &EnableLighting)
            .addFunction("EnableAlpha", &EnableAlpha)
            .endClass();
    }
}