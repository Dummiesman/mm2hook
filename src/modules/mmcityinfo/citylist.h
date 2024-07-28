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
        mmCityInfo **m_CityInfos;
        int m_NumCities;
        int m_Current;
    public:
        ANGEL_ALLOCATOR

        AGE_API mmCityList(void) {
            scoped_vtable x(this);
            hook::Thunk<0x524160>::Call<void>(this);
        }

        virtual AGE_API ~mmCityList(void) {
            scoped_vtable x(this);
            hook::Thunk<0x524180>::Call<void>(this);
        }

        AGE_API void LoadAll()                                 { hook::Thunk<0x5244F0>::Call<void>(this); }
        AGE_API void Load(char* cinfoName)                     { hook::Thunk<0x524330>::Call<void>(this, cinfoName); }
        AGE_API int GetCityID(char const *basename)            { return hook::Thunk<0x524270>::Call<int>(this, basename); }

        AGE_API mmCityInfo * GetCityInfo(int id) const         { return hook::Thunk<0x5241F0>::Call<mmCityInfo *>(this, id); }
        AGE_API mmCityInfo * GetCityInfo(char const* basename) const
                                                               { return hook::Thunk<0x524220>::Call<mmCityInfo *>(this, basename); }

        AGE_API mmCityInfo * GetCurrentCity(void)              { return hook::Thunk<0x524320>::Call<mmCityInfo *>(this); }

        AGE_API void SetCurrentCity(char const* basename)      { hook::Thunk<0x5242C0>::Call<void>(this, basename); }
        AGE_API void Print()                                   { hook::Thunk<0x524420>::Call<void>(this); }

        int GetNumCities() const
        {
            return m_NumCities;
        }

        bool Exists(const char* basename) const
        {
            for (int i = 0; i < m_NumCities; i++)
            {
                auto info = GetCityInfo(i);
                if (!strcmp(info->GetRaceDir(), basename))
                {
                    return true;
                }
            }
            return false;
        }

        //lua
        static void BindLua(LuaState L) {
            LuaBinding(L).beginClass<mmCityList>("mmCityList")
                .addFunction("Print", &Print)
                .addPropertyReadOnly("NumCities", &GetNumCities)
                .addFunction("Exists", &Exists)
                .addFunction("GetCityInfo", static_cast<mmCityInfo * (mmCityList::*)(char const*)const>(&GetCityInfo))
                .addFunction("GetCityInfoByIndex", static_cast<mmCityInfo * (mmCityList::*)(int)const>(&GetCityInfo))
                .addFunction("GetCityID", &GetCityID)
                .addFunction("SetCurrentCity", &SetCurrentCity)
                .endClass();
        }
    };
    declhook(0x6B1CA0, _Type<mmCityList*>, CityListPtr);
}