#pragma once
#include <modules\phys\phmaterialmgr.h>

namespace MM2
{
    // Forward declarations
    class lvlMaterialMgr;

    // External declarations
    extern class lvlMaterial;
    extern class phMaterial;

    // Class definitions
    class lvlMaterialMgr : public phMaterialMgr {
    public:
        /*
            phMaterialMgr Virtuals
        */
        virtual AGE_API phMaterial* Load(datAsciiTokenizer* tokenizer) override
                                                                { return hook::Thunk<0x466A50>::Call<phMaterial*>(this, tokenizer); }
        /*
            lvlMaterialMgr
        */
        AGE_API static lvlMaterialMgr& GetInstance()            { return hook::StaticThunk<0x42CCA0>::Call<lvlMaterialMgr&>(); }
        AGE_API lvlMaterial* Find(LPCSTR name) const            { return hook::Thunk<0x466A30>::Call<lvlMaterial*>(this, name); }
        AGE_API lvlMaterial* Lookup(int index) const            { return hook::Thunk<0x466A40>::Call<lvlMaterial*>(this, index); }

        static void BindLua(LuaState L) {
            LuaBinding(L).beginExtendClass<lvlMaterialMgr, phMaterialMgr>("lvlMaterialMgr")
                .addFunction("Find", &Find)
                .addFunction("Lookup", &Lookup)
                .endClass();
        }
    };

    ASSERT_SIZEOF(lvlMaterialMgr, 0x1c);
}