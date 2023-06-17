#pragma once
#include <mm2_common.h>
#include <modules\core\stream.h>

namespace MM2
{
    // Forward declarations
    class CArrayList;

    // External declarations


    // Class definitions

    class CArrayList {
    private:
        short count;
        short *data;
    public:
        CArrayList(void) {
            count = 0;
            data = nullptr;
        }

        ~CArrayList(void) {
            count = 0;
            data = nullptr;
        }

        int Count() const {
            return count;
        }

        short Get(int index) const {
            return data[index];
        }

        AGE_API void Append(int value)                      { hook::Thunk<0x543D50>::Call<void>(this, value); }

        AGE_API void ReadBinary(Stream *stream)             { hook::Thunk<0x543DE0>::Call<void>(this, stream); }
        AGE_API void SaveBinary(Stream *stream)             { hook::Thunk<0x543DB0>::Call<void>(this, stream); }
    };
}