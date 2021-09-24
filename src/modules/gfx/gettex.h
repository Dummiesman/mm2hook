#pragma once

namespace MM2
{
    // Forward declarations

    // External declarations
    
    // Statically available functions
    declhook(0x5CD38C, _Type<gfxImage* (*)(const char*, bool)>, gfxLoadImage);
    declhook(0x5CD680, _Type<gfxImage* (*)(gfxImage*, const char*, bool)>, gfxPrepareImage);

    static gfxTexture * gfxGetTexture(const char* a1, bool a2 = true) {
        return hook::StaticThunk<0x4B30F0>::Call<gfxTexture*>(a1, a2);
    }

    // Class definitions

    // Lua initialization

}