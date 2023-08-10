#pragma once
#include <mm2_common.h>
#include <modules\node\node.h>

namespace MM2
{
    // Forward declarations
    class AudSoundBase;

    // External declarations

    // Class definitions
    class AudSoundBase : public asNode {
    private:
        byte _buffer[0x40];
    private:
        //lua helper
        bool LoadLua(LPCSTR wavName, int handleIndex) {
            return Load(wavName, handleIndex, false) != FALSE;
        }
    public:
        AGE_API AudSoundBase() {
            scoped_vtable x(this);
            hook::Thunk<0x50D580>::Call<void>(this);
        };

        AGE_API AudSoundBase(unsigned int flags, int soundHandleCount, int unused) {
            scoped_vtable x(this);
            hook::Thunk<0x50D6D0>::Call<void>(this, flags, soundHandleCount, unused);
        };

        AGE_API virtual ~AudSoundBase() {
            scoped_vtable x(this);
            hook::Thunk<0x50D7B0>::Call<void>(this);
        };

        /*
            asNode virtuals
        */
        AGE_API void Update() override { hook::Thunk<0x50DBE0>::Call<void>(this); }

        /*
            AudSoundBase
        */
        AGE_API static unsigned int Get3DFlags() { return hook::StaticThunk<0x50E070>::Call<unsigned int>(); }
        AGE_API static unsigned int Get2DFlags() { return hook::StaticThunk<0x50E030>::Call<unsigned int>(); }
        AGE_API static unsigned int GetSoft2DFlags() { return hook::StaticThunk<0x50E040>::Call<unsigned int>(); }

        AGE_API bool IsPlaying() { return hook::Thunk<0x50E250>::Call<bool>(this); }
        AGE_API void Enable3DMode() { hook::Thunk<0x50E2F0>::Call<void>(this); }
        AGE_API void Disable3DMode() { hook::Thunk<0x50E300>::Call<void>(this); }
        AGE_API BOOL Load(LPCSTR wavName, int handle, bool unknown)
        {
            return hook::Thunk<0x50DE90>::Call<BOOL>(this, wavName, handle, unknown);
        }
        AGE_API void PlayOnce(float volume, float pitch) { hook::Thunk<0x50E090>::Call<void>(this, volume, pitch); }
        AGE_API void PlayLoop(float volume, float pitch) { hook::Thunk<0x50E150>::Call<void>(this, volume, pitch); }
        AGE_API void Stop() { hook::Thunk<0x50E1F0>::Call<void>(this); }
        AGE_API void SetFrequency(float frequency) { hook::Thunk<0x50DAB0>::Call<void>(this, frequency); }
        AGE_API void SetVolume(float volume) { hook::Thunk<0x50DA30>::Call<void>(this, volume); }
        AGE_API void SetPan(float pan, int a2) { hook::Thunk<0x50DB30>::Call<void>(this, pan, a2); }
        AGE_API void SetSoundHandleIndex(int index) { hook::Thunk<0x50E2C0>::Call<void>(this, index); }
        AGE_API void SetExtension(LPCSTR extension) { hook::Thunk<0x50D900>::Call<void>(this, extension); }
        AGE_API void SetSubPath(LPCSTR path) { hook::Thunk<0x50D8D0>::Call<void>(this, path); }
        AGE_API void SetPlayPosition(unsigned int position)
        {
            hook::Thunk<0x50E2E0>::Call<void>(this, position);
        }
        AGE_API void SetPriority(int a1) { hook::Thunk<0x50DB10>::Call<void>(this, a1); }
        AGE_API int GetSoundHandleIndex() { return hook::Thunk<0x50E2D0>::Call<int>(this); }
        AGE_API int GetNumSoundHandles() { return hook::Thunk<0x50E310>::Call<int>(this); }

        static void BindLua(LuaState L) {
            LuaBinding(L).beginExtendClass<AudSoundBase, asNode>("AudSoundBase")
                .addFactory([](unsigned int flags = 0xC2, int soundHandleCount = 1) { //0xC2 is the return value of Get2DFlags
                return new AudSoundBase(flags, soundHandleCount, -1);
            }, LUA_ARGS(_opt<unsigned int>, _opt<int>))

                .addStaticFunction("Get3DFlags", &Get3DFlags)
                .addStaticFunction("Get2DFlags", &Get2DFlags)
                .addStaticFunction("GetSoft2DFlags", &GetSoft2DFlags)

                .addPropertyReadOnly("NumSoundHandles", &GetNumSoundHandles)
                .addPropertyReadOnly("IsPlaying", &IsPlaying)
                .addProperty("SoundHandleIndex", &GetSoundHandleIndex, &SetSoundHandleIndex)

                .addFunction("Load", &LoadLua)

                .addFunction("Enable3DMode", &Enable3DMode)
                .addFunction("Disable3DMode", &Disable3DMode)

                .addFunction("PlayOnce", &PlayOnce, LUA_ARGS(_def<float, -1>, _def<float, -1>))
                .addFunction("PlayLoop", &PlayLoop, LUA_ARGS(_def<float, -1>, _def<float, -1>))
                .addFunction("Stop", &Stop)
                .addFunction("SetFrequency", &SetFrequency)
                .addFunction("SetPan", &SetPan)
                .addFunction("SetVolume", &SetVolume)

                .addFunction("SetSubPath", &SetSubPath)
                .addFunction("SetExtension", &SetExtension)
                .endClass();
        }
    };
}