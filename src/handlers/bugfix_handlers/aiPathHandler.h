#pragma once
#include "common.h"
#include "patch.h"
#include "mm2.h"

#include "aiPedestrianHandler.h"

class aiPathHandler {
public:
    void UpdatePedestrians();

    static void Install();
};