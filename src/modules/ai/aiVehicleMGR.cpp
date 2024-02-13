#include "aiVehicleMGR.h"
#include <modules\gfx\rstate.h>
#include <modules\model\static2.h>
#include <modules\vehicle\breakable.h>
#include <modules\ai\aiMap.h>

namespace MM2
{
    /*
        aiVehicleActive
    */

    vehWeelCheap* aiVehicleActive::GetWheel(int num) {
        switch (num) {
        case 0:
            return &WheelFrontLeft;
        case 1:
            return &WheelFrontRight;
        case 2:
            return &WheelBackLeft;
        case 3:
            return &WheelBackRight;
        }
        return nullptr;
    }

    //dgPhysEntity overrides
    AGE_API void aiVehicleActive::Update()                             { hook::Thunk<0x553890>::Call<void>(this); }
    AGE_API void aiVehicleActive::PostUpdate()                         { hook::Thunk<0x553960>::Call<void>(this); }
    AGE_API phInertialCS* aiVehicleActive::GetICS()                    { return hook::Thunk<0x5543B0>::Call<phInertialCS*>(this); }
    AGE_API lvlInstance* aiVehicleActive::GetInst()                    { return hook::Thunk<0x553430>::Call<lvlInstance*>(this); }
    AGE_API void aiVehicleActive::DetachMe()                           { hook::Thunk<0x553690>::Call<void>(this); }

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
    Matrix34 aiVehicleMatrix = Matrix34();

    // Properties
    aiVehicleSpline* aiVehicleInstance::GetSpline()
    {
        return this->Spline;
    }

    int aiVehicleInstance::GetVariant() const
    {
        return this->Variant;
    }
  
    //overrides
    AGE_API Vector3 const& aiVehicleInstance::GetPosition()              { return hook::Thunk<0x553030>::Call<Vector3 const&>(this); };
    AGE_API Matrix34 const& aiVehicleInstance::GetMatrix(Matrix34 &mtx)  { return hook::Thunk<0x553020>::Call<Matrix34 const&>(this, &mtx); };
    AGE_API void aiVehicleInstance::SetMatrix(Matrix34 const & mtx)      { hook::Thunk<0x553010>::Call<void>(this, &mtx); }
    AGE_API dgPhysEntity* aiVehicleInstance::GetEntity()                 { return hook::Thunk<0x52F50>::Call<dgPhysEntity*>(this); };
    AGE_API dgPhysEntity* aiVehicleInstance::AttachEntity()              { return hook::Thunk<0x552FBD>::Call<dgPhysEntity*>(this); };
    AGE_API void aiVehicleInstance::Detach()                             { hook::Thunk<0x552F80>::Call<void>(this); }

