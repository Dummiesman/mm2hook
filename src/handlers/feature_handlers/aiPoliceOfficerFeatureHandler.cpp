#include "aiPoliceOfficerFeatureHandler.h"

using namespace MM2;

/*
    aiPoliceOfficerFeatureHandler
*/

void* aiPoliceOfficerFeatureHandler::AllocHook(unsigned int size)
{
    unsigned int newSize = (sizeof(aiPoliceOfficer) * aiMap::GetInstance()->numCops) + 4;
    void* newMem = hook::StaticThunk<0x577360>::Call<void*>(newSize);
    return newMem;
}

void aiPoliceOfficerFeatureHandler::PlacementNew()
{
    void* buf = reinterpret_cast<void*>(this);
    new(buf) aiPoliceOfficer();
}

void aiPoliceOfficerFeatureHandler::DetectPerpetrator() {
    mmGameManager *mgr = mmGameManager::Instance;
    auto player = mgr->getGame()->GetPlayer();

    if (*getPtr<int>(player, 0x2258))
        return;

    //call original
    hook::Thunk<0x53DFD0>::Call<void>(this);
}

void aiPoliceOfficerFeatureHandler::Install() {
    InstallCallback("aiMap::Init", "Change memory size for aiPoliceOfficer.",
        &AllocHook, {
            cb::call(0x535D24), // aiMap::Init operator new
        }
    );

    InstallCallback("aiPoliceOfficer::DetectPerpetrator", "Doesn't make cops detect the player once they cross the finish line.",
        &DetectPerpetrator, {
            cb::call(0x53DC91),
        }
    );

    // Install new size for aiPoliceOfficer in Update/Reset
    //B8 D3ADB33F - mov eax, (Placeholder new size)
    //0FAF C7 - imul eax, edi
    //8B 4E 70 - mov ecx, [esi + 70]
    //8D 0C 08 - lea ecx, [eax + ecx]

    int newSize = sizeof(aiPoliceOfficer);

    InstallPatch({ 0xB8, 0xD3, 0xAD, 0xB3, 0x3F,
                   0x0F, 0xAF, 0xC7,
                   0x8B, 0x4E, 0x4C,
                   0x8D, 0x0C, 0x08,
                   0x90,0x90, 0x90, 0x90, 0x90, 0x90, 0x90, 0x90, 0x90, 0x90 }, {
        0x536D77,
        0x537250,
                   });

    // Install new size for aiPoliceOfficer
    mem::write(0x536D77 + 1, newSize);
    mem::write(0x537250 + 1, newSize);
    mem::write(0x535D48 + 1, newSize);
    mem::write(0x535D7D + 2, newSize);
    mem::write(0x5374C6 + 2, newSize);
    mem::write(0x53BD1C + 1, newSize);
    mem::write(0x534C90 + 1, newSize);

    InstallCallback("aiMap::Police", "Use new AIMAP::Police implementation.",
        &aiMap::Police, {
            cb::call(0x41A1D5),
            cb::call(0x41A210),
        }
    );

    InstallCallback("aiPoliceOfficer", "Use new constructor.",
        &PlacementNew, {
            cb::jmp(0x53D910),
        }
    );
}

