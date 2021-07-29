#pragma once
#include "common.h"
#include "patch.h"
#include "mm2.h"

class asCullManagerHandler {
public:
    void Init(int maxCullables, int maxCullables2D);

    static void Install();
};

