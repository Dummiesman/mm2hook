#pragma once
#include <modules\node.h>

namespace MM2
{
    // Forward declarations
    class mmGameMusicData;

    // External declarations


    // Class definitions
    class mmGameMusicData {
    public:
        AGE_API bool LoadAmbientSFX(LPCSTR name)            { return hook::Thunk<0x434060>::Call<bool>(this, name); }
    };
}