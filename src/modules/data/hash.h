#pragma once
#include <mm2_common.h>

namespace MM2
{
    // Forward declarations
    class HashTable;

    // External declarations


    // Class definitions
    class HashTable 
    {
    private:
        byte _buffer[0x18];
    public:
        AGE_API void* Access(char const* key)                            { return hook::Thunk<0x4C6CA0>::Call<void*>(this, key); }
        AGE_API bool Access(char const* key, void* out)                  { return hook::Thunk<0x4C6D10>::Call<bool>(this, key, out); }                        
        AGE_API bool Insert(char const* key, void* data)                 { return hook::Thunk<0x4C69F0>::Call<bool>(this, key, data); }
        AGE_API char const* AccessName(int entryNum)                     { return hook::Thunk<0x4C6D80>::Call<char const*>(this, entryNum); }
        AGE_API void* AccessData(int entryNum)                           { return hook::Thunk<0x4C6DA0>::Call<void*>(this, entryNum); }
        AGE_API bool Change(char const* oldKey, char const* newKey)      { return hook::Thunk<0x4C6C60>::Call<bool>(this, oldKey, newKey); }
        AGE_API bool Delete(char const* key)                             { return hook::Thunk<0x4C6B20>::Call<bool>(this, key); }
    };

    ASSERT_SIZEOF(HashTable, 0x18);

    // Lua initialization

}