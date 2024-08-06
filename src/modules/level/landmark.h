#pragma once
#include "fixed.h"

namespace MM2
{
    // Forward declarations


    // External declarations
    extern class fxWaterSpout;
    
    // Class definitions
    class lvlLandmark : public lvlFixedRotY
    {
    private:
        fxWaterSpout* m_Spout;
    public:
        /*
            lvlInstance virtuals
        */

        virtual AGE_API int IsVisible(gfxViewport const& viewport) override
                                                                    { return hook::Thunk<0x468030>::Call<int>(this, &viewport); }

        virtual AGE_API int Init(const char* a1, const Matrix34& a2, int a3) override;
        virtual AGE_API void DrawGlow() override;

        virtual AGE_API unsigned int SizeOf(void) override          { return sizeof(lvlLandmark); }

        virtual AGE_API bool IsLandmark() override                  { return hook::Thunk<0x467FF0>::Call<bool>(this); }
        virtual AGE_API bool IsCollidable() override                { return hook::Thunk<0x465EB0>::Call<bool>(this); }

        static void BindLua(LuaState L) {
            LuaBinding(L).beginExtendClass<lvlLandmark, lvlFixedRotY>("lvlLandmark")
                .addFactory([]() {
                return new lvlLandmark();
                })
                .endClass();
        }
    };

    //ASSERT_SIZEOF(lvlLandmark, 0x28);
    ASSERT_SIZEOF(lvlLandmark, 0x2C);
}