#pragma once
#include <modules\node.h>

namespace MM2
{
    // Forward declarations
    class mmCDPlayer;

    // External declarations


    // Class definitions
    class mmCDPlayer : public asNode {
    private:
        byte buffer[0x138];
    protected:
        hook::Field<0x1C, int> _currentTrack;
        hook::Field<0x24, int> _isPlaying;
    public:
        inline int getCurrentTrack(void) const {
            return _currentTrack.get(this);
        };

        inline bool getIsPlaying(void) const {
            return (_isPlaying.get(this) > 0);
        };

        AGE_API void Toggle()                               { hook::Thunk<0x4334D0>::Call<void>(this); }
        AGE_API void PrevTrack()                            { hook::Thunk<0x433460>::Call<void>(this); }
        AGE_API void PlayStop()                             { hook::Thunk<0x433370>::Call<void>(this); }
        AGE_API void NextTrack()                            { hook::Thunk<0x4333F0>::Call<void>(this); }

        /*
            asNode virtuals
        */
        AGE_API void Cull()  override                       { hook::Thunk<0x433170>::Call<void>(this); }
        AGE_API void Update()  override                     { hook::Thunk<0x433100>::Call<void>(this); }
        AGE_API void Reset()  override                      { hook::Thunk<0x4330F0>::Call<void>(this); }

        static void BindLua(LuaState L) {
            LuaBinding(L).beginExtendClass<mmCDPlayer, asNode>("mmCDPlayer")
                .addPropertyReadOnly("CurrentTrack", &getCurrentTrack)
                .addPropertyReadOnly("IsPlaying", &getIsPlaying)
                .addFunction("Toggle", &Toggle)
                .addFunction("PrevTrack", &PrevTrack)
                .addFunction("NextTrack", &NextTrack)
                .addFunction("PlayStop", &PlayStop)
            .endClass();
        }
    };
}