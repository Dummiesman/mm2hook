#pragma once
#include "common.h"
#include "patch.h"
#include "mm2.h"

class gettexHandler {
public:
    static MM2::gfxImage* PrepareHook(MM2::gfxImage* image, const char* name, bool lastMip);
    static void Install();
};