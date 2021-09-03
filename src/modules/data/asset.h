#pragma once
#include <modules\data.h>

namespace MM2
{
    // Forward declarations
    class datAssetManager;

    // External declarations
    extern class Stream;

    // Class definitions
    class datAssetManager {
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
        

        //lua
        static void BindLua(LuaState L) {
            LuaBinding(L).beginClass<datAssetManager>("datAssetManager")
                .addStaticFunction("Open", static_cast<Stream* (*)(LPCSTR, LPCSTR, LPCSTR, bool, bool)>(&datAssetManager::Open))
                .addStaticFunction("Exists", static_cast<bool (*)(LPCSTR, LPCSTR, LPCSTR)>(&datAssetManager::Exists))
                .endClass();
        }

    // Lua initialization

    };
}