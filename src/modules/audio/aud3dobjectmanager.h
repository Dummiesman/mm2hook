#pragma once
#include <mm2_common.h>
#include <modules\node\node.h>

namespace MM2
{
    // Forward declarations
    class Aud3DObjectManager;

    // External declarations

    // Class definitions
    class Aud3DObjectManager : public asNode {
    private:
        byte _buffer[0x180];
    public:
        ANGEL_ALLOCATOR

        /*
            Aud3DObjectManager
        */
        int AllocateSample(const char* name, int flags)
        {
            return hook::Thunk<0x50FCB0>::Call<int>(this, name, flags);
        }

        AudSoundBase* GetSample(int objectId, int sampleIndex)
        {
            return hook::Thunk<0x50F9F0>::Call<AudSoundBase*>(this, objectId, sampleIndex);
        }

        
        static void BindLua(LuaState L) {
            LuaBinding(L).beginClass<Aud3DObjectManager>("Aud3DObjectManager")
                .addFunction("AllocateSample", &AllocateSample)
                .addFunction("GetSample", &GetSample)
                .endClass();
        }
    };

    declhook(0x6B1494, _Type<Aud3DObjectManager*>, AUD3DOBJMGRPTR);
    ASSERT_SIZEOF(Aud3DObjectManager, 0x198);
}