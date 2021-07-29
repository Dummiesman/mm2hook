#pragma once
#include "common.h"
#include "patch.h"
#include "mm2.h"

class mmGameMusicDataHandler {
public:
    bool LoadAmbientSFX(LPCSTR name);

    static void Install();
};