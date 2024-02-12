#pragma once
#include "vehinfo.h"

namespace MM2
{
    // Forward declarations
    class mmVehList;

    // External declarations


    // Class definitions
    class mmVehList {
    private:
        mmVehInfo **vehInfos;
        mmVehInfo *defaultVehicle;
        int numVehicles;
    public:
        ANGEL_ALLOCATOR

        AGE_API mmVehList(void) {
            scoped_vtable x(this);
            hook::Thunk<0x524550>::Call<void>(this);
        }

        virtual AGE_API ~mmVehList(void) {
            scoped_vtable x(this);
            hook::Thunk<0x524570>::Call<void>(this);
        }

        AGE_API void LoadAll()                               { hook::Thunk<0x524950>::Call<void>(this); }

        AGE_API mmVehInfo * GetVehicleInfo(int id)           { return hook::Thunk<0x5245E0>::Call<mmVehInfo *>(this, id); }
        AGE_API mmVehInfo * GetVehicleInfo(const char *basename)   
                                                             { return hook::Thunk<0x524610>::Call<mmVehInfo *>(this, basename); }
        AGE_API int GetVehicleID(const char* basename)       { return hook::Thunk<0x5246B0>::Call<int>(this, basename); }

        AGE_API void SetDefaultVehicle(const char* basename) { hook::Thunk<0x524690>::Call<void>(this, basename); }
        AGE_API void Print()                                 { hook::Thunk<0x524810>::Call<void>(this); }
        
        //helper
        inline int GetNumVehicles() {
            return this->numVehicles;
        }

        //lua
        static void BindLua(LuaState L) {
            LuaBinding(L).beginClass<mmVehList>("mmVehList")
                .addFunction("Print", &Print)
                .addPropertyReadOnly("NumVehicles", &GetNumVehicles)
                .addFunction("GetVehicleInfo", static_cast<mmVehInfo *(mmVehList::*)(const char*)>(&GetVehicleInfo))
                .addFunction("GetVehicleInfoByIndex", static_cast<mmVehInfo *(mmVehList::*)(int)>(&GetVehicleInfo))
                .addFunction("GetVehicleID", &GetVehicleID)
                .addFunction("SetDefaultVehicle", &SetDefaultVehicle)
                .endClass();
        }
    };

    declhook(0x6B1CA8, _Type<mmVehList *>, VehicleListPtr);
}