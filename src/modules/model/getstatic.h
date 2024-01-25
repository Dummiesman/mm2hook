#pragma once
#include <mm2_common.h>

namespace MM2
{
    // Forward declarations

    // External declarations
    extern class modStatic;
    extern modStatic* modGetStatic(LPCSTR file, void(__cdecl* boundsCB)(Vector3*, void*) = nullptr, void* boundsCBParam = nullptr, bool a4 = false);
    extern modStatic* modGetStatic(LPCSTR file, float& radius, bool a3);

    // Class definitions

    // Lua initialization

}