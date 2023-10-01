#include "mmViewMgrBugfixHandler.h"

using namespace MM2;

static ConfigValue<bool> cfgMissingDashboardFix("MissingDashboardFix", true);

/*
    mmViewMgrBugfixHandler
*/

void mmViewMgrBugfixHandler::SetViewSetting_Dash(int a1)
{
    mmGameManager* mgr = mmGameManager::Instance;

    auto player = mgr->getGame()->GetPlayer();
    auto dash = player->GetHUD()->GetDashView();

    if (!dash->getIsLoaded())
        return;

    //call original
    hook::Thunk<0x431D10>::Call<void>(this, a1);
}

void mmViewMgrBugfixHandler::SetViewSetting_Mirror(int a1)
{
    //call original
    hook::Thunk<0x431D10>::Call<void>(this, a1);

    //copy over mirror state to MMSTATE
    auto viewmgr = reinterpret_cast<mmViewMgr*>(this);
    auto mirror = viewmgr->GetMirror();
    auto state = &MMSTATE;
    state->MirrorEnabled = mirror->isActive();
}

void mmViewMgrBugfixHandler::Install()
{
    if (cfgMissingDashboardFix.Get()) {
        InstallCallback("mmViewMgr::SetViewSetting", "Disables the dashboard view camera if a dashboard model is missing.",
            &SetViewSetting_Dash, {
                cb::call(0x41489A),
            }
        );
    }

    InstallCallback("mmViewMgr::SetViewSetting", "Fixes mirror state not saving to MMSTATE (and therefore the player config file).",
        &SetViewSetting_Mirror, {
            cb::call(0x4148B8),
        }
    );
}