#pragma once
#include <modules\core\stream.h>

namespace MM2
{
    // Forward declarations
    class modPackage;

    // External declarations


    // Class definitions
    class modPackage {
    private:
        modPackage *next;
        Stream *stream;
        char file[32];
        uint magic;
        uint fileSize;
    public:
        ANGEL_ALLOCATOR 

        AGE_API modPackage(void) {
            scoped_vtable x(this);
            hook::Thunk<0x4A46D0>::Call<void>(this);
        }

        AGE_API ~modPackage(void) {
            scoped_vtable x(this);
            hook::Thunk<0x4A46F0>::Call<void>(this);
        }

        AGE_API bool Open(LPCSTR dir, LPCSTR filename)          { return hook::Thunk<0x4A4700>::Call<bool>(this, dir, filename); }
        AGE_API void Close(void)                                { return hook::Thunk<0x4A4790>::Call<void>(this); }
        AGE_API Stream * OpenFile(LPCSTR file)                  { return hook::Thunk<0x4A47B0>::Call<Stream *>(this, file); }
        AGE_API void CloseFile(void)                            { return hook::Thunk<0x4A4800>::Call<void>(this); }
        AGE_API void SkipTo(LPCSTR file)                        { return hook::Thunk<0x4A48D0>::Call<void>(this, file); }
        AGE_API void Skip(void)                                 { return hook::Thunk<0x4A4970>::Call<void>(this); }

        AGE_API const char* GetCurrentFileName() const
        {
            return file;
        }
    };

    // Lua initialization

}