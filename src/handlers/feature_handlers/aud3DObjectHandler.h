#pragma once
#include "common.h"
#include "patch.h"
#include "mm2.h"

class aud3DObjectHandler {
public:
    float CalculateDoppler(float speed);

    static void Install();
};