#pragma once
#include "common.h"
#include "patch.h"
#include "mm2.h"

class aiPedestrianHandler
{
public:
    static int UpdateAttempts;
    void Update();
    static void Install();
};