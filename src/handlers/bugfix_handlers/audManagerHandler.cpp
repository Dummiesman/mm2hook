#include "audManagerHandler.h"

using namespace MM2;

static ConfigValue<int> cfgAudioMaxSounds("AudioMaxSounds", 800);
static ConfigValue<int> cfgAudioMaxConcurrent("AudioMaxConcurrent", 32);

/*
    audManagerHandler
*/

// reimplemented using similar logic found in MM1
// AudManager is aware of a Mixer, but it's never initialized
class MixerCTL {
private:
    float unk_04;
    float unk_08;
    float unk_0C;

    float unk_10;
    float unk_14;
    float unk_18;

    HWND window;

    int unk_20;
    int unk_24; // = 1?

    uint32_t mixer_id;

    float wave_balance;
    float cd_balance; // initialized but not used
public:
    explicit MixerCTL() {
        // override the vtable
        *reinterpret_cast<int*>(this) = 0x5B4E70;

        unk_04 = 1.0f;
        unk_08 = 1.0f;
        unk_0C = 1.0f;

        unk_10 = 1.0f;
        unk_14 = 1.0f;
        unk_18 = 1.0f;

        window = GetActiveWindow();

        unk_24 = 1;

        wave_balance = 0.0f;
        cd_balance = 0.0f;
    }

    ULONG Init(void) {
        return hook::Thunk<0x51C1F0>::Call<ulong>(this);
    }

    virtual ~MixerCTL() FORWARD_THUNK;
};

ASSERT_SIZEOF(MixerCTL, 0x34);

static MixerCTL* CreateMixerCTL() {
    auto result = new MixerCTL();

    // initialize the mixer
    result->Init();

    return result;
}

void audManagerHandler::Init(int maxSounds, int a2, int a3, char* a4, short a5, short a6) {
    maxSounds = cfgAudioMaxSounds;

    LogFile::Printf(1, "[audManagerHandler::Init]: %d max sounds", maxSounds);

#ifdef USE_MIXER_STUFF
    // mixer already initialized?
    if (*getPtr<void*>(this, 0x34) == nullptr) {
        LogFile::Printf(1, "[audManagerHandler::Init]: Creating mixer...");

        auto mixer = CreateMixerCTL();

        setPtr<MixerCTL*>(this, 0x34, mixer);
    }
#endif

    hook::Thunk<0x519350>::Call<void>(this, maxSounds, a2, a3, a4, a5, a6);
}

void audManagerHandler::SetMaxConcurrent(int type, int value)
{
    if (type == 1) value = cfgAudioMaxConcurrent.Get();
    hook::Thunk<0x5A1B60>::Call<void>(this, type, value);
}

void audManagerHandler::AssignCDVolume(float value) {
    // update mixer volume first
    SetMixerCDVolume(value);

    // to prevent CD volume acting as a "master volume" slider...
    // I have no idea why this is needed :/
    SetMixerWaveVolume(MMSTATE->SoundFXVolume);

    // AudManager::AssignCDVolume
    hook::Thunk<0x519A30>::Call<void>(this, value);
}

void audManagerHandler::SetupCDAudio(float balance) {
    SetMixerCDVolume(MMSTATE->MusicVolume);
    SetMixerWaveVolume(MMSTATE->SoundFXVolume);

    // AudManager::AssignCDBalance
    hook::Thunk<0x519880>::Call<void>(this, balance);
}

void audManagerHandler::SetMixerCDVolume(float value) {
    auto mixer = *getPtr<void*>(this, 0x34);

    if (mixer != nullptr) {
        LogFile::Printf(1, "[audManagerHandler::SetMixerVolume]: Setting mixer CD volume to %.2f", value);

        // MixerCTL::AssignCDVolume
        hook::Thunk<0x51C330>::Call<void>((MixerCTL*)mixer, value);
    }
}

void audManagerHandler::SetMixerWaveVolume(float value) {
    auto mixer = *getPtr<void*>(this, 0x34);

    if (mixer != nullptr) {
        LogFile::Printf(1, "[audManagerHandler::SetMixerVolume]: Setting mixer sound volume to %.2f", value);

        // MixerCTL::AssignWaveVolume
        hook::Thunk<0x51C310>::Call<void>((MixerCTL*)mixer, value);
    }
}

bool audManagerHandler::MinInstall() {
    return !datAssetManager::Exists("aud\\aud11\\al2", "AL2Pre01", "11k.wav");
}

void audManagerHandler::Install() {
    int audHeapSize = HookConfig::GetProperty("AudioHeapSize", 32);

    LogFile::Printf(1, "Audio heap size: %dMB", audHeapSize);

    mem::write(0x51938D + 1, (int)(audHeapSize * 1000000));

    InstallCallback("AudManager::Init", "Allows the mixer control to be initialized along with the audio manager.",
        &Init, {
            cb::call(0x401F1B),
        }
    );

    InstallCallback("audManager::SetMaxConcurrent", "Allows customization of max concurrent sounds.",
        &SetMaxConcurrent, {
            cb::call(0x5193F9),
            cb::call(0x50EF46),
        }
    );

#ifdef USE_MIXER_STUFF
    InstallCallback("AudManager::AssignCDVolume", "Properly sets mixer volume when changing CD volume.",
        &AssignCDVolume, {
            cb::call(0x401F81), // InitAudioManager
            cb::call(0x50C932), // PUAudioOptions::SetCDVolume
        }
    );

    InstallCallback("AudManager::AssignWaveVolume", "Properly sets mixer volume when changing sound volume.",
        &AssignCDVolume, {
            cb::call(0x401F71), // InitAudioManager
            cb::call(0x50C8FC), // PUAudioOptions::SetWaveVolume
        }
    );

    InstallCallback(&SetupCDAudio, "Allows the mixer volume to be updated when loading player config.", {
            cb::call(0x525DC6), // mmPlayerConfig::SetAudio
        }
    );
#endif

    InstallCallback("AudManager::MinInstall", "Non registry dependent minimum install check.",
        &MinInstall, {
            cb::jmp(0x519470),
        }
    );
}