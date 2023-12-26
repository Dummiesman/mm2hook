#include "pedestrianInstanceHandler.h"

using namespace MM2;

static ConfigValue<bool> cfgRagdolls("Ragdolls", true);


/*
    pedestrianInstanceHandler
*/

void pedestrianInstanceHandler::aiMapClearPeds(MM2::aiPath* path)
{
    // This is a backported version of ClearPeds from MC1
    auto ped = path->GetPedestrians();
    auto aimap = aiMap::GetInstance();
    auto ragdollMgr = pedRagdollMgr::Instance;

    while (ped)
    {
        auto next = ped->GetNext();
        path->RemovePedestrian(ped);
        aimap->AddPedestrian(ped);
        if (ragdollMgr != nullptr)
        {
            ragdollMgr->Detach(ped->GetInstance());
        }
        ped = next;
    }
}

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

    auto cityData = aiMap::GetInstance()->GetCityData();
    pedRagdollMgr::Instance->Init(cityData->GetNumPedTypes(), cityData->GetGoodWeatherPedNames());

    aiMap::GetInstance()->AddChild(pedRagdollMgr::Instance);
}

phBound * pedestrianInstanceHandler::GetBound(int a1)
{
    return pedRagdollMgr::Instance->GetBoundBox();
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
    auto ragdollSkel = active->GetSkeleton();
    ragdollSkel->Attach(&pedestrianMatrixList[0]);

    //get animationInstance and draw it
    auto animationInstance = inst->GetAnimationInstance();
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

void pedestrianInstanceHandler::FirstImpactCallback()
{
    //call original
    hook::Thunk<0x57C2E0>::Call<void>(this);

    //call play impact reaction
    auto active = reinterpret_cast<pedActive*>(this);
    float force = active->GetICS()->GetImpulse().Mag(); // assumed atm. decompilation of MC1 method yields uninitialized second param.
    reinterpret_cast<aiPedestrianInstance*>(active->GetInst())->GetPedestrian()->GetAudio()->PlayImpactReaction(force);
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

    InstallCallback("aiMap::ClearPath", "aiMap clearpeds hook to detach ragdolls",
        &aiMapClearPeds, {
            cb::call(0x539B70), // aiMap::AdjustPedestrians
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

    // MM2 doesn't call PlayImpactReaction for some reason
    // it's the only difference in this function versus MC1
    InstallVTableHook("pedActive::FirstImpactCallback",
        &FirstImpactCallback, {
            0x5B63C0
        }
    );

    // reduce impact cooldown time from 60 seconds (MM2 default) to 7 seconds (MC1 default)
    // this lets the player well, actually hear creature impact sounds
    mem::write(0x5D24C4, 7.0f);
}

