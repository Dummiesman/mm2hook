#pragma once
#include <modules\audio\sound.h>
#include <modules\audio\aud3dobject.h>
#include <modules\vehicle\car.h>

namespace MM2
{
    // Forward declarations
    class vehCarAudio;

    // External declarations
    extern class vehEngineAudio;

    // Class definitions
    class vehCarAudio : public Aud3DObject {
    private:
        byte _buffer[0xD0];
    protected:
        static hook::Field<0x104, vehEngineAudio> _engineAudio;
        static hook::Field<0x118, vehCarSim *> _sim;
    public:
        /*
            Aud3DObject virtuals
        */
        AGE_API void AssignSounds() override                    { hook::Thunk<0x4DBE40>::Call<void>(this); }
        AGE_API void UnAssignSounds(int a1) override            { hook::Thunk<0x4DBF80>::Call<void>(this, a1); }
        AGE_API void UpdateAudio() override                     { hook::Thunk<0x4DBBB0>::Call<void>(this); }
        AGE_API void Update() override                          { hook::Thunk<0x4DC320>::Call<void>(this); }
        AGE_API void SetNon3DParams() override                  { hook::Thunk<0x4DC240>::Call<void>(this); }
        AGE_API void Set3DParams()  override                    { hook::Thunk<0x5B31A4>::Call<void>(this); }

        /*
            vehCarAudio
        */
        AGE_API bool IsAirBorne() const                         { return hook::Thunk<0x4DC340>::Call<bool>(this); }
        AGE_API bool IsBrakeing() const                         { return hook::Thunk<0x4DC4F0>::Call<bool>(this); }
        AGE_API void SetMinAmpSpeed(float speed)                { hook::Thunk<0x4DC000>::Call<void>(this, speed); }
        AGE_API void PlayHorn()                                 { hook::Thunk<0x4DC1D0>::Call<void>(this); }
        AGE_API void StopHorn()                                 { hook::Thunk<0x4DC210>::Call<void>(this); }

        vehCarSim* GetCarSim() const {
            return _sim.get(this);
        };

        vehEngineAudio* GetEngineAudio() const
        {
            return _engineAudio.ptr(this);
        }

        static void BindLua(LuaState L) {
            LuaBinding(L).beginExtendClass<vehCarAudio, Aud3DObject>("vehCarAudio")
                .addPropertyReadOnly("IsAirBorne", &IsAirBorne)
                .addPropertyReadOnly("IsBrakeing", &IsBrakeing)
                .addPropertyReadOnly("EngineAudio", &GetEngineAudio)
                .addFunction("SetMinAmpSpeed", &SetMinAmpSpeed)
                .addFunction("PlayHorn", &PlayHorn)
                .addFunction("StopHorn", &StopHorn)
                .endClass();
        }
    };

    ASSERT_SIZEOF(vehCarAudio, 0x130);
}