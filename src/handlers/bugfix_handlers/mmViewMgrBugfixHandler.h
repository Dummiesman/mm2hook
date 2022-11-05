#pragma once
#include "common.h"
#include "patch.h"
#include "mm2.h"

class mmViewMgrBugfixHandler {
public:
    void SetViewSetting_Dash(int a1);
    void SetViewSetting_Mirror(int a1);
    static void Install();
};