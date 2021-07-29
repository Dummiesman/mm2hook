#pragma once
#include "common.h"
#include "patch.h"
#include "mm2.h"

class gizParkedCarMgrHandler {
public:
    static void EnumeratePath(LPCSTR a1, const MM2::Matrix34* a2, bool a3);
    static void Install();
};