#pragma once

namespace MM2
{
    // Forward declarations
    class gfxImage;
    class gfxTexture;
    class gfxBitmap;
    class gfxTextureCacheEntry;
    class gfxTextureCachePool;

    // External declarations


    // Class definitions
    class gfxTexture {
    public:
        /*0x00*/uint BindIndex;

        /*0x04*/const char *Name;

        /*0x08*/ushort Width;
        /*0x0A*/ushort Height;

        /*0x0C*/uint TexEnv;
        /*0x10*/uint DominantColor;

        /*0x14*/IDirectDrawSurface7 *DirectDrawSurface;
        /*0x18*/IDirectDrawPalette *DirectDrawPalette;

        /*0x1C*/gfxTextureCacheEntry *CacheEntry;
        /*0x20*/gfxTextureCachePool *CachePool;

        /*0x24*/uint RefCount;

        /*0x28*/gfxTexture *Next;
        /*0x2C*/gfxTexture *NextLOD;

        /*0x30*/byte LOD;
        /*0x31*/byte MaxLOD;

        static hook::Type<bool> sm_EnableSetLOD;
        static hook::Type<bool> sm_Allow32;
        static hook::Type<gfxTexture*> sm_First;
        static hook::Type<bool> sm_UseInternalCache;
    public:
        const char * getName() {
            return this->Name;
        }

        int getWidth() {
            return this->Width;
        }

        int getHeight() {
            return this->Height;
        }

        gfxTextureCachePool * getPool() {
            return this->CachePool;
        }

        gfxTextureCacheEntry * getCacheEntry() {
            return this->CacheEntry;
        }

        gfxTexture * getNext() {
            return this->Next;
        }

        gfxTexture * getNextLOD() {
            return this->NextLOD;
        }
    public:
        ANGEL_ALLOCATOR

        AGE_API gfxTexture()
        {
            hook::Thunk<0x4AC8E0>::Call<void>(this);
        }

        AGE_API ~gfxTexture()
        {
            hook::Thunk<0x4AC910>::Call<void>(this);
        }

        AGE_API gfxTexture * Clone()
        {
            return hook::Thunk<0x4AE250>::Call<gfxTexture*>(this);
        }

        AGE_API void Blit(int destX, int destY, gfxTexture *src, int srcX, int srcY, int width, int height)
        {
            return hook::Thunk<0x4AE1E0>::Call<void>(this, destX, destY, src, srcX, srcY, width, height);
        }

        AGE_API static gfxTexture * Create(gfxImage* img, bool lastMip) 
        {
            return hook::StaticThunk<0x4AD090>::Call<gfxTexture*>(img, lastMip);
        }

        AGE_API static gfxTexture* CreateRenderTarget(int width, int height)
        {
            return hook::StaticThunk<0x4ADF00>::Call<gfxTexture*>(width, height);
        }

        static void BindLua(LuaState L) {
            LuaBinding(L).beginClass<gfxTexture>("gfxTexture")
                .addStaticFunction("CreateRenderTarget", &CreateRenderTarget)

                .addStaticProperty("First", [] { return sm_First.get(); })
                .addStaticProperty("UseInternalCache", [] { return sm_UseInternalCache.get(); })

                .addFunction("Clone", &Clone)

                .addPropertyReadOnly("Name", &getName)
                .addPropertyReadOnly("Next", &getNext)
                .addPropertyReadOnly("NextLOD", &getNextLOD)

                .addPropertyReadOnly("Width", &getWidth)
                .addPropertyReadOnly("Height", &getHeight)
                .addPropertyReadOnly("CacheEntry", &getCacheEntry)
                .addPropertyReadOnly("CachePool", &getPool)

                .endClass();
        }
    };

    class gfxBitmap {
    public:
        const char* Name;
        uint16_t Width;
        uint16_t Height;
        IDirectDrawSurface7* pSurface;
        uint32_t RefCount;
        gfxBitmap* Next;
    public:
        const char* getName() {
            return this->Name;
        }

        int getWidth() {
            return this->Width;
        }

        int getHeight() {
            return this->Height;
        }
    public:
        ANGEL_ALLOCATOR

        static void BindLua(LuaState L) {
        LuaBinding(L).beginClass<gfxBitmap>("gfxBitmap")
            .addPropertyReadOnly("Name", &getName)

            .addPropertyReadOnly("Width", &getWidth)
            .addPropertyReadOnly("Height", &getHeight)

            .endClass();
    }
    };

    class gfxTextureCacheEntry {
    public:
        gfxTexture* pTexture;
        IDirectDrawSurface7* pSurface;
        uint32_t LastAccessTime;
        gfxTextureCacheEntry* Next;
    public:
        inline gfxTexture* getTexture() {
            return this->pTexture;
        }

        inline gfxTextureCacheEntry* getNext() {
            return this->Next;
        }

        static void BindLua(LuaState L) {
            LuaBinding(L).beginClass<gfxTextureCacheEntry>("gfxTextureCacheEntry")
                .addPropertyReadOnly("NextEntry", &getNext)
                .addPropertyReadOnly("Texture", &getTexture)

                .endClass();
        }
    };

    class gfxTextureCachePool {
    public:
        uint16_t Width;
        uint16_t Height;
        uint16_t MipMapCount;
        uint16_t TextureCount;
        uint16_t EntryCount;
        uint16_t HasNoSurface;
        gfxTextureCacheEntry* First;
        gfxTextureCachePool* Next;
        DDPIXELFORMAT PixelFormat;
    public:
        inline int getMipMapCount() {
            return this->MipMapCount;
        }

        inline int getWidth() {
            return this->Width;
        }

        inline int getHeight() {
            return this->Height;
        }

        inline int getTextureCount() {
            return this->TextureCount;
        }

        inline int getEntryCount() {
            return this->EntryCount;
        }

        inline gfxTextureCachePool * getNext() {
            return this->Next;
        }

        inline gfxTextureCacheEntry* getFirst() {
            return this->First;
        }

        static hook::Type<gfxTextureCachePool*> sm_FirstPool;

        static void BindLua(LuaState L) {
            LuaBinding(L).beginClass<gfxTextureCachePool>("gfxTextureCachePool")
                .addStaticProperty("FirstPool", [] { return sm_FirstPool.get(); })

                .addPropertyReadOnly("FirstEntry", &getFirst)
                .addPropertyReadOnly("NextPool", &getNext)

                .addPropertyReadOnly("Width", &getWidth)
                .addPropertyReadOnly("Height", &getHeight)
                .addPropertyReadOnly("MipMapCount", &getMipMapCount)
                .addPropertyReadOnly("EntryCount", &getEntryCount)
                .addPropertyReadOnly("TextureCount", &getTextureCount)

                .endClass();
        }
    };

    // Lua initialization

}