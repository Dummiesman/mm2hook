#pragma once
#include <modules\level\inst.h>

namespace MM2
{
    // Forward declarations
    class gizInstance;

    // External declarations
    extern class gizPathspline;
    extern class dgPath;

    // Class definitions
    class gizInstance : public lvlInstance
    {
    private:
        Matrix34 Matrix;
        float CG_Y;
        char Variant;
    public:
        /*
            gizInstance 
        */
        AGE_API void Init(LPCSTR name, int loadFlags = 0)  { hook::Thunk<0x579E90>::Call<void>(this, name, loadFlags); }

        /*
            lvlInstance virtuals
        */

        virtual AGE_API void Draw(int a1) override          { hook::Thunk<0x579F30>::Call<void>(this, a1); }
        virtual AGE_API const Vector3 & GetPosition() override
                                                            { return hook::Thunk<0x579E30>::Call<const Vector3 &>(this); }
        virtual AGE_API const Matrix34 & GetMatrix(Matrix34 *a1) override
                                                            { return hook::Thunk<0x579E40>::Call<const Matrix34 &>(this, a1); }
        virtual AGE_API void SetMatrix(const Matrix34 & a1) override 
                                                            { hook::Thunk<0x579E60>::Call<void>(this, &a1); }

        static void BindLua(LuaState L) {
            LuaBinding(L).beginExtendClass<gizInstance, lvlInstance>("gizInstance")
                .endClass();
        }
    };
    ASSERT_SIZEOF(gizInstance, 0x4C);
}