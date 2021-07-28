#pragma once
#include "common.h"
#include "patch.h"
#include "mm2.h"

class vehCarAudioContainerBugfixHandler : handler_t {
public:
    void StartSiren();
    void StopSiren();

    static void Install(void);
};