#include "vehCableCarInstanceHandler.h"

using namespace MM2;

/*
    Model Index Constants
*/
const int SHADOW_GEOM_ID = 1;
const int HLIGHT_GEOM_ID = 2;


/*
    vehCableCarInstanceHandler
*/

Matrix34 cableCarMatrix = Matrix34();

void vehCableCarInstanceHandler::DrawShadow()
{
    //get vars
    auto inst = reinterpret_cast<lvlInstance*>(this);
    int geomSet = inst->getGeomSetId() - 1;

    //get our shader set
    int shaderSet = 0;
    auto shaders = lvlInstance::GetGeomTableEntry(geomSet)->pShaders[shaderSet];

    //
    Matrix34 shadowMatrix;
    Matrix34 dummyMatrix;

    if (lvlInstance::ComputeShadowMatrix(&shadowMatrix, inst->getRoomId(), &inst->GetMatrix(&dummyMatrix)))
    {
        //setup renderer
        gfxRenderState::SetWorldMatrix(shadowMatrix);

        //draw shadow
        modStatic* shadow = lvlInstance::GetGeomTableEntry(geomSet + SHADOW_GEOM_ID)->getHighestLOD();
        if (shadow != nullptr)
        {
            shadow->Draw(shaders);
        }
    }
}

void vehCableCarInstanceHandler::DrawGlow()
{
    //We only draw a headlight glow here. Bail immediately if we can
    if (!aiMap::Instance->showHeadlights)
        return;

    //get vars
    auto inst = reinterpret_cast<lvlInstance*>(this);
    int geomSet = inst->getGeomSetId() - 1;

    //setup renderer
    Matrix34 instMtx = inst->GetMatrix(&cableCarMatrix);
    memcpy(&cableCarMatrix, &instMtx, sizeof(Matrix34));
    gfxRenderState::SetWorldMatrix(cableCarMatrix);

    //get our shader set
    int shaderSet = 0;
    auto shaders = lvlInstance::GetGeomTableEntry(geomSet)->pShaders[shaderSet];

    //get lights
    modStatic* hlight = lvlInstance::GetGeomTableEntry(geomSet + HLIGHT_GEOM_ID)->getHighestLOD();
    if (hlight != nullptr)
    {
        hlight->Draw(shaders);
    }
}

bool vehCableCarInstanceHandler::BeginGeom(const char* a1, const char* a2, int a3)
{
    //We hook this to set flag 64 (shadow)
    auto inst = reinterpret_cast<lvlInstance*>(this);
    inst->setFlag(64);

    //Call original
    return inst->BeginGeom(a1, a2, a3);
}

void vehCableCarInstanceHandler::Install()
{
    InstallVTableHook("vehCableCarInstanceHandler::DrawGlow",
        &DrawGlow, {
            0x5B5690,
        }
    );

    InstallVTableHook("vehCableCarInstanceHandler::DrawShadow",
        &DrawShadow, {
            0x5B5688,
        }
    );

    InstallCallback("aiCableCar::Init", "Hook BeginGeom to set instance shadowing flag.",
        &BeginGeom, {
            cb::call(0x53F8AA),
        }
    );
}

