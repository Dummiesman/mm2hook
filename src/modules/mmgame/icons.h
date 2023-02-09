#pragma once
#include <modules\node.h>
#include "oppiconinfo.h"

namespace MM2
{
    // Forward declarations
    class mmIcons;

    // External declarations

    // Class definitions
    class mmIcons : public asNode {
    private:
        byte _buffer[0xA4];
    private:
        static hook::Field<0x9C, int> _iconCount;
        static hook::Field<0xAC, OppIconInfo*> _icons;
    public:
        int GetIconCount() const { return _iconCount.get(this); }
        OppIconInfo* GetIcon(int index) {
            if (index < 0 || index >= GetIconCount())
                return nullptr;

            auto icons = _icons.get(this); 
            if (icons == nullptr)
                return nullptr;
            
            return &icons[index];
        }

        AGE_API void RegisterOpponents(OppIconInfo* infos, int numInfos, void* font)
                                                    { hook::Thunk<0x4322F0>::Call<void>(this, infos, numInfos, font); }

        /*
            asNode virtuals
        */        
        AGE_API void Update() override              { hook::Thunk<0x432390>::Call<void>(this); }
        AGE_API void Cull() override                { hook::Thunk<0x4323D0>::Call<void>(this); }

        static void BindLua(LuaState L) {
            LuaBinding(L).beginExtendClass<mmIcons, asNode>("mmIcons")
                .addPropertyReadOnly("NumIcons", &GetIconCount)
                .addFunction("GetIcon", &GetIcon)
                .endClass();
        }
    };
    ASSERT_SIZEOF(mmIcons, 0xBC);
}