#pragma once
#include "common.h"
#include "patch.h"
#include "mm2.h"

#include "mmMirrorHandler.h"

class cityLevelBugfixHandler
{
public:
    static void UpdateRainParticles();
    static MM2::Stream* OpenPvsStream(const char* folder, const char* file, const char* extension, bool a4, bool a5);
    static void Install();
};