    AGE_API void aiVehicleInstance::Draw(int lod)
    {
        if (lod < 0 || lod > 3)
            return;
        if (this->GetGeomIndex() == 0)
            return;

        //get shaders
        auto shaders = this->GetShader(this->GetVariant());

        //setup renderer
        gfxRenderState::SetWorldMatrix(this->GetMatrix(aiVehicleMatrix));

        //draw the body
        modStatic* bodyGeom = this->GetGeom(lod, 0);
        if (bodyGeom != nullptr)
            bodyGeom->Draw(shaders);

        this->BreakableMgr->Draw(this->GetMatrix(aiVehicleMatrix), shaders, lod);

        //draw reflection (only in H LOD)
        float reflectionIntensity;
        auto reflectionMap = lvlLevel::GetSingleton()->GetEnvMap(this->GetRoomId(), this->GetPosition(), reflectionIntensity);
        if (lod == 3 && reflectionMap != nullptr && bodyGeom != nullptr)
        {
            modShader::BeginEnvMap(reflectionMap, this->GetMatrix(aiVehicleMatrix));
            bodyGeom->DrawEnvMapped(shaders, reflectionMap, reflectionIntensity);
            modShader::EndEnvMap();
        }

        //draw wheels (only in H LOD)
        if (lod == 3)
        {
            auto data = this->GetData();

            if (this->GetEntity() != nullptr)
            {
                auto vehicleActive = (aiVehicleActive*)this->GetEntity();

                for (int i = 0; i < 4; i++)
                {
                    modStatic* wheelModel = this->GetGeom(lod, WHL0_GEOM_ID + i);
                    if (wheelModel != nullptr)
                        DrawPart(wheelModel, vehicleActive->GetWheel(i)->GetMatrix(), shaders, vehCarModel::PartReflections);
                }

                modStatic* whl4Model = this->GetGeom(lod, WHL4_GEOM_ID);
                if (whl4Model != nullptr)
                {
                    Matrix34 whl4Matrix;

                    whl4Matrix.Identity();
                    whl4Matrix.SetRow(3, data->GetWheelPosition(4));
                    whl4Matrix.Dot(this->GetMatrix(aiVehicleMatrix));

                    DrawPart(whl4Model, whl4Matrix, shaders, vehCarModel::PartReflections);
                }

                modStatic* whl5Model = this->GetGeom(lod, WHL5_GEOM_ID);
                if (whl5Model != nullptr)
                {
                    Matrix34 whl5Matrix;

                    whl5Matrix.Identity();
                    whl5Matrix.SetRow(3, data->GetWheelPosition(5));
                    whl5Matrix.Dot(this->GetMatrix(aiVehicleMatrix));

                    DrawPart(whl5Model, whl5Matrix, shaders, vehCarModel::PartReflections);
                }
            }
            else {
                for (int i = 0; i < 4; i++)
                {
                    Matrix34 wheelMatrix;

                    wheelMatrix.Identity();
                    wheelMatrix.SetRow(3, data->GetWheelPosition(i));
                    wheelMatrix.MakeRotateX(-this->Spline->GetWheelRotation());
                    wheelMatrix.Dot(this->GetMatrix(aiVehicleMatrix));

                    modStatic* wheelModel = this->GetGeom(lod, WHL0_GEOM_ID + i);
                    if (wheelModel != nullptr)
                    {
                        modStatic* sWheelModel = this->GetGeom(lod, SWHL0_GEOM_ID + i);
                        if (this->Spline->GetSpeed() > 20.f && sWheelModel != nullptr && vehCarModel::EnableSpinningWheels)
                        {
                            DrawPart(sWheelModel, wheelMatrix, shaders, vehCarModel::PartReflections);
                        }
                        else {
                            DrawPart(wheelModel, wheelMatrix, shaders, vehCarModel::PartReflections);
                        }
                    }
                }

                modStatic* whl4Model = this->GetGeom(lod, WHL4_GEOM_ID);
                if (whl4Model != nullptr)
                {
                    Matrix34 whl4Matrix;

                    whl4Matrix.Identity();
                    whl4Matrix.SetRow(3, data->GetWheelPosition(4));
                    whl4Matrix.MakeRotateX(-this->Spline->GetWheelRotation());
                    whl4Matrix.Dot(this->GetMatrix(aiVehicleMatrix));

                    modStatic* swhl4Model = this->GetGeom(lod, SWHL4_GEOM_ID);
                    if (this->Spline->GetSpeed() > 20.f && swhl4Model != nullptr && vehCarModel::EnableSpinningWheels)
                    {
                        DrawPart(swhl4Model, whl4Matrix, shaders, vehCarModel::PartReflections);
                    }
                    else {
                        DrawPart(whl4Model, whl4Matrix, shaders, vehCarModel::PartReflections);
                    }
                }

                modStatic* whl5Model = this->GetGeom(lod, WHL5_GEOM_ID);
                if (whl5Model != nullptr)
                {
                    Matrix34 whl5Matrix;

                    whl5Matrix.Identity();
                    whl5Matrix.SetRow(3, data->GetWheelPosition(5));
                    whl5Matrix.MakeRotateX(-this->Spline->GetWheelRotation());
                    whl5Matrix.Dot(this->GetMatrix(aiVehicleMatrix));

                    modStatic* swhl5Model = this->GetGeom(lod, SWHL5_GEOM_ID);
                    if (this->Spline->GetSpeed() > 20.f && swhl5Model != nullptr && vehCarModel::EnableSpinningWheels)
                    {
                        DrawPart(swhl5Model, whl5Matrix, shaders, vehCarModel::PartReflections);
                    }
                    else {
                        DrawPart(whl5Model, whl5Matrix, shaders, vehCarModel::PartReflections);
                    }
                }
            }
        }

        //draw pop-up headlights
        if (lod >= 2)
        {
            modStatic* plighton = this->GetGeom(lod, PLIGHTON_GEOM_ID);
            if (plighton != nullptr)
            {
                if (aiMap::GetInstance()->showHeadlights)
                    DrawPart(plighton, this->GetMatrix(aiVehicleMatrix), shaders, lod == 3);
            }

            modStatic* plightoff = this->GetGeom(lod, PLIGHTOFF_GEOM_ID);
            if (plightoff != nullptr)
            {
                if (!aiMap::GetInstance()->showHeadlights)
                    DrawPart(plightoff, this->GetMatrix(aiVehicleMatrix), shaders, lod == 3);
            }
        }

        this->LOD = lod + 1;
    }

