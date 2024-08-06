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
        mmVehInfo** m_VehInfos;
        mmVehInfo*  m_DefaultVehicle;
        int m_NumVehicles;
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

        AGE_API mmVehInfo * GetVehicleInfo(int id) const     { return hook::Thunk<0x5245E0>::Call<mmVehInfo *>(this, id); }
        AGE_API mmVehInfo * GetVehicleInfo(const char *basename) const  
                                                             { return hook::Thunk<0x524610>::Call<mmVehInfo *>(this, basename); }
        AGE_API int GetVehicleID(const char* basename)       { return hook::Thunk<0x5246B0>::Call<int>(this, basename); }

        AGE_API void SetDefaultVehicle(const char* basename) { hook::Thunk<0x524690>::Call<void>(this, basename); }
        AGE_API void Print()                                 { hook::Thunk<0x524810>::Call<void>(this); }
        
        mmVehInfo* GetDefaultVehicle() const
        {
            return m_DefaultVehicle;
        }

        bool Exists(const char* basename) const
        {
            for (int i = 0; i < m_NumVehicles; i++)
            {
                auto info = GetVehicleInfo(i);
                if (!strcmp(info->GetBaseName(), basename))
                {
                    return true;
                }
            }
            return false;
        }

        int GetNumVehicles() 
        {
            return m_NumVehicles;
        }

        //lua
        static void BindLua(LuaState L) {
            LuaBinding(L).beginClass<mmVehList>("mmVehList")
                .addPropertyReadOnly("NumVehicles", &GetNumVehicles)
                .addPropertyReadOnly("DefaultVehicle", &GetDefaultVehicle)
                .addFunction("Print", &Print)
                .addFunction("Exists", &Exists)
                .addFunction("GetVehicleInfo", static_cast<mmVehInfo* (mmVehList::*)(const char*) const>(&GetVehicleInfo))
                .addFunction("GetVehicleInfoByIndex", static_cast<mmVehInfo* (mmVehList::*)(int) const>(&GetVehicleInfo))
                .addFunction("GetVehicleID", &GetVehicleID)
                .addFunction("SetDefaultVehicle", &SetDefaultVehicle)
                .endClass();
        }
    };

    declhook(0x6B1CA8, _Type<mmVehList *>, VehicleListPtr);
}