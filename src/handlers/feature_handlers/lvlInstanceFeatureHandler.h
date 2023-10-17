#pragma once
#include "common.h"
#include "patch.h"
#include "mm2.h"

class lvlInstanceFeatureHandler {
public:
    unsigned int lvlLandmark_SizeOf();
    void lvlLandmark_DrawGlow();
    int lvlLandmark_Init(const char* a1, const MM2::Matrix34& a2, int a3);

    static void Install();
};

