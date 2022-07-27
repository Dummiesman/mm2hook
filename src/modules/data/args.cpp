#pragma once
#include "args.h"

namespace MM2
{
    declfield(datArgParser::Argc)(0x6A3C28);
    declfield(datArgParser::Argv)(0x6A3C30);

    template<class outType>
    std::tuple<bool, outType> datArgParser::getTyped(LPCSTR arg, UINT index, bool(*func)(LPCSTR, UINT, outType*))
    {
        outType out;
        bool res = func(arg, index, &out);
        return std::make_tuple<bool, outType>(std::move(res), std::move(out));
    }

    std::tuple<bool, int> datArgParser::getInt(LPCSTR arg, int index)
    {
        return getTyped<int>(arg, index, Get);
    }

    std::tuple<bool, float> datArgParser::getFloat(LPCSTR arg, int index)
    {
        return getTyped<float>(arg, index, Get);
    }

    std::tuple<bool, LPCSTR> datArgParser::getString(LPCSTR arg, int index)
    {
        return getTyped<LPCSTR>(arg, index, Get);
    }

    AGE_API bool datArgParser::Get(LPCSTR arg)                          { return hook::StaticThunk<0x4C6190>::Call<bool>(arg); }
    AGE_API bool datArgParser::Get(LPCSTR arg, UINT index, int *out)    { return hook::StaticThunk<0x4C61C0>::Call<bool>(arg, index, out); }
    AGE_API bool datArgParser::Get(LPCSTR arg, UINT index, float *out)  { return hook::StaticThunk<0x4C6210>::Call<bool>(arg, index, out); }
    AGE_API bool datArgParser::Get(LPCSTR arg, UINT index, LPCSTR *out) { return hook::StaticThunk<0x4C6260>::Call<bool>(arg, index, out); }
    AGE_API int datArgParser::GetNum(LPCSTR arg)                        { return hook::StaticThunk<0x4C62A0>::Call<int>(arg); }

    void datArgParser::BindLua(LuaState L) {
        LuaBinding(L).beginClass<datArgParser>("datArgParser")
            .addStaticFunction("Get", static_cast<bool (*)(LPCSTR)>(&Get))
            .addStaticFunction("GetNum", &GetNum)
            .addStaticFunction("GetInt", &getInt, LUA_ARGS(LPCSTR, _def<int, 0>))
            .addStaticFunction("GetFloat", &getFloat, LUA_ARGS(LPCSTR, _def<int, 0>))
            .addStaticFunction("GetString", &getString, LUA_ARGS(LPCSTR, _def<int, 0>))
            .endClass();
    }
}