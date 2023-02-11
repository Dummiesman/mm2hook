#pragma once
#include <modules\data\base.h>
#include <modules\parse.h>

namespace MM2
{
    // Forward declarations
    class mmInfoBase;

    // External declarations


    // Class definitions
    class mmInfoBase : public Base {
    private:
        char IOPath[128];
        int Something1234ByDefault;
    public:
        AGE_API mmInfoBase(void) {
            scoped_vtable x(this);
            hook::Thunk<0x528230>::Call<void>(this);
        }

        virtual AGE_API ~mmInfoBase(void) {
            scoped_vtable x(this);
            hook::Thunk<0x41AB90>::Call<void>(this);
        }

        // mmInfoBase virtuals
        virtual void FileIO(datParser& parser) {
            hook::Thunk<0x41AB50>::Call<void>(this, &parser);
        }

        // mmInfoBase members
        BOOL Load(const char* a2)
        {
            return hook::Thunk<0x528260>::Call<BOOL>(this, a2);
        }

        BOOL Save(const char* a2)
        {
            return hook::Thunk<0x528310>::Call<BOOL>(this, a2);
        }

        void SetIOPath(const char* path)
        {
            hook::Thunk<0x5283C0>::Call<void>(this, path);
        }

        //lua
        static void BindLua(LuaState L) {
            LuaBinding(L).beginExtendClass<mmInfoBase, Base>("mmInfoBase")
                .addFunction("Load", &Load)
                .addFunction("Save", &Save)
                .addFunction("SetIOPath", &SetIOPath)
                .endClass();
        }
    };
    ASSERT_SIZEOF(mmInfoBase, 0x88);
}