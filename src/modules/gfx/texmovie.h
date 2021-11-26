#pragma once
#include "texture.h"

namespace MM2
{
    // Forward declarations
    class gfxTextureMovie;
    struct gfxTextureMovieClientList;

    // External declarations


    // Class definitions
    struct gfxTextureMovieClientList
    {
        gfxTexture** Textures;
        gfxTextureMovieClientList* Previous;
    };

    class gfxTextureMovie {
    public:
        const char* Name;
        gfxTextureMovie* Previous;
        gfxTexture** Frames;
        gfxTextureMovieClientList* ClientList;
        unsigned char FrameCount;
        unsigned char CurrentFrame;
        float CurrentTime;
        float FrameTime;
    };

    // Lua initialization

}