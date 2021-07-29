#pragma once
#include "common.h"
#include "patch.h"
#include "mm2.h"

class memMemoryAllocatorHandler {
public:
    void* Allocate(unsigned int size, bool array);
    static void Install();
};