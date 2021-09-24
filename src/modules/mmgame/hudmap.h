#pragma once
#include <modules\node.h>

namespace MM2
{
    // Forward declarations
    class mmHudMap;

    // External declarations

    // Class definitions
    class mmHudMap : public asNode {
    protected:
        AGE_API int GetCurrentMapMode()                     { return hook::Thunk<0x42EF20>::Call<int>(this); }
        AGE_API int GetNextMapMode()                        { return hook::Thunk<0x42EF00>::Call<int>(this); }
        AGE_API void SetMapMode(int a1)                     { hook::Thunk<0x42EF30>::Call<void>(this, a1); }
    public:
        AGE_API void Activate()                             { hook::Thunk<0x42EEE0>::Call<void>(this); }
        AGE_API void Deactivate()                           { hook::Thunk<0x42EEF0>::Call<void>(this); }
        AGE_API void SetOrient(bool a1)                     { hook::Thunk<0x42FA40>::Call<void>(this, a1); }
        AGE_API void SetZoomIn(bool a1)                     { hook::Thunk<0x42FA20>::Call<void>(this, a1); }
        AGE_API void ToggleMapOrient()                      { hook::Thunk<0x42FA10>::Call<void>(this); }
        AGE_API void ToggleMapRes()                         { hook::Thunk<0x42FA00>::Call<void>(this); }
        AGE_API bool GetOrient()                            { return hook::Thunk<0x42FA50>::Call<bool>(this); }
        AGE_API bool GetZoomIn()                            { return hook::Thunk<0x42FA30>::Call<bool>(this); }

        /*
            asNode virtuals
        */

        AGE_API void Cull() override                { hook::Thunk<0x42F1B0>::Call<void>(this); }
        AGE_API void Update() override              { hook::Thunk<0x42F1A0>::Call<void>(this); }
        AGE_API void Reset() override               { hook::Thunk<0x42EE90>::Call<void>(this); }
        AGE_API void FileIO(datParser &parser) override 
                                                    { hook::Thunk<0x42FA60>::Call<void>(this, &parser); }
        AGE_API char * GetClassName() override      { return hook::Thunk<0x42FD60>::Call<char*>(this); }

        static void BindLua(LuaState L) {
            LuaBinding(L).beginExtendClass<mmHudMap, asNode>("mmHudMap")
                .addFunction("Activate", &Activate)
                .addFunction("Deactivate", &Deactivate)
                .addFunction("SetOrient", &SetOrient)
                .addFunction("SetZoomIn", &SetZoomIn)
                .addFunction("ToggleMapOrient", &ToggleMapOrient)
                .addFunction("ToggleMapRes", &ToggleMapRes)
                .addFunction("GetOrient", &GetOrient)
                .addFunction("GetZoomIn", &GetZoomIn)
                .addFunction("GetCurrentMapMode", &GetCurrentMapMode)
                .addFunction("GetNextMapMode", &GetNextMapMode)
                .addFunction("SetMapMode", &SetMapMode)
            .endClass();  
        }
    };
}