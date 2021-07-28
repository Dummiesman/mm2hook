#include "mmBillInstanceHandler.h"

using namespace MM2;

/*
    mmBillInstanceHandler
*/

void mmBillInstanceHandler::Scale(float x, float y, float z) {
    hook::Thunk<0x4BE560>::Call<void>(this, x, y, -z);
}

void mmBillInstanceHandler::Install() {
    InstallCallback("mmBillInstance::Draw", "Fix inverted checkpoints",
        &Scale, {
            cb::call(0x43F952), // mmBillInstance::Draw
        }
    );

    Installf("Installing fix for vertical billboarding of CnR checkpoints...");
    mem::nop(0x43F8FD, (6 * 3)); // 6 fld/fstp instructions (size: 3)
}