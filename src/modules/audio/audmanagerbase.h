#pragma once
#include <modules\node\node.h>

namespace MM2
{
    // Forward declarations
    class AudManagerBase;

    // External declarations

    // Class definitions
    class AudManagerBase : public asNode {
    private:
            //lua helpers
            bool luaGetIsStereo() {
                return IsStereo() == TRUE;
            }
    public:
        AGE_API AudManagerBase() {
            scoped_vtable x(this);
            hook::Thunk<0x50EE10>::Call<void>(this);
        };

        AGE_API ~AudManagerBase() {
            scoped_vtable x(this);
            hook::Thunk<0x50EE40>::Call<void>(this);
        };

        //instance
        static hook::Type<AudManagerBase*> Instance;

        //members
        AGE_API BOOL IsStereo()                         { return hook::Thunk<0x50F0D0>::Call<BOOL>(this); }
        AGE_API float RandomizeNumber(float min, float max)
                                                        { return hook::Thunk<0x50F290>::Call<float>(this, min, max); }
        AGE_API float RandomizeNumber(float max)        { return hook::Thunk<0x50F230>::Call<float>(this, max); }
                                                    

        //asNode overrides
        AGE_API virtual void Update() override          { hook::Thunk<0x50F130>::Call<void>(this); }
        AGE_API virtual void UpdatePaused() override    { hook::Thunk<0x50F1A0>::Call<void>(this); }

        static void BindLua(LuaState L) {
            LuaBinding(L).beginExtendClass<AudManagerBase, asNode>("AudManagerBase")
                .addStaticFunction("Instance", [] { return (AudManagerBase*)Instance; })
                .addPropertyReadOnly("IsStereo", &luaGetIsStereo)
            .endClass();
        }
    };

    declhook(0x6B1474, _Type<AudManagerBase*>, AUDMGRPTR);
}