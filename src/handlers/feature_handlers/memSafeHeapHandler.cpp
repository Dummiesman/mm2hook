#include "memSafeHeapHandler.h"

using namespace MM2;

static ConfigProperty cfgHeapSize("HeapSize", "heapsize");

/*
    memSafeHeapHandler
*/

int g_heapSize = 128;
char g_shadowHeap[65535];

void memSafeHeapHandler::Init(void* memAllocator, unsigned int heapSize, bool p3, bool p4, bool checkAlloc) {
    // fast way of expanding to the proper size
    // same as ((g_heapSize * 1024) * 1024)
    heapSize = (g_heapSize << 20);

    LogFile::Format("[memSafeHeap::Init]: Allocating %dMB heap (%d bytes)\n", g_heapSize, heapSize);
    return hook::Thunk<0x577210>::Call<void>(this, memAllocator, heapSize, p3, p4, checkAlloc); //TODO: move to own class
}

void memSafeHeapHandler::ShadowInit(void* pMemory, unsigned int size, bool a4, bool checkAlloc)
{
    // By default MM2 inits a shadow heap of 8K, but it's (current year) and we need more (ZIP files are allocated here)
    size = sizeof(g_shadowHeap);
    pMemory = &g_shadowHeap;
    hook::Thunk<0x576730>::Call<void>(this, pMemory, size, a4, checkAlloc);
}

void memSafeHeapHandler::Install() {
    cfgHeapSize.Get(g_heapSize);

    InstallCallback("memSafeHeap::Init", "Adds '-heapsize' parameter that takes a size in megabytes. Defaults to 128MB.",
        &Init, {
            cb::call(0x4015DD),
        }
    );
    InstallCallback("memMemoryAllocator::Init", "Expands the shadow heap.",
        &ShadowInit, {
            cb::call(0x4023AB),
        }
    );
}