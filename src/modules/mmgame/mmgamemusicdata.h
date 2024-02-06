#pragma once
#include <modules\node.h>

namespace MM2
{
    // Forward declarations
    class mmGameMusicData;
    class mmSingleRaceMusicData;
    class mmSingleRoamMusicData;

    // External declarations


    // Class definitions
    class mmGameMusicData {
    public:
        AGE_API bool LoadAmbientSFX(LPCSTR name)                { return hook::Thunk<0x434060>::Call<bool>(this, name); }
        AGE_API bool Load(LPCSTR musicCsv, LPCSTR ambientCsv)   { return hook::Thunk<0x433F40>::Call<bool>(this, musicCsv, ambientCsv); }
        AGE_API virtual bool LoadMusic(LPCSTR csvName)          { return hook::Thunk<0x434250>::Call<bool>(this, csvName); }

        static void BindLua(LuaState L) {
            LuaBinding(L).beginClass<mmGameMusicData>("mmGameMusicData")
                .addFunction("Load", &Load)
                .endClass();
        }
    };

    class mmSingleRaceMusicData : public mmGameMusicData
    {
    public:
        AGE_API virtual bool LoadMusic(LPCSTR csvName) override { return hook::Thunk<0x4336D0>::Call<bool>(this, csvName); }

        static void BindLua(LuaState L) {
            LuaBinding(L).beginExtendClass<mmSingleRaceMusicData, mmGameMusicData>("mmSingleRaceMusicData")
                .endClass();
        }
    };

    class mmSingleRoamMusicData : public mmGameMusicData
    {
    public:
        AGE_API virtual bool LoadMusic(LPCSTR csvName) override { return hook::Thunk<0x438980>::Call<bool>(this, csvName); }

        static void BindLua(LuaState L) {
            LuaBinding(L).beginExtendClass<mmSingleRoamMusicData, mmGameMusicData>("mmSingleRoamMusicData")
                .endClass();
        }
    };
}