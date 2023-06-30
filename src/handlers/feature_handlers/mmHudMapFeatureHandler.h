#pragma once
#include "common.h"
#include "patch.h"
#include "mm2.h"

using namespace MM2;

class mmHudMapFeatureHandler {
public:
    void DrawColoredTri(unsigned int color, const Matrix34& a2);
    void DrawWhiteTri(const Matrix34& a1);
    void DrawLightOrangeTri(const Matrix34& a1);
    void DrawLightGreenTri(const Matrix34& a1);
    
    void DrawIcon(int iconType, const Matrix34& matrix);
    void DrawOutlinedIcon(int iconType, const Matrix34& matrix);
    void DrawOutlinedIcon(int iconType, int iconOutlineType, const Matrix34& matrix);

    void DrawCustomIcon(int iconType, unsigned int color, const Matrix34& matrix);

    void DrawNfsMwPlayerIcon(const Matrix34& matrix);
    void DrawNfsMwOpponentIcon(const Matrix34& matrix);
    void DrawPlayer();
    void DrawCops();
    void DrawOpponents();

    static void Install();
};

