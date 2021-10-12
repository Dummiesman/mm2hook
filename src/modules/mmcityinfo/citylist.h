#pragma once
#include "cityinfo.h"

namespace MM2
{
    // Forward declarations
    class mmCityList;

    // External declarations


    // Class definitions
    class mmCityList {
    private:
        mmCityInfo **cityInfos;
        int numCities;
        int curCity;
    public:
        AGE_API mmCityList(void) {
            scoped_vtable x(this);
            hook::Thunk<0x524160>::Call<void>(this);
        }

        virtual AGE_API ~mmCityList(void) {
            scoped_vtable x(this);
            hook::Thunk<0x524180>::Call<void>(this);
        }

        AGE_API void LoadAll()                              { hook::Thunk<0x5244F0>::Call<void>(this); }
        AGE_API void Load(char* cinfoName)                  { hook::Thunk<0x524330>::Call<void>(this, cinfoName); }
        AGE_API int GetCityID(char const *city)             { return hook::Thunk<0x524270>::Call<int>(this, city); }

        AGE_API mmCityInfo * GetCityInfo(int city)          { return hook::Thunk<0x5241F0>::Call<mmCityInfo *>(this, city); }
        AGE_API mmCityInfo * GetCityInfo(char const* city)  { return hook::Thunk<0x524220>::Call<mmCityInfo *>(this, city); }

        AGE_API mmCityInfo * GetCurrentCity(void)           { return hook::Thunk<0x524320>::Call<mmCityInfo *>(this); }

        AGE_API void SetCurrentCity(char const* city)       { hook::Thunk<0x5242C0>::Call<void>(this, city); }
        AGE_API void Print()                                { hook::Thunk<0x524420>::Call<void>(this); }

        //helper
        inline int GetNumCities() {
            return this->numCities;
        }

        //lua
        static void BindLua(LuaState L) {
            LuaBinding(L).beginClass<mmCityList>("mmCityList")
                .addFunction("Print", &Print)
                .addPropertyReadOnly("NumCities", &GetNumCities)
                .addFunction("GetCityInfo", static_cast<mmCityInfo * (mmCityList::*)(char const*)>(&GetCityInfo))
                .addFunction("GetCityInfoByIndex", static_cast<mmCityInfo * (mmCityList::*)(int)>(&GetCityInfo))
                .addFunction("GetCityID", &GetCityID)
                .addFunction("SetCurrentCity", &SetCurrentCity)
                .endClass();
        }
    };
    declhook(0x6B1CA0, _Type<mmCityList*>, CityListPtr);
}