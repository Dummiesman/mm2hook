#pragma once
#include "common.h"
#include "patch.h"
#include "mm2.h"

class ImGuiFeatureHandler {
public:
    static void EndPhase();
    static void BeginPhase(bool a1);
	void MenuUpdateHook();
    static void Install();
};