    AGE_API void aiVehicleInstance::DrawShadow()                         { hook::Thunk<0x552CC0>::Call<void>(this); }
    AGE_API void aiVehicleInstance::DrawShadowMap()                      { hook::Thunk<0x552F30>::Call<void>(this); }
    AGE_API void aiVehicleInstance::DrawReflected(float intensity)       { hook::Thunk<0x552CB0>::Call<void>(this, intensity); }
    AGE_API unsigned int aiVehicleInstance::SizeOf()                     { return hook::Thunk<0x553060>::Call<unsigned int>(this); };
    AGE_API phBound* aiVehicleInstance::GetBound(int type)               { return hook::Thunk<0x552F40>::Call<phBound*>(this, type); };
        
    //members
    aiVehicleData* aiVehicleInstance::GetData()                          { return hook::Thunk<0x553F80>::Call<aiVehicleData*>(this); }
    AGE_API void aiVehicleInstance::DrawPart(modStatic* model, const Matrix34& matrix, modShader* shaders, int unused)
                                                                         { hook::Thunk<0x552870>::Call<void>(this, model, &matrix, shaders, unused); }

    AGE_API void aiVehicleInstance::DrawPart(int lod, int geomId, const Matrix34& matrix, modShader* shaders)
    {
        auto model = this->GetGeom(lod, geomId);
        if (model != nullptr)
            DrawPart(model, matrix, shaders, 0);
    }

    void aiVehicleInstance::DrawPartReflected(int lod, int geomId, const Matrix34& matrix, modShader* shaders)
    {
        auto model = this->GetGeom(lod, geomId);
        if (model != nullptr) {
            DrawPart(model, matrix, shaders, 0);

            float reflectionIntensity;
            auto reflectionMap = lvlLevel::GetSingleton()->GetEnvMap(this->GetRoomId(), this->GetPosition(), reflectionIntensity);
            if (reflectionMap != nullptr)
            {
                modShader::BeginEnvMap(reflectionMap, matrix);
                model->DrawEnvMapped(shaders, reflectionMap, reflectionIntensity);
                modShader::EndEnvMap();
            }
        }
    }

    void aiVehicleInstance::DrawPart(int lod, int geomId, const Matrix34& matrix, modShader* shaders, bool reflected)
    {
        if (reflected)
            DrawPartReflected(lod, geomId, matrix, shaders);
        else
            DrawPart(lod, geomId, matrix, shaders);
    }

    //lua
    void aiVehicleInstance::BindLua(LuaState L) {
        LuaBinding(L).beginExtendClass<aiVehicleInstance, lvlInstance>("aiVehicleInstance")
            //members
            .addFunction("GetData", &GetData)
            .endClass();
    }
}