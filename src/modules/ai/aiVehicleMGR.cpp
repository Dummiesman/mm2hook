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
    AGE_API void aiVehicleManager::Attach(aiVehicleInstance* instance)          { hook::Thunk<0x553DC0>::Call<void>(this, instance); }
    AGE_API void aiVehicleManager::Detach(aiVehicleActive* active)              { hook::Thunk<0x553E30>::Call<void>(this, active); }

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

    /*
        aiVehicleInstance
    */

    // Properties
    aiVehicleSpline* aiVehicleInstance::GetSpline()
    {
        return _spline.get(this);
    }

    int aiVehicleInstance::GetVariant() const
    {
        return _variant.get(this);
    }
        
    //overrides
    AGE_API Vector3 const& aiVehicleInstance::GetPosition()              { return hook::Thunk<0x553030>::Call<Vector3 const&>(this); };
    AGE_API Matrix34 const& aiVehicleInstance::GetMatrix(Matrix34& a1)   { return hook::Thunk<0x553020>::Call<Matrix34 const&>(this, &a1); };
    AGE_API void aiVehicleInstance::SetMatrix(Matrix34 const & a1)       { hook::Thunk<0x553010>::Call<void>(this, &a1); }
    AGE_API dgPhysEntity* aiVehicleInstance::GetEntity()                 { return hook::Thunk<0x52F50>::Call<dgPhysEntity*>(this); };
    AGE_API dgPhysEntity* aiVehicleInstance::AttachEntity()              { return hook::Thunk<0x552FBD>::Call<dgPhysEntity*>(this); };
    AGE_API void aiVehicleInstance::Detach()                             { hook::Thunk<0x552F80>::Call<void>(this); }
    AGE_API void aiVehicleInstance::DrawShadow()                         { hook::Thunk<0x552CC0>::Call<void>(this); }
    AGE_API void aiVehicleInstance::DrawShadowMap()                      { hook::Thunk<0x552F30>::Call<void>(this); }
        
    AGE_API void aiVehicleInstance::Draw(int a1)                         { hook::Thunk<0x552160>::Call<void>(this, a1); }

    AGE_API void aiVehicleInstance::DrawReflected(float a1)              { hook::Thunk<0x552CB0>::Call<void>(this, a1); }
    AGE_API unsigned int aiVehicleInstance::SizeOf()                     { return hook::Thunk<0x553060>::Call<unsigned int>(this); };
    AGE_API phBound* aiVehicleInstance::GetBound(int a1)                 { return hook::Thunk<0x552F40>::Call<phBound*>(this, a1); };
        
    //members
    aiVehicleData* aiVehicleInstance::GetData()                           { return hook::Thunk<0x553F80>::Call<aiVehicleData*>(this); }
    AGE_API void aiVehicleInstance::DrawPart(modStatic* a1, const Matrix34& a2, modShader* a3, int a4)
                                                                          { hook::Thunk<0x552870>::Call<void>(this, a1, &a2, a3, a4); }

    //lua
    void aiVehicleInstance::BindLua(LuaState L) {
        LuaBinding(L).beginExtendClass<aiVehicleInstance, lvlInstance>("aiVehicleInstance")
            //members
            .addFunction("GetData", &GetData)
            .endClass();
    }
}