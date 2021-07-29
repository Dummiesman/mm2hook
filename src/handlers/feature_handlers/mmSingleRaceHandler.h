#pragma once
#include "common.h"
#include "patch.h"
#include "mm2.h"

class mmSingleRaceHandler {
public:
    void QueueCopVoice(float a1);
    void SetPriority(int a1);
    static void Install();
};

