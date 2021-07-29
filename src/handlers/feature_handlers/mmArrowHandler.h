#pragma once
#include "common.h"
#include "patch.h"
#include "mm2.h"

class mmArrowHandler {
public:
    void SetShape(LPCSTR modelName, LPCSTR dirName, bool useLVertex, MM2::Vector3* a4);
    static void Install();
};

