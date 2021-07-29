#pragma once
#include "common.h"
#include "patch.h"
#include "mm2.h"

class vehCarAudioContainerHandler {
public:
    static void SetSirenCSVName(LPCSTR name);

    static void Install();
};