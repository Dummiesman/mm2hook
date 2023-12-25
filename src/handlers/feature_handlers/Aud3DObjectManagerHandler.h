#pragma once
#include "common.h"
#include "patch.h"
#include "mm2.h"

class Aud3DObjectManagerHandler {
public:
    void InitAmbObjContainer(LPCSTR name);
    void Construct(int maxSounds);

    static void Install();
};