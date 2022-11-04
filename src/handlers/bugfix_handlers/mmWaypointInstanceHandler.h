#pragma once
#include "common.h"
#include "patch.h"
#include "mm2.h"

class mmWaypointInstanceHandler
{
public:
    void Draw(int lod);
    static void Install();
};