#pragma once
#include "DMusicManager.h"

namespace MM2
{
    //declfield(datTimeManager::Seconds)(0x5CE820);
    void DMusicManager::SetPan(float pan)
    {
        hook::Thunk<0x517540>::Call<void>(this, pan);
    }

    void DMusicManager::SetVolume(float volume)
    {
        hook::Thunk<0x5174F0>::Call<void>(this, volume);
    }

    DMusicObject* DMusicManager::GetDMusicObjectPtr() const
    {
        return hook::Thunk<0x5174D0>::Call<DMusicObject*>(this);
    }

    void DMusicManager::BindLua(LuaState L) {
        LuaBinding(L).beginClass<DMusicManager>("DMusicManager")
            .addFunction("GetDMusicObjectPtr", &GetDMusicObjectPtr)
            .addFunction("SetVolume", &SetVolume)
            .addFunction("SetPan", &SetPan)
            .endClass();
    }
}