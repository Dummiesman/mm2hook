#pragma once
#include "common.h"
#include "patch.h"
#include "mm2.h"

class memSafeHeapHandler {
public:
    void Init(void* memAllocator, unsigned int heapSize, bool p3, bool p4, bool checkAlloc);
    void ShadowInit(void* pMemory, unsigned int size, bool a4, bool checkAlloc);

    static void Install();
};