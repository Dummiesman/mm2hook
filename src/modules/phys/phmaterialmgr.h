#pragma once
#include <mm2_common.h>

namespace MM2
{
    // Forward declarations
    class phMaterialMgr;

    // External declarations
    extern class phMaterial;
    extern class datAsciiTokenizer;
    extern class Stream;
    extern class HashTable;

    // Class definitions
    class phMaterialMgr {
    private:
        HashTable* m_Table;
        phMaterial** m_Materials;
        int m_IndexedNameCount;
        int m_NumMaterials;
        int m_MaxNumMaterials;
        char* m_IndexedMaterialNames;
    public:
        /*
            phMaterialMgr Virtuals
        */
        virtual AGE_API phMaterial* Load(Stream* stream)        { return hook::Thunk<0x493780>::Call<phMaterial*>(this, stream); }
        virtual AGE_API phMaterial* Load(datAsciiTokenizer* tokenizer)
                                                                { return hook::Thunk<0x493700>::Call<phMaterial*>(this, tokenizer); }

        /*
            phMaterialMgr
        */
        AGE_API void AddToTable(phMaterial* material)           { hook::Thunk<0x493660>::Call<void>(this, material); }
        AGE_API phMaterial* Find(LPCSTR name) const             { return hook::Thunk<0x493640>::Call<phMaterial*>(this, name); }
        AGE_API int FindIndexOfName(LPCSTR name)                { return hook::Thunk<0x493870>::Call<int>(this, name); }
        AGE_API LPCSTR FindNameOfIndex(int index) const         { return hook::Thunk<0x493840>::Call<LPCSTR>(this, index); }

        AGE_API int GetNumMaterials() const
        {
            return m_NumMaterials;
        }

        AGE_API phMaterial* GetMaterial(int index) const
        {
            if (index < 0 || index >= m_NumMaterials)
                return nullptr;
            return m_Materials[index];
        }

        static void BindLua(LuaState L) {
            LuaBinding(L).beginClass<phMaterialMgr>("phMaterialMgr")
                //properties
                .addPropertyReadOnly("NumMaterials", &GetNumMaterials)
                .addFunction("GetMaterial", &GetMaterial)
                .addFunction("AddToTable", &AddToTable)
                .addFunction("Find", &Find)
                .addFunction("FindIndexOfName", &FindIndexOfName)
                .addFunction("FindNameOfIndex", &FindNameOfIndex)
                .endClass();
        }
    };

    ASSERT_SIZEOF(phMaterialMgr, 0x1C);
}