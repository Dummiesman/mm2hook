#include "memMemoryAllocatorHandler.h"

using namespace MM2;

/*
    memMemoryAllocatorHandler
*/

void* memMemoryAllocatorHandler::Allocate(unsigned int size, bool array)
{
    void* memory = hook::Thunk<0x5767A0>::Call<void*>(this, size, array); //original
    memset(memory, 0x00, size); //cleanup
    return memory;
}

void memMemoryAllocatorHandler::Install()
{
    InstallCallback("operator new", "Null out allocated memory.",
        &Allocate, {
            cb::call(0x57736F),
        }
    );
    InstallCallback("operator new[]", "Null out allocated memory.",
        &Allocate, {
            cb::call(0x5773AF),
        }
    );
}