#pragma once
#include "common.h"
#include "patch.h"
#include "mm2.h"

class gfxImageHandler
{
public:
    void Scale(int width, int height);

    static void Install();
};