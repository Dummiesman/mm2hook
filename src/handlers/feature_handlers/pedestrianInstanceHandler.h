#pragma once
#include "common.h"
#include "patch.h"
#include "mm2.h"

class pedestrianInstanceHandler {
public:
    void DrawRagdoll();
    void Draw(int a1);
    void Detach();
    bool IsCollidable();
    MM2::phBound * GetBound(int a1);

    void aiMapClean();
    void aiMapInit(char* a1, char* a2, char* a3, const MM2::dgStatePack* a4, int a5, MM2::vehCar* a6, bool a7);

    static void Install();
};

