#pragma once
#include <mm2_common.h>
#include "aud3dobject.h"

namespace MM2
{
    // Forward declarations
    class AudCreatureContainer;

    // External declarations

    // Class definitions
    class AudCreatureContainer : public Aud3DObject {
    private:
        byte _buffer[0x14];
    public:
        /*
            Aud3DObject virtuals
        */
        AGE_API virtual void AssignSounds() override            { hook::Thunk<0x5109A0>::Call<void>(this); }
        AGE_API virtual void UnAssignSounds(int a1) override    { hook::Thunk<0x5109E0>::Call<void>(this, a1); }
        AGE_API virtual void UpdateAudio() override             { hook::Thunk<0x510A20>::Call<void>(this); }
        AGE_API virtual void Update() override                  { hook::Thunk<0x510950>::Call<void>(this); }

        /*
            AudCreatureContainer
        */
        AGE_API void PlayImpactReaction(float force)            { hook::Thunk<0x510CC0>::Call<void>(this, force); }
        AGE_API void PlayAvoidanceReaction(float a1)            { hook::Thunk<0x510CF0>::Call<void>(this, a1); }

        static void BindLua(LuaState L) {
            LuaBinding(L).beginExtendClass<AudCreatureContainer, Aud3DObject>("AudCreatureContainer")
                .addFunction("PlayImpactReaction", &PlayImpactReaction)
                .addFunction("PlayAvoidanceReaction", &PlayAvoidanceReaction)
                .endClass();
        }
    };

    ASSERT_SIZEOF(AudCreatureContainer, 0x74);
}