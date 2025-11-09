#pragma once
#include "common.h"
#include "patch.h"
#include "mm2.h"

/// <summary>
/// Fixes a crash when the selected vehicle is missing
/// </summary>
class mmInterfaceFeatureHandler {
public:
    void ShowMain(int a2);
    void Destruct();
    int ShowcaseMenuStateHook(int menuId);
    static void Install();
};