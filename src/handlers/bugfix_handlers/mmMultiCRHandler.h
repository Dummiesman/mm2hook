#pragma once
#include "common.h"
#include "patch.h"
#include "mm2.h"

class mmMultiCRHandler {
public:
    void GoldRotate(MM2::Vector3* axis, float amount);
    bool LoadMusic(char* a1, char* a2);
    void LoadPositions(const char* a1);
    static void Install();
};