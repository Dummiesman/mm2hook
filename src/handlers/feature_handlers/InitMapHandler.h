#pragma once
#include "common.h"
#include "patch.h"
#include "mm2.h"

class InitMapHandler {
public:
    void AllocMemory(int start, int count);
    void LoadMapFile(LPCSTR file, int base, bool main);
    void InitMap();

    static void Install();
};

