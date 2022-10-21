#pragma once
#include <mm2_common.h>

namespace MM2
{
    // Forward declarations
    class parCsvFile;

    // External declarations


    // Class definitions
    class parCsvFile
    {
    private:
        char* ColumnNames[16];
        char** Data;
        int ColumnCount;
        int RowCount;
    public:
        bool Load(char const* directory, char const* file, BOOL unk1, int unk2)
                                                                { return hook::Thunk<0x4A82F0>::Call<bool>(this, directory, file, unk1, unk2); }
        void Print() const                                      { hook::Thunk<0x4A8600>::Call<void>(this); }
        int GetInt(int a1, int a2)                              { return hook::Thunk<0x4A86A0>::Call<int>(this, a1, a2); }
        float GetFloat(int a1, int a2)                          { return hook::Thunk<0x4A86D0>::Call<float>(this, a1, a2); }

        int GetColumnCount() const {
            return this->ColumnCount;
        }

        int GetRowCount() const {
            return this->RowCount;
        }

        static void BindLua(LuaState L) {
            LuaBinding(L).beginClass<parCsvFile>("parCsvFile")
                .addConstructor(LUA_ARGS())
                .addPropertyReadOnly("ColumnCount", &GetColumnCount)
                .addPropertyReadOnly("RowCount", &GetRowCount)
                .addFunction("GetInt", &GetInt)
                .addFunction("GetFloat", &GetFloat)
                //.addFunction("Load")
                .addFunction("Print", &Print)
                .endClass();
        }
    };

    ASSERT_SIZEOF(parCsvFile, 0x4C);
}