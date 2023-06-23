#pragma once
#include <modules\node.h>
#include "oppiconinfo.h"
#include "luaoppicons.h"

namespace MM2
{
    // Forward declarations
    class mmIcons;

    // External declarations
    extern class gfxTexture;

    // Class definitions
    class mmIcons : public asNode {
    private:
        gfxTexture* IconTexture;
        Vector3 TriangleVerts[4];
        Vector3 IconVerts[4];
        Vector2 IconUVs[4];
        int IconCount;
        float MinDistance2;
        float MaxDistance2;
        Matrix34* CameraMatrixPtr;
        OppIconInfo* IconInfo;
        float UVFraction;
        int IconMode;
        BOOL DiamondPointers;
    private:
        void registerOpponentsLua(LuaOppIconInfo& container, LuaRef fontRef);
    public:
        int GetIconCount() const;
        OppIconInfo* GetIcon(int index);

        AGE_API void RegisterOpponents(OppIconInfo* infos, int numInfos, LocFont* font);

        /*
            asNode virtuals
        */        
        AGE_API void Update() override;
        AGE_API void Cull() override;

        static void BindLua(LuaState L);
    };
    ASSERT_SIZEOF(mmIcons, 0xBC);
}