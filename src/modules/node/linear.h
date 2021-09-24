#pragma once

namespace MM2
{
    // Forward declarations
    class asLinearCS;

    // External declarations


    // Class definitions
    class asLinearCS : public asNode
    {
    private:
        Matrix34 matrix;
        Matrix34 camera;
        int global;
        asLinearCS* inherit;
    public:
        AGE_API asLinearCS() {
            scoped_vtable x(this);
            hook::Thunk<0x4A32F0>::Call<void>(this);
        };

        virtual AGE_API ~asLinearCS() {
            scoped_vtable x(this);
            hook::Thunk<0x431B80>::Call<void>(this);
        };

        /*
            asNode Virtuals
        */
        virtual AGE_API void Cull(void) override            { hook::Thunk<0x4A3400>::Call<void>(this); };
        virtual AGE_API void Update(void) override          { hook::Thunk<0x4A3370>::Call<void>(this); };

        //lua
        static void BindLua(LuaState L) {
            LuaBinding(L).beginExtendClass<asLinearCS, asNode>("asLinearCS")
                .endClass();
        }
    };

    ASSERT_SIZEOF(asLinearCS, 0x80);


    // Lua initialization

}