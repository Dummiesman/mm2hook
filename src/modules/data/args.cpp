#pragma once
#include "args.h"

namespace MM2
{
    declfield(datArgParser::Argc)(0x6A3C28);
    declfield(datArgParser::Argv)(0x6A3C30);

    int datArgParser::getIntLua(LPCSTR arg, UINT index, int defaultValue) {
        int out;
        return datArgParser::Get(arg, index, &out) ? out : defaultValue;
    }

    float datArgParser::getFloatLua(LPCSTR arg, UINT index, float defaultValue) {
        float out;
        return datArgParser::Get(arg, index, &out) ? out : defaultValue;
    }

    LPCSTR datArgParser::getStringLua(LPCSTR arg, UINT index, LPCSTR defaultValue) {
        LPCSTR out;
        return datArgParser::Get(arg, index, &out) ? out : defaultValue;
    }

    AGE_API int datArgParser::SaveToArchive(char* a1)
    {
        return hook::StaticThunk<0x4C5EC0>::Call<int>(a1);
    }

    AGE_API void datArgParser::RestoreFromArchive(char* a1)
    {
        hook::StaticThunk<0x4C5D10>::Call<void>(a1);
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
            .addStaticFunction("GetInt", &getIntLua)
            .addStaticFunction("GetFloat", &getFloatLua)
            .addStaticFunction("GetString", &getStringLua)
            .endClass();
    }
}