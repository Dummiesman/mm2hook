#pragma once
#include "common.h"
#include "patch.h"
#include "mm2.h"

class vehSirenHandler {
public:
    void Update();
    void Reset();
    void* SizeOf();
    static void Install();
};

