#pragma once
#include "common.h"
#include "patch.h"
#include "mm2.h"

class mmHudMapHandler {
public:
    bool CanActivateMap();
    void Activate();
    void SetMapMode(int mode);
    static void Install();
};