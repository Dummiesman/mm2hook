#pragma once
#include "common.h"
#include "patch.h"
#include "mm2.h"

class StreamHandler {
public:
    static MM2::Stream * Open(const char *filename, bool readOnly);

    static void Install();
};

