#include "pedestrianInstanceHandler.h"

using namespace MM2;

static ConfigValue<bool> cfgRagdolls("Ragdolls", true);


/*
    pedestrianInstanceHandler
*/

void pedestrianInstanceHandler::aiMapClean()
{
    //clean aimap
    hook::Thunk<0x534C10>::Call<void>(this);

    //destroy pedRagdollMgr
    delete pedRagdollMgr::Instance;
}

void pedestrianInstanceHandler::aiMapInit(char * a1, char * a2, char * a3, const dgStatePack * a4, int a5, vehCar * a6, bool a7)
{
    //init aimap
    hook::Thunk<0x534FC0>::Call<void>(this, a1, a2, a3, a4, a5, a6, a7);

    //init pedRagdollMgr
    pedRagdollMgr::Instance = new pedRagdollMgr();

    char* values[2] = { "pedmodel_man", "pedmodel_woman" };
    pedRagdollMgr::Instance->Init(2, values);

    aiMap::GetInstance()->AddChild(pedRagdollMgr::Instance);
}

phBound * pedestrianInstanceHandler::GetBound(int a1)
{
    return pedRagdollMgr::Instance->getBoundBox();
}

bool pedestrianInstanceHandler::IsCollidable() 
{
    return pedRagdollMgr::Instance->UnusedActive();
}

void pedestrianInstanceHandler::DrawRagdoll() 
{
    auto inst = reinterpret_cast<aiPedestrianInstance*>(this);

    //matrices
    Matrix44 pedestrianMatrixList[32]; //bone matrices

    //get pedActive
    auto active = reinterpret_cast<pedActive*>(inst->GetEntity());

    //set matrix
    Matrix44 identityMatrix;
    identityMatrix.Identity();
    gfxRenderState::SetWorldMatrix(identityMatrix);

    //attach skeleton
    auto ragdollSkel = active->getSkeleton();
    ragdollSkel->Attach(&pedestrianMatrixList[0]);

    //get animationInstance and draw it
    auto animationInstance = inst->getAnimationInstance();
    auto anim = animationInstance->getAnimation();
    anim->pModel->Draw(&pedestrianMatrixList[0], anim->ppShaders[animationInstance->getVariant()], 0xFFFFFFFF);
}

void pedestrianInstanceHandler::Draw(int a1) 
{
    auto inst = reinterpret_cast<aiPedestrianInstance*>(this);

    //if we have no ragdoll, call the original function
    if (inst->GetEntity() == nullptr) {
        hook::Thunk<0x57B5F0>::Call<void>(this, a1);
        return;
    }else{
        this->DrawRagdoll();
    }
}

void pedestrianInstanceHandler::Detach()
{
    auto inst = reinterpret_cast<aiPedestrianInstance*>(this);
    auto entity = inst->GetEntity();

    //call pedActive::Detach
    if (entity == nullptr)
        hook::Thunk<0x57C260>::ThisCall<void>(entity);
}

void pedestrianInstanceHandler::Install()
{
    //are ragdolls enabled
    if (!cfgRagdolls.Get())
        return;

    InstallCallback("aiMap::Init", "aiMap initialization hook for ragdoll manager",
        &aiMapInit, {
            cb::call(0x412B98), // mmGame::Init
        }
    );

    InstallCallback("aiMap::Clean", "aiMap clean hook for ragdoll manager",
        &aiMapClean, {
            cb::call(0x413A44), // mmGame::Init
        }
    );

    InstallVTableHook("aiPedestrianInstance::Draw",
        &Draw, {
            0x5B631C
        }
    );

    InstallVTableHook("aiPedestrianInstance::GetBound",
        &GetBound, {
            0x5B6354
        }
    );

    InstallVTableHook("aiPedestrianInstance::Detach",
        &Detach, {
            0x5B6318
        }
    );

    InstallVTableHook("aiPedestrianInstance::IsCollidable",
        &IsCollidable , {
            0x5B6340
        }
    );
}

