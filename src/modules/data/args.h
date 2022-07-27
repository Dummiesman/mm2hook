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
        static int getIntLua(LPCSTR arg, UINT index, int defaultValue);
        static float getFloatLua(LPCSTR arg, UINT index, float defaultValue);
        static LPCSTR getStringLua(LPCSTR arg, UINT index, LPCSTR defaultValue);
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