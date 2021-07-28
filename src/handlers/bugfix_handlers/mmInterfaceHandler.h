#pragma once
#include "common.h"
#include "patch.h"
#include "mm2.h"

/// <summary>
/// Fixes a crash when the selected vehicle is missing
/// </summary>
class mmInterfaceHandler {
public:
    void PlayerResolveCars();
    static void Install();
};