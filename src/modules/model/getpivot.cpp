#include "getpivot.h"

using namespace MM2;

bool MM2::GetPivot(Matrix34& mtx, LPCSTR basename, LPCSTR file) {
    return hook::StaticThunk<0x4A5280>::Call<bool>(&mtx, basename, file);
}
