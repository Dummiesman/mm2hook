#pragma once
#include <modules\phys\phmaterial.h>

namespace MM2
{
    // Forward declarations
    class lvlMaterial;

    // External declarations
    extern class datAsciiTokenizer;
    extern class phMaterial;

    // Class definitions
    class lvlMaterial : public phMaterial {
    private:
        float Drag;
        float Width;
        float Height;
        float Depth;
        short PtxIndex[2];
        float PtxThreshold[2];
    public:
        AGE_API lvlMaterial() {
            scoped_vtable x(this);
            hook::Thunk<0x4664C0>::Call<void>(this);
        }

        /*
            phMaterial Virtuals
        */
        AGE_API void Save(datAsciiTokenizer *writer) override   { hook::Thunk<0x4666F0>::Call<void>(this, writer); }

        /*
            lvlMaterial
        */
        AGE_API void Load(datAsciiTokenizer *reader)            { hook::Thunk<0x466510>::Call<void>(this, reader); }
        AGE_API void Copy(const lvlMaterial &material)          { hook::Thunk<0x4668E0>::Call<void>(this, &material); }

        float GetDrag() const { return this->Drag; }
        void SetDrag(float drag) { this->Drag = drag; }

        float GetWidth() const { return this->Width; }
        void SetWidth(float width) { this->Width = width; }

        float GetHeight() const { return this->Height; }
        void SetHeight(float height) { this->Height = height; }

        float GetDepth() const { return this->Depth; }
        void SetDepth(float depth) { this->Depth = depth; }

        int GetPtxIndex(int num) const {
            if (num < 0 || num > 1)
                return -1;
            return PtxIndex[num];
        }

        void SetPtxIndex(int num, int index) {
            if (num < 0 || num > 1)
                return;
            PtxIndex[num] = (short)index;
        }

        float GetPtxThreshold(int num) const {
            if (num < 0 || num > 1)
                return 0.f;
            return PtxThreshold[num];
        }

        void SetPtxThreshold(int num, float threshold) {
            if (num < 0 || num > 1)
                return;
            PtxThreshold[num] = threshold;
        }

        static void BindLua(LuaState L) {
            LuaBinding(L).beginExtendClass<lvlMaterial, phMaterial>("lvlMaterial")
                //properties
                .addFactory([]() -> lvlMaterial* { return new lvlMaterial(); })

                .addProperty("Drag", &GetDrag, &SetDrag)
                .addProperty("Width", &GetWidth, &SetWidth)
                .addProperty("Height", &GetHeight, &SetHeight)
                .addProperty("Depth", &GetDepth, &SetDepth)

                .addFunction("GetPtxIndex", &GetPtxIndex)
                .addFunction("SetPtxIndex", &SetPtxIndex)

                .addFunction("GetPtxThreshold", &GetPtxThreshold)
                .addFunction("SetPtxThreshold", &SetPtxThreshold)

                .addFunction("Copy", &Copy)

                .endClass();
        }
    };

    ASSERT_SIZEOF(lvlMaterial, 0x4C);
}