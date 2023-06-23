#pragma once
#include "common.h"
#include "patch.h"
#include "mm2.h"

class mmIconsHandler {
public:
    void RegisterOpponents(MM2::OppIconInfo *icons, int count, MM2::LocFont* a3);
    void RegisterOpponents_Blitz(MM2::OppIconInfo *icons, int count, MM2::LocFont* a3);

    static void Install();
};

