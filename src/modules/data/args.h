#pragma once
#include "mm2_common.h"

namespace MM2
{
    // Forward declarations
    class datArgParser;

    // External declarations


    // Class definitions
    class datArgParser {
    private:
        //lua helpers
        template<class outType>
        static std::tuple<bool, outType> getTyped(LPCSTR arg, UINT index, bool(*func)(LPCSTR, UINT, outType*), outType defaultValue);

        static std::tuple<bool, int> getInt(LPCSTR arg, int index = 0);
        static std::tuple<bool, float> getFloat(LPCSTR arg, int index = 0);
        static std::tuple<bool, LPCSTR> getString(LPCSTR arg, int index = 0);
    public:
        static hook::Type<int> Argc;
        static hook::Type<char **> Argv;

        AGE_API static bool Get(LPCSTR arg);
        AGE_API static bool Get(LPCSTR arg, UINT index, int* out);
        AGE_API static bool Get(LPCSTR arg, UINT index, float* out);
        AGE_API static bool Get(LPCSTR arg, UINT index, LPCSTR* out);
        AGE_API static int GetNum(LPCSTR arg);

        static void BindLua(LuaState L);
    };
}