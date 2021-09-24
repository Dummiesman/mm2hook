#pragma once


namespace MM2
{
    // Forward declarations
    struct gfxLight;

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
        inline Matrix34 getMatrix() {
            return this->Matrix;
        }
            
        inline void setMatrix(Matrix34 a1) {
            this->Matrix = a1;
        }

        inline void setVariant(int variant) {
            if (this->Shaders == nullptr || variant >= this->VariantCount)
                return;
            this->ChosenShaderSet = Shaders[variant];
        }

        inline int getVariant() {
            if (this->Shaders == nullptr)
                return -1;
            
            //compare shader sets to our current
            for (int i = 0; i < this->VariantCount; i++) {
                if (this->ChosenShaderSet == this->Shaders[i])
                    return i;
            }

            return -1;
        }

        inline int getVariantCount() {
            return this->VariantCount;
        }

        inline Vector3 getPosition() {
            return Vector3(this->Matrix.m30, this->Matrix.m31, this->Matrix.m32);
        }

        inline void setPosition(Vector3 position) {
            this->Matrix.m30 = position.X;
            this->Matrix.m31 = position.Y;
            this->Matrix.m32 = position.Z;
        }
    public:
        AGE_API asMeshSetForm(void) {
            scoped_vtable x(this);
            hook::Thunk<0x533600>::Call<void>(this);
        }

        AGE_API ~asMeshSetForm(void) {
            scoped_vtable x(this);
            hook::Thunk<0x5339D0>::Call<void>(this);
        }

        /*
            asNode virtuals
        */

        virtual AGE_API void Cull(void)                         { hook::Thunk<0x533810>::Call<void>(this); };
        virtual AGE_API void Update(void)                       { hook::Thunk<0x5337F0>::Call<void>(this); };
    
        //Last arg is never used, so I've set it to nullptr. It's a Vector3 reference, which was meant for offset I guess.
        AGE_API void SetShape(LPCSTR modelName, LPCSTR dirName, bool useLVertex)
                                                                { hook::Thunk<0x533660>::Call<void>(this, modelName, dirName, useLVertex, nullptr); }
        AGE_API void SetZRead(bool a1)                          { hook::Thunk<0x533770>::Call<void>(this, a1); }
        AGE_API void SetZWrite(bool a1)                         { hook::Thunk<0x533790>::Call<void>(this, a1); }
        AGE_API void EnableLighting(bool a1)                    { hook::Thunk<0x5337B0>::Call<void>(this, a1); }
        AGE_API void EnableAlpha(bool a1)                       { hook::Thunk<0x5337D0>::Call<void>(this, a1); }

        static void BindLua(LuaState L) {
            LuaBinding(L).beginExtendClass<asMeshSetForm, asNode>("asMeshSetForm")
                .addConstructor(LUA_ARGS())
                .addProperty("Matrix", &getMatrix, &setMatrix)
                .addProperty("Position", &getPosition, &setPosition)
                .addProperty("Variant", &getVariant, &setVariant)
                .addPropertyReadOnly("NumVariants", &getVariantCount)

                .addFunction("SetShape", &SetShape)
                .addFunction("SetZRead", &SetZRead)
                .addFunction("SetZWrite", &SetZWrite)
                .addFunction("EnableLighting", &EnableLighting)
                .addFunction("EnableAlpha", &EnableAlpha)
            .endClass();
        }
    };

    // Lua initialization

}