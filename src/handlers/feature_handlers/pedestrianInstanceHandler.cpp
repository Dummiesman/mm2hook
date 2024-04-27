#include "pedestrianInstanceHandler.h"

using namespace MM2;

static ConfigValue<bool> cfgRagdolls("Ragdolls", true);
static ConfigValue<bool> cfgPedShadows("3DShadows", false);

hook::Type<float> FrameFraction2 = 0x6B4724;
hook::Type<int> FrameDelta2 = 0x6B4720;

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

void pedestrianInstanceHandler::Init(char* a1, int a2, int a3) {
    //call original
    hook::Thunk<0x54B0D0>::Call<void>(this, a1, a2, a3);

    auto inst = reinterpret_cast<aiPedestrian*>(this)->GetInstance();
    inst->SetFlag(64); //shadow flag
}

void pedestrianInstanceHandler::DrawShadow()
{
    auto inst = reinterpret_cast<aiPedestrianInstance*>(this);
    auto animationInstance = inst->GetAnimationInstance();
    auto pedanim = animationInstance->GetAnimation();
    auto timeWeather = cityLevel::GetCurrentLighting();

    if (MMSTATE->TimeOfDay == 3 || lvlLevel::GetSingleton()->GetRoomInfo(inst->GetRoomId())->Flags & static_cast<int>(RoomFlags::Subterranean))
        return;

    Vector3 lightDirection;
    SetLightDirection(&lightDirection, timeWeather->KeyHeading, timeWeather->KeyPitch);

    Matrix34 shadowMatrix;
    Matrix34* pedMatrix = inst->GetPedestrian()->GetMatrix();
    Matrix44 pedestrianMatrixList[32]; //bone matrices

    bool prevLighting = gfxRenderState::SetLighting(true);

    if (lvlInstance::ComputeShadowMatrix(shadowMatrix, inst->GetRoomId(), *pedMatrix))
    {
        float angle = lightDirection.X * shadowMatrix.m10 + lightDirection.Z * shadowMatrix.m12;
        shadowMatrix.SetRow(1, Vector3(lightDirection.X, -angle, lightDirection.Z));

        float dist = shadowMatrix.GetRow(3).Dist(pedMatrix->GetRow(3));
        shadowMatrix.SetRow(3, shadowMatrix.GetRow(3) + shadowMatrix.GetRow(1) * dist);

        //if we have no ragdoll
        if (inst->GetEntity() == nullptr)
        {
            auto sequence = pedanim->GetSequence(animationInstance->GetCurrentState());

            int frame1 = animationInstance->GetCurrentFrame();
            int frame2 = frame1;

            auto animation = sequence->pCrAnimation;
            if (FrameFraction2.get() != 0.0f)
            {
                if (sequence->Direction > 0)
                {
                    frame2 = (frame1 + 1) % animation->GetFrameCount();
                }
                else if (sequence->Direction < 0)
                {
                    frame2 = frame1 - 1;
                    if (frame2 < 0) frame2 = animation->GetFrameCount() - 1;
                }
            }

            crAnimFrame finalFrame = crAnimFrame();
            if (frame1 == frame2)
            {
                finalFrame.Copy(animation->GetFrame(frame1));
            }
            else
            {
                float blend = FrameFraction2.get();
                finalFrame.Blend(blend, animation->GetFrame(frame1), animation->GetFrame(frame2));
            }

            auto skeleton = pedanim->GetSkeleton();
            hook::StaticThunk<0x57B410>::Call<void>(skeleton, &finalFrame); // Some form of crAnimFrame::Pose but static??
            skeleton->Update();
            skeleton->Attach(pedestrianMatrixList);

            gfxRenderState::SetWorldMatrix(shadowMatrix);
            pedanim->GetModel()->DrawShadowed(pedestrianMatrixList, pedanim->GetShaders(animationInstance->GetVariant()), 0xFFFFFFFF, ComputeShadowIntensity(timeWeather->KeyColor));
        }
        else
        {
            Matrix34 shadowMatrix2;
            Matrix44::I.ToMatrix34(shadowMatrix2);

            auto active = reinterpret_cast<pedActive*>(inst->GetEntity());
            auto ragdollSkel = active->GetSkeleton();
            ragdollSkel->Attach(pedestrianMatrixList);

            float angle2 = lightDirection.X * shadowMatrix2.m10 + lightDirection.Z * shadowMatrix2.m12;
            shadowMatrix2.SetRow(1, Vector3(lightDirection.X, -angle2, lightDirection.Z));

            shadowMatrix2.SetRow(3, Vector3(
                shadowMatrix2.GetRow(3).X - shadowMatrix2.m10 * shadowMatrix.m31,
                shadowMatrix.m31,
                shadowMatrix2.GetRow(3).Z - shadowMatrix2.m12 * shadowMatrix.m31));

            gfxRenderState::SetWorldMatrix(shadowMatrix2);
            pedanim->GetModel()->DrawShadowed(pedestrianMatrixList, pedanim->GetShaders(animationInstance->GetVariant()), 0xFFFFFFFF, ComputeShadowIntensity(timeWeather->KeyColor));
        }
    }

    gfxRenderState::SetLighting(prevLighting);
}

