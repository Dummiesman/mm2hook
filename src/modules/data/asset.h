#pragma once
#include <tuple>
#include <vector>
#include <string>

#include <modules\data.h>

namespace MM2
{
    // Forward declarations
    class datAssetManager;
    class datAssetManagerLuaEnumerator;

    // External declarations
    extern class Stream;

    // Class definitions
    class datAssetManager {
    private:
        static int enumerateLua(lua_State* L, LPCSTR path, bool useCore)
        {
            return CppFunctor::make<datAssetManagerLuaEnumerator>(L, path, useCore);
        }
    public:
        static hook::Type<char *> sm_Path;

        AGE_API static Stream * Open(LPCSTR directory, LPCSTR filename, bool foo, bool readonly)
                                                                        { return hook::StaticThunk<0x4C5870>::Call<Stream *>(directory, filename, foo, readonly); }
        AGE_API static Stream * Open(LPCSTR directory, LPCSTR filename, LPCSTR extension, bool foo, bool readonly)
                                                                        { return hook::StaticThunk<0x4C58C0>::Call<Stream *>(directory, filename, extension, foo, readonly); }

        AGE_API static void FullPath(char *buffer, int length, LPCSTR directory, LPCSTR filename)
                                                                        { hook::StaticThunk<0x4C55E0>::Call<void>(buffer, length, directory, filename); }

        AGE_API static void FullPath(char *buffer, int length, LPCSTR directory, LPCSTR filename, LPCSTR extension) 
                                                                        { hook::StaticThunk<0x4C56F0>::Call<void>(buffer, length, directory, filename, extension); }
        

        // these don't work for files outside of archives
        AGE_API static bool Exists(LPCSTR directory, LPCSTR filename) 
                                                                        { return hook::StaticThunk<0x4C59B0>::Call<bool>(directory, filename); }
        
        AGE_API static bool Exists(LPCSTR directory, LPCSTR filename, LPCSTR extension) 
                                                                        { return hook::StaticThunk<0x4C59E0>::Call<bool>(directory, filename, extension); }
        
        AGE_API static int Enumerate(LPCSTR path, void(* callback)(LPCSTR const, bool, void*), void* this_pointer, bool useCore)
                                                                        { return hook::StaticThunk<0x4C5A80>::Call<int>(path, callback, this_pointer, useCore); }

        //lua
        static void BindLua(LuaState L) {
            LuaBinding(L).beginClass<datAssetManager>("datAssetManager")
                .addStaticFunction("Open", static_cast<Stream* (*)(LPCSTR, LPCSTR, LPCSTR, bool, bool)>(&datAssetManager::Open))
                .addStaticFunction("Exists", static_cast<bool (*)(LPCSTR, LPCSTR, LPCSTR)>(&datAssetManager::Exists))
                .addStaticFunction("Enumerate", &enumerateLua)
                .endClass();
        }
    };

    class datAssetManagerLuaEnumerator : public CppFunctor
    {
    private:
        int enumerateProgress = 0;
        int enumerateResultsCount = 0;
        std::vector<std::tuple<std::string, bool>> enumerateResults;
    private:
        void EnumerateCallbackInstance(LPCSTR path, bool isDirectory)
        {
            enumerateResults.push_back(std::make_tuple(std::string(path), isDirectory));
        }

        static void EnumerateCallback(LPCSTR path, bool isDirectory, void* this_pointer)
        {
            reinterpret_cast<datAssetManagerLuaEnumerator*>(this_pointer)->EnumerateCallbackInstance(path, isDirectory);
        }
    public:
        datAssetManagerLuaEnumerator(LPCSTR path, bool useCore)
        {
            this->enumerateResultsCount = datAssetManager::Enumerate(path, &datAssetManagerLuaEnumerator::EnumerateCallback, this, useCore);
        }

        virtual ~datAssetManagerLuaEnumerator()
        {

        }

        virtual int run(lua_State* L) override
        {
            if (enumerateProgress >= enumerateResultsCount)
                return 0;

            auto tup = enumerateResults[enumerateProgress];
            auto path = std::get<0>(tup);
            auto isDir = std::get<1>(tup);

            LuaState(L).push(path);
            LuaState(L).push(isDir);

            enumerateProgress++;

            return 2;
        }
    };
}