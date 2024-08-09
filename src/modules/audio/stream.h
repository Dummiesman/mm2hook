#pragma once
#include "sound.h"

namespace MM2
{
    // Forward declarations
    class AudStream;

    // External declarations

    // Class definitions
    class AudStream : public AudSoundBase {
    private:
        float Volume;
    public:
        AGE_API AudStream(unsigned int flags, int soundHandleCount, int unused) {
            scoped_vtable x(this);
            hook::Thunk<0x50E320>::Call<void>(this, flags, soundHandleCount, unused);
        };

        AGE_API virtual ~AudStream() {
            scoped_vtable x(this);
            hook::Thunk<0x50E3C0>::Call<void>(this);
        };

        /*
            AudStream
        */
        AGE_API void PlayOnce(const char* filename) { hook::Thunk<0x50E3F0>::Call<void>(this, filename); }
        AGE_API void SetVolume(float volume) { hook::Thunk<0x50E3D0>::Call<void>(this, volume); }

        static void BindLua(LuaState L) {
            LuaBinding(L).beginExtendClass<AudStream, AudSoundBase>("AudStream")
                .addFactory([](unsigned int flags = 0xC2, int soundHandleCount = 1) { //0xC2 is the return value of Get2DFlags
                return new AudStream(flags, soundHandleCount, 0);
            }, LUA_ARGS(_opt<unsigned int>, _opt<int>))
                .addFunction("PlayOnce", &PlayOnce, LUA_ARGS(const char*))
                .addFunction("SetVolume", &SetVolume)
                .endClass();
        }
    };
}