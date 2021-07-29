#pragma once
#include "common.h"
#include "patch.h"
#include "mm2.h"

class mmHudMapFeatureHandler {
public:
    void DrawColoredTri(unsigned int color, const MM2::Matrix34 *a2);
    void DrawWhiteTri(const MM2::Matrix34 *a1);
    void DrawLightOrangeTri(const MM2::Matrix34 *a1);
    void DrawLightGreenTri(const MM2::Matrix34 *a1);
    void DrawIcon(int iconType, const MM2::Matrix34 *matrix);
    void DrawNfsMwPlayerIcon(const MM2::Matrix34 *matrix);
    void DrawNfsMwOpponentIcon(const MM2::Matrix34 *matrix);
    void DrawPlayer();
    void DrawCops();
    void DrawOpponents();

    static void Install();
};

