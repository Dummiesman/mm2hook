#pragma once
#include "timemgr.h"

namespace MM2
{
    declfield(datTimeManager::Seconds)(0x5CE820);
    declfield(datTimeManager::ActualSeconds)(0x5CE824);
    declfield(datTimeManager::InvSeconds)(0x5CE828);
    declfield(datTimeManager::FPS)(0x5CE82C);
    declfield(datTimeManager::Time)(0x6A3C48);
    declfield(datTimeManager::FrameCount)(0x6A3C50);
    declfield(datTimeManager::ElapsedTime)(0x6A3C40);

    AGE_API void datTimeManager::SetTempOversampling(bool a1, int a2)
    {
        hook::StaticThunk<0x4C64F0>::Call<void>(a1, a2);
    }

    AGE_API void datTimeManager::RealTime(float step) {
        hook::StaticThunk<0x4C6580>::Call<void>(step);
    }

    AGE_API void datTimeManager::Reset() {
        hook::StaticThunk<0x4C6300>::Call<void>();
    };

    AGE_API void datTimeManager::Update() {
        hook::StaticThunk<0x4C6340>::Call<void>();
    };

    void datTimeManager::BindLua(LuaState L) {
        LuaBinding(L).beginClass<datTimeManager>("datTimeManager")
            .addStaticProperty("Seconds", [] {return Seconds.get(); })
            .addStaticProperty("InvSeconds", [] {return InvSeconds.get(); })
            .addStaticProperty("FPS", [] {return FPS.get(); })
            .addStaticProperty("ActualSeconds", [] {return ActualSeconds.get(); })
            .addStaticProperty("FrameCount", []() {return FrameCount.get(); })
            .addStaticProperty("ElapsedTime", []() {return ElapsedTime.get(); })
            .addStaticFunction("RealTime", &RealTime, LUA_ARGS(_def<float, 0>))
            .endClass();
    }
}