#include "mmViewMgrBugfixHandler.h"

using namespace MM2;

static ConfigValue<bool> cfgMissingDashboardFix("MissingDashboardFix", true);

/*
    mmViewMgrBugfixHandler
*/

void mmViewMgrBugfixHandler::SetViewSetting(int a1)
{
    mmGameManager* mgr = mmGameManager::Instance;
    auto player = mgr->getGame()->getPlayer();
    auto basename = player->getCar()->getCarDamage()->GetName();

    //check if dashboard model is missing
    //TODO: use mmDashView instead,
    //byte at 0x604 is 1 if model exists, 0 if it does not
    string_buf<80> buffer("%s_dash", basename);
    if (!datAssetManager::Exists("geometry", buffer, "pkg"))
        return;

    //call original
    hook::Thunk<0x431D10>::Call<void>(this, a1);
}

void mmViewMgrBugfixHandler::Install()
{
    if (cfgMissingDashboardFix.Get()) {
        InstallCallback("mmViewMgr::SetViewSetting", "Disables the dashboard view camera if a dashboard model is missing.",
            &SetViewSetting, {
                cb::call(0x41489A),
            }
        );
    }
}