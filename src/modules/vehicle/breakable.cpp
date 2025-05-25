#include "breakable.h"
#include <modules/gfx/rstate.h>
#include <modules/model/static2.h>
#include <modules/level/level.h>
#include <modules/model/shader.h>

using namespace MM2;

// vehBreakable
AGE_API vehBreakable::vehBreakable(modStatic** a1, Matrix34* a2, int a3, int a4, int a5)
{
    hook::Thunk<0x4D8530>::Call<void>(this);
}
        
AGE_API void vehBreakable::Add(vehBreakable* breakable) {
    hook::Thunk<0x4D8BE0>::Call<void>(this, breakable);
}

void vehBreakable::BindLua(LuaState L) 
{
    LuaBinding(L).beginClass<vehBreakable>("vehBreakable")
        .addFunction("Add", &Add)
    .endClass();
}

// vehBreakableMgr
vehBreakableMgr::vehBreakableMgr()                           
{
    hook::Thunk<0x4D8580>::Call<void>(this);
}

vehBreakableMgr::~vehBreakableMgr()                          
{
    hook::Thunk<0x4D85B0>::Call<void>(this); 
}

int vehBreakableMgr::GetVariant()
{
    return this->variant;
}

void vehBreakableMgr::SetVariant(int variant)
{
    this->variant = variant;
}

bool vehBreakableMgr::Init(Matrix34* a1)                  
{
    return hook::Thunk<0x4D85E0>::Call<bool>(this, a1); 
}

void vehBreakableMgr::Add(LPCSTR a1, LPCSTR a2, modStatic** a3, int a4, int a5)
{
    hook::Thunk<0x4D86A0 >::Call<void>(this, a1, a2, a3, a4, a5);
}

void vehBreakableMgr::Create(LPCSTR a1, LPCSTR a2, modStatic** a3, int a4, int a5)
{
    hook::Thunk<0x4D8600 >::Call<void>(this, a1, a2, a3, a4, a5);
}

void vehBreakableMgr::Eject(vehBreakable* a1, int room)
{
    hook::Thunk<0x4D8940>::Call<void>(this, a1, room);
}

void vehBreakableMgr::EjectAll(int room)
{
    hook::Thunk<0x4D8BB0>::Call<void>(this, room);
}

void vehBreakableMgr::Reset()
{
    hook::Thunk<0x4D8710>::Call<void>(this);
}

void vehBreakableMgr::Draw(const Matrix34& world, modShader* shaders, int lod)
{
    for (auto i = this->first; i; i = i->next)
    {
        if (i->isAttached)
        {
            auto lodModel = i->model->GetLOD(lod);
            if (lodModel != nullptr)
            {
                Matrix34 drawMatrix = Matrix34();
                drawMatrix.Set(i->matrix);
                drawMatrix.Dot(world);

                //setup renderer
                gfxRenderState::SetWorldMatrix(drawMatrix);

                lodModel->Draw(shaders);

                //reflection
                if (EnableReflections)
                {
                    float reflectionIntensity;
                    auto reflectionMap = lvlLevel::GetSingleton()->GetEnvMap(0, world.GetRow(3), reflectionIntensity);
                    if (lod == 3 && reflectionMap != nullptr)
                    {
                        modShader::BeginEnvMap(reflectionMap, drawMatrix);
                        lodModel->DrawEnvMapped(shaders, reflectionMap, reflectionIntensity);
                        modShader::EndEnvMap();
                    }
                }
            }
        }
    }
}

vehBreakable* vehBreakableMgr::Get(int index)
{
    return hook::Thunk<0x4D86F0>::Call<vehBreakable*>(this, index);
}

void vehBreakableMgr::BindLua(LuaState L) 
{
    LuaBinding(L).beginClass<vehBreakableMgr>("vehBreakableMgr")
        .addConstructor(LUA_ARGS())
        .addFunction("Add", &Add)
        .addFunction("Create", &Create)
        .addFunction("Eject", &Eject)
        .addFunction("EjectAll", &EjectAll)
        .addFunction("Get", &Get)
    .endClass();
}