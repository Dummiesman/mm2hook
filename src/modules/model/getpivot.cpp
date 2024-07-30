#include "getpivot.h"

bool MM2::GetPivot(Matrix34& mtx, const char* basename, const char* file) 
{
    return hook::StaticThunk<0x4A5280>::Call<bool>(&mtx, basename, file);
}
