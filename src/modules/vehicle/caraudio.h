#pragma once
#include <modules\audio\sound.h>
#include <modules\audio\aud3dobject.h>
#include <modules\vehicle\car.h>

namespace MM2
{
    // Forward declarations
    class vehCarAudio;

    // External declarations


    // Class definitions

    class vehCarAudio : public Aud3DObject {
    private:
        byte _buffer[0xCF];
    protected:
        hook::Field<0x118, vehCarSim *> _sim;
    public:
        vehCarSim * getCarSim() const {
            return _sim.get(this);
        };

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
        AGE_API bool IsAirBorne()                               { return hook::Thunk<0x4DC340>::Call<bool>(this); }
        AGE_API bool IsBrakeing()                               { return hook::Thunk<0x4DC4F0>::Call<bool>(this); }
        AGE_API void SetMinAmpSpeed(float speed)                { hook::Thunk<0x4DC000>::Call<void>(this, speed); }
        AGE_API void PlayHorn()                                 { hook::Thunk<0x4DC1D0>::Call<void>(this); }
        AGE_API void StopHorn()                                 { hook::Thunk<0x4DC210>::Call<void>(this); }

        static void BindLua(LuaState L) {
            LuaBinding(L).beginExtendClass<vehCarAudio, Aud3DObject>("vehCarAudio")
                .addPropertyReadOnly("IsAirBorne", &IsAirBorne)
                .addPropertyReadOnly("IsBrakeing", &IsBrakeing)
                .addFunction("SetMinAmpSpeed", &SetMinAmpSpeed)
                .addFunction("PlayHorn", &PlayHorn)
                .addFunction("StopHorn", &StopHorn)
                .endClass();
        }
    };

    ASSERT_SIZEOF(vehCarAudio, 0x130);

    // Lua initialization

}