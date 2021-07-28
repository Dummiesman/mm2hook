#pragma once
#include "common.h"
#include "patch.h"
#include "mm2.h"

class mmPopupHandler {
public:
    void HudEnable();
    void HudDisable(int a1);
    static void Install();
};