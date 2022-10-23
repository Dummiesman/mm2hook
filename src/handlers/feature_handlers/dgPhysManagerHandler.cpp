#include "dgPhysManagerHandler.h"

using namespace MM2;

/*
    dgPhysManagerHandler
*/

// 1 = Update, 2 = IgnoreMover, 4 = NewMover1, 8 = NewMover2, 16 = NewMover3, 32 = ResetTable, 64 = DeclareMover
// used to test rewritten functions individually or in combo when MAX_MOVERS is default
static ConfigValue<int> cfgPhysRewriteFlags("dbg_PhysRewriteFlags", 127);


void dgPhysManagerHandler::Install() {
    // large parts of dgPhysManager have been rewritten to accomodate an increased MAX_MOVERS value
    // adjust size 
    mem::write(0x4129C7 + 1, sizeof(dgPhysManager));
    
    // adjust offsets in ctor
    int collisionTableStart = 0xA4;
    int collisionTableSize = sizeof(dgPhysManager::CollisionTableEntry) * dgPhysManager::MAX_MOVERS;
    int sampleStepOffset = collisionTableStart + collisionTableSize + 8;
    int maxSamplesOffset = collisionTableStart + collisionTableSize + 4;
    mem::write(0x468216 + 2, sampleStepOffset);
    mem::write(0x46820C + 2, maxSamplesOffset);

    // install callbacks
    int physRewriteFlags = cfgPhysRewriteFlags.Get();
    bool allowVanillaFunctions = (dgPhysManager::MAX_MOVERS == 32);

    if (!allowVanillaFunctions || physRewriteFlags & 1) {
        InstallCallback("dgPhysManager::Update", "Use rewritten Update",
            &dgPhysManager::Update, {
                cb::call(0x403091),
            }
        );
    }

    if (!allowVanillaFunctions || physRewriteFlags & 32) {
        InstallCallback("dgPhysManager::ResetTable", "Use rewritten ResetTable",
            &dgPhysManager::ResetTable, {
                cb::call(0x4682D4), // dgPhysManager::Reset
                cb::call(0x468DCE), // dgPhysManager::Update - eventually will be rewritten too
            }
        );
    }

    if (!allowVanillaFunctions || physRewriteFlags & 2) {
        InstallCallback("dgPhysManager::IgnoreMover", "Use rewritten IgnoreMover",
            &dgPhysManager::IgnoreMover, {
                cb::call(0x440004),
                cb::call(0x5536D5),
            }
        );
    }

    if (!allowVanillaFunctions || physRewriteFlags & 4) {
        InstallCallback("dgPhysManager::NewMover", "Use rewritten NewMover",
            static_cast<void(dgPhysManager::*)(lvlInstance*)>(&dgPhysManager::NewMover), {
                cb::call(0x4D8B8E),
            }
        );
    }

    if (!allowVanillaFunctions || physRewriteFlags & 8) {
        InstallCallback("dgPhysManager::NewMover", "Use rewritten NewMover (2 arg)",
            static_cast<void(dgPhysManager::*)(lvlInstance*, lvlInstance*)>(&dgPhysManager::NewMover), {
                cb::call(0x469991),
                cb::call(0x4699A7),
            }
        );
    }

    if (!allowVanillaFunctions || physRewriteFlags & 16) {
        InstallCallback("dgPhysManager::NewMover", "Use rewritten NewMover (3 arg)",
            static_cast<void(dgPhysManager::*)(lvlInstance*, lvlInstance*, lvlInstance*)>(&dgPhysManager::NewMover), {
                cb::call(0x442554),
                cb::call(0x442446),
            }
        );
    }

    if (!allowVanillaFunctions || physRewriteFlags & 64)
    {
        InstallCallback("dgPhysManager::DeclareMover", "Use rewritten DeclareMover",
            &dgPhysManager::DeclareMover, {
                cb::call(0x41418B),
                cb::call(0x4141AD),

                cb::call(0x43D88C),
                cb::call(0x43D8B1),

                cb::call(0x4404DF),

                cb::call(0x53D45E),
                cb::call(0x53D484),
                cb::call(0x53D49F),

                cb::call(0x53DF90),

                cb::call(0x54AB01),

                cb::call(0x553F66),

                cb::call(0x5545C9),

                cb::call(0x56B1EC),
                cb::call(0x56BEEC),
                cb::call(0x56F61A),

                cb::call(0x57BC6D),
            }
        );
    }
}

