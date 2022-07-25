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
        std::tuple<bool, outType> getTyped(LPCSTR arg, UINT index, bool(*func)(LPCSTR, UINT, outType*))
        {
            outType out;
            bool res = func(arg, index, &out);
            return std::make_tuple<bool, outType>(std::move(res), std::move(out));
        }

        std::tuple<bool, int> getInt(LPCSTR arg, UINT index = 0)
        {
            return getTyped<int>(arg, index, Get);
        }

        std::tuple<bool, float> getFloat(LPCSTR arg, UINT index = 0)
        {
            return getTyped<float>(arg, index, Get);
        }

        std::tuple<bool, LPCSTR> getString(LPCSTR arg, UINT index = 0)
        {
            return getTyped<LPCSTR>(arg, index, Get);
        }
    public:
        static hook::Type<int> Argc;
        static hook::Type<char **> Argv;

        AGE_API static bool Get(LPCSTR arg)                          { return hook::StaticThunk<0x4C6190>::Call<bool>(arg); }
        AGE_API static bool Get(LPCSTR arg, UINT index, int *out)    { return hook::StaticThunk<0x4C61C0>::Call<bool>(arg, index, out); }
        AGE_API static bool Get(LPCSTR arg, UINT index, float *out)  { return hook::StaticThunk<0x4C6210>::Call<bool>(arg, index, out); }
        AGE_API static bool Get(LPCSTR arg, UINT index, LPCSTR *out) { return hook::StaticThunk<0x4C6260>::Call<bool>(arg, index, out); }
        AGE_API static int GetNum(LPCSTR arg)                        { return hook::StaticThunk<0x4C62A0>::Call<int>(arg); }

        static void BindLua(LuaState L) {
            LuaBinding(L).beginClass<datArgParser>("datArgParser")
                .addStaticFunction("Get", static_cast<bool (*)(LPCSTR)>(&Get))
                .addStaticFunction("GetNum", &GetNum)
                //.addStaticFunction("GetInt", &getInt, LUA_ARGS(LPCSTR, _opt<UINT>))
                //.addStaticFunction("GetFloat", &getFloat, LUA_ARGS(LPCSTR, _opt<UINT>))
                //.addStaticFunction("GetString", &getString, LUA_ARGS(LPCSTR, _opt<UINT>))
                .endClass();
        }
    };
}