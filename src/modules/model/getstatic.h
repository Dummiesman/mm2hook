#pragma once
#include <mm2_common.h>

namespace MM2
{
    // Forward declarations

    // External declarations
    extern class modStatic;
    AGE_API modStatic * modGetStatic(LPCSTR file, void(__cdecl* boundsCB)(Vector3*, void*), void* boundsCBParam, bool a4);
    AGE_API modStatic* modGetStatic(LPCSTR file, float& radius, bool a3);

    // Class definitions

    // Lua initialization

}