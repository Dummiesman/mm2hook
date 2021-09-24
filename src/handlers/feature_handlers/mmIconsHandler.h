#pragma once
#include "common.h"
#include "patch.h"
#include "mm2.h"

#include "Wanted_Common.h"

class mmIconsHandler {
public:
    void RegisterOpponents(MM2::OppIconInfo *icons, int count, void *a3);
    void RegisterOpponents_Blitz(MM2::OppIconInfo *icons, int count, void *a3);

    static void Install();
};

