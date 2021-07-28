#pragma once
#include "common.h"
#include "patch.h"
#include "mm2.h"

class mmBillInstanceHandler
{
public:
    void Scale(float x, float y, float z);
    static void Install();
};