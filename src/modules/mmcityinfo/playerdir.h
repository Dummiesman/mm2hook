#pragma once
#include "infobase.h"

namespace MM2
{
    // Forward declarations
    class mmPlayerDirectory;

    // External declarations


    // Class definitions
    class mmPlayerDirectory : public mmInfoBase {
    private:
        int PlayerCount;
        char** PlayerNames;
        char** SaveFileNames;
        char LastPlayer[80];
    private:
        bool LoadLua(LPCSTR fileName)
        {
            return this->Load(fileName) == TRUE;
        }
    public:
        AGE_API mmPlayerDirectory(void) {
            scoped_vtable x(this);
            hook::Thunk<0x526520>::Call<void>(this);
        }

        virtual AGE_API ~mmPlayerDirectory(void) {
            scoped_vtable x(this);
            hook::Thunk<0x526560>::Call<void>(this);
        }

        // mmPlayerDirectory members
        int GetNumPlayers() const                                { return hook::Thunk<0x5266C0>::Call<int>(this); }
        int FindPlayer(const char* playerName) const             { return hook::Thunk<0x526930>::Call<int>(this, playerName); }
        const char* GetLastPlayer() const                        { return hook::Thunk<0x5266D0>::Call<const char*>(this); }
        const char* GetPlayerName(const char* fileName) const    { return hook::Thunk<0x526A30 >::Call<const char*>(this, fileName); }
        const char* GetPlayerName(int index) const
        {
            if (index < 0 || index >= GetNumPlayers())
                return nullptr;
            return PlayerNames[index];
        }

        const char* GetFileName(const char* playerName) const    { return hook::Thunk<0x5269A0>::Call<const char*>(this, playerName); }
        const char* GetFileName(int index) const                 { return hook::Thunk<0x526AC0>::Call<const char*>(this, index); }
        BOOL Load(const char* fileName)                          { return hook::Thunk<0x526CA0>::Call<BOOL>(this, fileName); }
        BOOL Save(const char* fileName, BOOL binary)             { return hook::Thunk<0x526CD0>::Call<BOOL>(this, fileName, binary); }

        //lua
        static void BindLua(LuaState L) {
            LuaBinding(L).beginExtendClass<mmPlayerDirectory, mmInfoBase>("mmPlayerDirectory")
                .addConstructor(LUA_ARGS())
                .addPropertyReadOnly("NumPlayers", &GetNumPlayers)
                .addFunction("Load", &LoadLua)
                .addFunction("FindPlayer", &FindPlayer)
                .addFunction("GetLastPlayer", &GetLastPlayer)
                .addFunction("GetPlayerName", static_cast<const char* (mmPlayerDirectory::*)(const char*) const>(&GetPlayerName))
                .addFunction("GetPlayerNameByIndex", static_cast<const char* (mmPlayerDirectory::*)(int) const>(&GetPlayerName))
                .addFunction("GetFileName", static_cast<const char* (mmPlayerDirectory::*)(const char*) const>(&GetFileName))
                .addFunction("GetFileNameByIndex", static_cast<const char* (mmPlayerDirectory::*)(int) const>(&GetFileName))
                .endClass();
        }
    };
    ASSERT_SIZEOF(mmPlayerDirectory, 0xE4);
}