void pedestrianInstanceHandler::DrawRagdoll() 
{
    auto inst = reinterpret_cast<aiPedestrianInstance*>(this);
    auto active = reinterpret_cast<pedActive*>(inst->GetEntity());

    Matrix44 pedestrianMatrixList[32]; //bone matrices
    
    auto ragdollSkel = active->GetSkeleton();
    ragdollSkel->Attach(pedestrianMatrixList);

    //get animationInstance and draw it
    auto animationInstance = inst->GetAnimationInstance();
    auto anim = animationInstance->GetAnimation();

    gfxRenderState::SetWorldMatrix(Matrix44::I);
    anim->pModel->Draw(pedestrianMatrixList, anim->ppShaders[animationInstance->GetVariant()], 0xFFFFFFFF);
}

void pedestrianInstanceHandler::AnimationInstance_Draw(bool a1)
{
    auto inst = reinterpret_cast<pedAnimationInstance*>(this);
    Matrix44 pedestrianMatrixList[32]; //bone matrices

    auto pedanim = inst->GetAnimation();
    auto sequence = pedanim->GetSequence(inst->GetCurrentState());

    int frame1 = inst->GetCurrentFrame();
    int frame2 = frame1;

    auto animation = sequence->pCrAnimation;
    if (FrameFraction2.get() != 0.0f) 
    {
        if (sequence->Direction > 0)
        {
            frame2 = (frame1 + 1) % animation->GetFrameCount();
        }
        else if (sequence->Direction < 0)
        {
            frame2 = frame1 - 1;
            if (frame2 < 0) frame2 = animation->GetFrameCount() - 1;
        }
    }

    crAnimFrame finalFrame = crAnimFrame();
    if (frame1 == frame2)
    {
        finalFrame.Copy(animation->GetFrame(frame1));
    }
    else 
    {
        float blend = FrameFraction2.get();
        finalFrame.Blend(blend, animation->GetFrame(frame1), animation->GetFrame(frame2));
    }

    auto skeleton = pedanim->GetSkeleton();
    hook::StaticThunk<0x57B410>::Call<void>(skeleton, &finalFrame); // Some form of crAnimFrame::Pose but static??
    skeleton->Update();
    skeleton->Attach(pedestrianMatrixList);

    if (a1)
    {
        pedanim->GetModel()->Draw(pedestrianMatrixList, pedanim->GetShaders(inst->GetVariant()), 0xFFFFFFFF);
    }
    else
    {
        pedanim->DrawSkeleton(inst->GetVariant(), skeleton);
    }
}

void pedestrianInstanceHandler::AnimationInstance_Update()
{
    // for inteprolated pedestrians we need to switch animations one frame earlier
    auto inst = reinterpret_cast<pedAnimationInstance*>(this);
    int delta = FrameDelta2.get();
    int cutShortFrames = 1;
    
    auto state = inst->GetAnimation()->GetSequence(inst->GetCurrentState());
    int direction = state->Direction;
    if (direction == -1)
    {
        inst->SetCurrentFrame(inst->GetCurrentFrame() - delta);
        if (inst->GetCurrentFrame() > (state->FrameCount + cutShortFrames)) return;
    }
    else if (direction == 1)
    {
        inst->SetCurrentFrame(inst->GetCurrentFrame() + delta);
        if (inst->GetCurrentFrame() < (state->FrameCount - cutShortFrames)) return;
    }
    else if (delta == 0)
    {
        return;
    }
    inst->Start(inst->GetNextState());
}

void pedestrianInstanceHandler::Draw(int a1) 
{
    auto inst = reinterpret_cast<aiPedestrianInstance*>(this);

    //if we have no ragdoll, call the original function
    if (inst->GetEntity() == nullptr) 
    {
        hook::Thunk<0x57B5F0>::Call<void>(this, a1);
        return;
    }
    else
    {
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

    InstallCallback("pedAnimationInstance::Draw", "Interpolate ped animations",
        &AnimationInstance_Draw, {
            cb::call(0x57B6EE),
        }
    );

    InstallCallback("pedAnimationInstance::Update", "Interpolate ped animations",
        &AnimationInstance_Update, {
            cb::call(0x54BF78),
        }
    );

    InstallCallback("aiPedestrian::Init", "Set flag id to 64 to render pedestrian shadows",
        &Init, {
            cb::call(0x53584F), // aiMap::Init
            cb::call(0x5358CC), // aiMap::Init
        }
    );

    InstallVTableHook("aiPedestrianInstance::Draw",
        &Draw, {
            0x5B631C
        }
    );

    if (cfgPedShadows.Get()) {
        InstallVTableHook("aiPedestrianInstance::DrawShadow",
            &DrawShadow, {
                0x5B6320
            }
        );
    }

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

