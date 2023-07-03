#include "gizTrainHandler.h"

using namespace MM2;

/*
    gizTrainHandler
*/

void gizTrainHandler::Update() 
{
    auto train = reinterpret_cast<gizTrain*>(this);
    train->Update();
    train->GetAudio()->Update(train->GetSpeedFactor() * 50.0f);
}

void gizTrainHandler::Install()
{
    InstallCallback("gizTrain::Update", "Use proper speed value for audio.",
        &Update, {
            cb::call(0x579193),
        }
    );

    // NOP out old Audio Update
    InstallPatch({ 0x90, 0x90, 0x90, 0x90, 0x90, 0x90,0x90, 0x90, 0x90, 0x90, 0x90 }, {
        0x578EC9
    });

    // Start the train at speed 0.0... Not speed 1.0
    InstallPatch({ 0x00, 0x00, 0x00, 0x00 }, {
        0x578C31 + 3
    });
}
