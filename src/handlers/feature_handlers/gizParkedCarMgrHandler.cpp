#include "gizParkedCarMgrHandler.h"

using namespace MM2;

static ConfigValue<bool> cfgDynamicParkedCarDensity("DynamicParkedCarDensity", true);
static ConfigValue<bool> cfgUseAllParkedCars("UseAllParkedCars", true);

/*
    gizParkedCarMgrHandler
*/

void gizParkedCarMgrHandler::EnumeratePath(LPCSTR a1, Matrix34* a2, bool a3) {
    DisableGlobalSeed();

    int numParkedCarModels = (cfgUseAllParkedCars.Get()) ? 3 : 2;
    int num = irand() % (numParkedCarModels + 1);

    if (num > 0)
    {
        // check if we can spawn
        // in cruise, we scale down parked car spawn rates based
        // on traffic density
        float spawnChance = 1.0f;
        float spawnChanceThreshold = 0.0f;
        if (cfgDynamicParkedCarDensity.Get() &&
            dgStatePack::Instance->getGameMode() == dgGameMode::Cruise)
        {
            spawnChance = frand();
            spawnChanceThreshold = 1.0f - dgStatePack::Instance->TrafficDensity;
        }

        // spawn
        if (spawnChance >= spawnChanceThreshold)
        {
            LPCSTR carname = string_buf<128>("giz_pcar%02d_l", num);
            auto banger = dgUnhitBangerInstance::RequestBanger(carname, TRUE);

            if (banger != nullptr)
            {
                Matrix34 mtx;
                Matrix34 rotationMatrix;

                rotationMatrix.Identity();
                rotationMatrix.MakeRotateY(1.5707964f);

                mtx.Set(a2);
                mtx.Dot(rotationMatrix);
                mtx.SetRow(3, a2->GetRow(3)); // set position to a2 position

                banger->Init(carname, mtx, 0);
                banger->SetVariant(irand());

                auto bangerRoom = lvlLevel::Singleton->FindRoomId(a2->GetRow(3), 0);
                lvlLevel::Singleton->MoveToRoom(banger, bangerRoom);
            }
        }
    }

    EnableGlobalSeed();
}

void gizParkedCarMgrHandler::Install() {
    InstallCallback("gizParkedCarMgr::Init", "Scales parked cars with traffic density, allows for more parked car models.",
        &EnumeratePath, {
            cb::push(0x579B80),
        }
    );
}