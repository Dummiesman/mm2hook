#include "getstatic.h"

using namespace MM2;

modStatic* MM2::modGetStatic(LPCSTR file, void(__cdecl* boundsCB)(Vector3*, void*), void* boundsCBParam, bool a4)
{
    return hook::StaticThunk<0x4A4E30>::Call<modStatic*>(file, boundsCB, boundsCBParam, a4);
}
modStatic* MM2::modGetStatic(LPCSTR file, float& radius, bool a3)
{
    return hook::StaticThunk<0x4A52E0>::Call<modStatic*>(file, &radius, a3);
}