#pragma once
#include "common.h"
#include "patch.h"
#include "mm2.h"

class vehBreakableMgrBugfixHandler {
private:
    static bool GetPivotHook(MM2::Matrix34& mtx, const char* basename, const char* file);
public:
    static void Install();
};