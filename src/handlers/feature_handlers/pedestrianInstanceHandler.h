#pragma once
#include "common.h"
#include "patch.h"
#include "mm2.h"

class pedestrianInstanceHandler {
public:
    void DrawRagdoll();
    void AnimationInstance_Draw(bool a1);
    void AnimationInstance_Update();
    void Draw(int a1);
    void DrawShadow();
    void Detach();
    bool IsCollidable();
    MM2::phBound * GetBound(int a1);
    void FirstImpactCallback();

    void aiMapClearPeds(MM2::aiPath* path);
    void aiMapClean();
    void aiMapInit(char* a1, char* a2, char* a3, const MM2::dgStatePack* a4, int a5, MM2::vehCar* a6, bool a7);
    void Init(char* a1, int a2, int a3);

    static void Install();
};

