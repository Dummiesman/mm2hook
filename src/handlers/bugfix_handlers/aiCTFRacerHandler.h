#pragma once
#include "common.h"
#include "patch.h"
#include "mm2.h"

class aiCTFRacerHandler {
public:
    void Init(int id, char* basename);
    static void Install();
};