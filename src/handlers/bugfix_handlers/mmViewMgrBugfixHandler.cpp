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
    auto dash = player->getDashView();

    if (!dash->getIsLoaded())
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