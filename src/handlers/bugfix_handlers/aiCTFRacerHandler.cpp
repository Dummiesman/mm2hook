#include "aiCTFRacerHandler.h"

using namespace MM2;

/*
    aiCTFRacerHandler
*/

void aiCTFRacerHandler::Init(int id, char* basename)
{
    //by default aiCTFRacer uses a hardcoded basename only present in Midnight Club
    LPCSTR aiType = aiMap::GetCTFOpponentType(id % aiMap::GetCTFOpponentTypeCount());
    reinterpret_cast<aiCTFRacer*>(this)->aiCTFRacer::Init(id, aiType);
}

void* aiCTFRacerHandler::AllocHook(unsigned int size)
{
    unsigned int newSize = (sizeof(aiCTFRacer) * aiMap::GetInstance()->numCTFRacers) + 4;
    void* newMem = hook::StaticThunk<0x577360>::Call<void*>(newSize);
    return newMem;
}

void aiCTFRacerHandler::Install()
{
    InstallCallback("aiMap::Init", "Change memory size for aiCTFRacer.",
        &AllocHook, {
            cb::call(0x535488), // aiMap::Init operator new
        }
    );

    InstallCallback("aiMap::Init", "Change basename for aiCTFRacer.",
        &Init, {
            cb::call(0x5354DE), // aiCTFRacer::Init
        }
    );

    InstallCallback("aiCTFRacer::Update", "Use rewritten Update function.",
        &aiCTFRacer::Update, {
            cb::call(0x53711D),
        }
    );

    InstallCallback("aiCTFRacer::Reset", "Use rewritten Reset function.",
        &aiCTFRacer::Reset, {
            cb::call(0x536BD4),
            cb::call(0x5545FE),
        }
    );

    // Install new size for aiCTFRacer in Update/Reset
    //B8 D3ADB33F - mov eax, (Placeholder new size)
    //0FAF C7 - imul eax, edi
    //8B 4E 70 - mov ecx, [esi + 70]
    //8D 0C 08 - lea ecx, [eax + ecx]

    unsigned int newSize = sizeof(aiCTFRacer);

    InstallPatch({ 0xB8, 0xD3, 0xAD, 0xB3, 0x3F,
                   0x0F, 0xAF, 0xC7,
                   0x8B, 0x4E, 0x70,
                   0x8D, 0x0C, 0x08,
                   0x90,0x90, 0x90, 0x90, 0x90, 0x90, 0x90, 0x90, 0x90, 0x90}, {
        0x536BBC,
        0x537105,
        });

    // Install new size for aiCTFRacer
    mem::write(0x536BBC + 1, newSize);
    mem::write(0x537105 + 1, newSize);
    mem::write(0x5354AC + 1, newSize);
    mem::write(0x5354E8 + 2, newSize);
    mem::write(0x53746C + 2, newSize);
    mem::write(0x53BFBC + 1, newSize);
}