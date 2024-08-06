#pragma once
#include <mm2_lua.h>
#include "sound.h"


namespace MM2
{
    // Forward declarations
    struct AudSpeechData;
    struct tagSpeechQueueData;
    class AudSpeech;

    // External declarations

    // Class definitions
    struct AudSpeechData
    {
        char* Name;
        float EndSuffixNumber;
        float AddValue;
        int dword_0c;
        int LastOneShotIndex;
        int SoundBaseCount;
        AudSoundBase** SoundBases;
    };


    struct tagSpeechQueueData 
    {
        int CategoryIndex;
        int SoundIndex;
        float TimeUntilPlay;
    };

    class AudSpeech
    {
    private:
        AudSpeechData** SpeechData;
        void* Stream;
        AudSoundBase* CurrentOneShot;
        tagSpeechQueueData** Queue;
        int SpeechDataCount;
        int QueueSize;
        char char_18;
        char Streaming;
        char SubPath[80];
        char Extension[4];
    private:
        // change this, im lazy
        void playLua(int cat, float timeUntilPlay)
        {
            this->Play(cat, timeUntilPlay);
        }
    public:
        ANGEL_ALLOCATOR

        AGE_API AudSpeech(bool stream) {
            scoped_vtable x(this);
            hook::Thunk<0x510090>::Call<void>(this, stream);
        };

        AGE_API ~AudSpeech() {
            scoped_vtable x(this);
            hook::Thunk<0x510120>::Call<void>(this);
        };

        /*
            AudSpeech
        */
        AGE_API bool IsPlaying() const                          { return hook::Thunk<0x5105E0>::Call<bool>(this); }
        AGE_API void SetExtension(LPCSTR extension)             { hook::Thunk<0x510220>::Call<void>(this, extension); }
        AGE_API void SetSubPath(LPCSTR extension)               { hook::Thunk<0x5101D0>::Call<void>(this, extension); }
        AGE_API void SetVolume(float volume)                    { hook::Thunk<0x510480>::Call<void>(this, volume); }
        AGE_API void Play(int category, float timeUntilPlay = 0.0f)
                                                                { hook::Thunk<0x510450>::Call<void>(this, category, timeUntilPlay); }
        AGE_API void Play(int category, int soundIndex, float timeUntilPlay = 0.0f)    
                                                                { hook::Thunk<0x05106B0>::Call<void>(this, category, soundIndex, timeUntilPlay); }
        AGE_API void Stop()                                     { hook::Thunk<0x510840>::Call<void>(this); }
        AGE_API void AllocateQueuePlayData()                    { hook::Thunk<0x510350>::Call<void>(this); }
        AGE_API void AllocateSpeechData(LPCSTR name, float endValue, float addValue)
                                                                { hook::Thunk<0x510270>::Call<void>(this, name, endValue, addValue); }
        AGE_API void Update()                                   { hook::Thunk<0x510720>::Call<void>(this); }

        int GetSpeechDataCount() const
        {
            return this->SpeechDataCount;
        }

        static void BindLua(LuaState L) {
            LuaBinding(L).beginClass<AudSpeech>("AudSpeech")
                .addFactory([](bool stream) {
                    auto object = new AudSpeech(stream);
                    //MM2Lua::MarkForCleanupOnShutdown(object);
                    return object;
                })
                .addPropertyReadOnly("IsPlaying", &IsPlaying)
                .addPropertyReadOnly("NumSpeechDatas", &GetSpeechDataCount)
                .addFunction("SetExtension", &SetExtension)
                .addFunction("SetSubPath", &SetSubPath)
                .addFunction("SetVolume", &SetVolume)
                .addFunction("PlayRandom", static_cast<void(AudSpeech::*)(int, float)>(&Play))
                .addFunction("Play", static_cast<void(AudSpeech::*)(int, int, float)>(&Play))
                .addFunction("Stop", &Stop)
                .addFunction("AllocateQueuePlayData", &AllocateQueuePlayData)
                .addFunction("AllocateSpeechData", &AllocateSpeechData)
                .addFunction("Update", &Update)

                .endClass();
        }
    };
    ASSERT_SIZEOF(AudSpeech, 0x70);
}