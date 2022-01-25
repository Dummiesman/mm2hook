#pragma once

namespace MM2
{
    // Forward declarations

    // External declarations
    
    // Statically available functions
    declhook(0x5CD38C, _Type<gfxImage* (*)(const char*, bool)>, gfxLoadImage);

    static bool gfxSaveTargaImage(const char * filename, gfxImage * image)
    {
        return hook::StaticThunk<0x4AF4E0>::Call<bool>(filename, image);
    }

    static bool gfxSaveBmpImage(const char * filename, gfxImage * image)
    {
        return hook::StaticThunk<0x4AFAB0>::Call<bool>(filename, image);
    }
    

    // Class definitions

    // Lua initialization

}