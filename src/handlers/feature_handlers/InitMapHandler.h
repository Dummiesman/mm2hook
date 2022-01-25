#pragma once
#include "common.h"
#include "patch.h"
#include "mm2.h"

class InitMapHandler {
public:
    void CopyToMM2();
    void LoadMapFile(LPCSTR file, unsigned int base, bool main);
    void InitMap();

    static void Install();
};

