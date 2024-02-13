#include "aiVehicleMGR.h"

namespace MM2
{
    /*
        aiVehicleActive
    */



    /*
        aiVehicleManager
    */

    AGE_API aiVehicleManager::aiVehicleManager(void)
    {
        scoped_vtable x(this);
        hook::Thunk<0x553B30>::Call<void>(this);
    }

    AGE_API aiVehicleManager::~aiVehicleManager(void)
    {
        scoped_vtable x(this);
        hook::Thunk<0x553C2>::Call<void>(this);
    }

    //members
    AGE_API void aiVehicleManager::Init(char *unused)                           { hook::Thunk<0x553CE0>::Call<void>(this, unused); }
    AGE_API int aiVehicleManager::AddVehicleDataEntry(LPCSTR name)              { return hook::Thunk<0x553FA0>::Call<int>(this, name); }
    AGE_API void aiVehicleManager::SaveEntry()                                  { hook::Thunk<0x5541E0>::Call<void>(this); }

    //asNode overrides
    AGE_API void aiVehicleManager::Reset()                                      { hook::Thunk<0x553D60>::Call<void>(this); }
    AGE_API void aiVehicleManager::Update()                                     { hook::Thunk<0x553EA0>::Call<void>(this); }

    //helpers
    int aiVehicleManager::GetDataCount()
    {
        return this->numVehicleDatas;
    }

    aiVehicleData * aiVehicleManager::GetData(int num)
    {
        if (num < 0 || num >= GetDataCount())
            return nullptr;
        return &this->vehicleDatas[num];
    }

    //lua
    void aiVehicleManager::BindLua(LuaState L) {
        LuaBinding(L).beginExtendClass<aiVehicleManager, asNode>("aiVehicleManager")
            .addStaticProperty("Instance", [] { return Instance.get(); })

            .addFunction("AddVehicleDataEntry", &AddVehicleDataEntry)
            .addFunction("SaveEntry", &SaveEntry)

            .addPropertyReadOnly("NumVehicleDatas", &GetDataCount)
            .addPropertyReadOnly("DataCount", &GetDataCount) // old naming convention
            .addFunction("GetData", &GetData)
            .endClass();
    }
}