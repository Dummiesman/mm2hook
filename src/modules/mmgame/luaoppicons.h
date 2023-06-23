#pragma once
#include <modules\vector.h>
#include "oppiconinfo.h"

namespace MM2
{
    // Forward declarations
    class LuaOppIconInfo;

    // External declarations
    extern class gfxBitmap;

    // Class definitions
	class LuaOppIconInfo {
    private:
        int iconCount;
        Matrix34* matrices;
        OppIconInfo* icons;
    public:
        ANGEL_ALLOCATOR

        LuaOppIconInfo(int numIcons)
        {
            this->iconCount = numIcons;
            this->matrices = new Matrix34[numIcons];
            this->icons = new OppIconInfo[numIcons];

            for (int i = 0; i < numIcons; i++)
            {
                icons[i].MatrixPtr = &matrices[i];
            }
        }

        int GetIconCount() const
        {
            return iconCount;
        }

        OppIconInfo* GetFirst()
        {
            return &icons[0];
        }

        OppIconInfo* GetIcon(int index)
        {
            if (index < 0 || index >= iconCount) return nullptr;
            return &icons[index];
        }

        void UpdateMatrix(int index, Matrix34 const& matrix)
        {
            if (index < 0 || index >= iconCount) return;
            matrices[index].Set(matrix);
        }

        static void BindLua(LuaState L) {
            LuaBinding(L).beginClass<LuaOppIconInfo>("OppIconInfoContainer")
                .addFactory([](int iconCount) {
                    return new LuaOppIconInfo(iconCount);
                })
                .addFunction("GetIcon", &GetIcon)
                .addFunction("UpdateMatrix", &UpdateMatrix)
                .endClass();
        }
    };
}

