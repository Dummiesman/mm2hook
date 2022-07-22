#include "mmPopupHandler.h"

using namespace MM2;

static ConfigValue<bool> cfgChatMusicFix("ChatMusicFix", true);

/*
    mmPopupHandler
*/

// fix cd player persisting through popups
bool wasCdPlayerEnabled = false;

void mmPopupHandler::HudEnable() {
    auto cdPlayer = mmGameManager::Instance.get()->getGame()->GetPlayer()->GetHUD()->GetCDPlayer();

    // if cd player is inactive, and it was active before
    // reactivate it
    if (wasCdPlayerEnabled && !cdPlayer->isActive())
        cdPlayer->Toggle();

    hook::Thunk<0x42D910>::Call<void>(this); // mmHUD::Enable
}

void mmPopupHandler::HudDisable(int a1) {
    auto cdPlayer = mmGameManager::Instance.get()->getGame()->GetPlayer()->GetHUD()->GetCDPlayer();
    wasCdPlayerEnabled = cdPlayer->isActive();

    // hide the cd player if shown
    if (wasCdPlayerEnabled)
        cdPlayer->Toggle();

    hook::Thunk<0x42D970>::Call<void>(this, a1); // mmHUD::Disable
}

void mmPopupHandler::Install() {
    // Fixes chat music presisting after the chat box is closed
    if (cfgChatMusicFix) {
        InstallPatch({ 0x01 }, {
            0x42B558 + 1,
            0x42B537 + 1,
            });
    }

    // CD player fixes
    InstallCallback("mmPopup::DisablePU", "Shows the CD player on popup disable",
        &HudEnable, {
            cb::call(0x42A2F5),
        }
    );

    InstallCallback("mmPopup::ShowResults", "Hides the CD player when popups are showing",
        &HudDisable, {
            cb::call(0x42A65F),
            cb::call(0x42A722),
            cb::call(0x42A7EB),
            cb::call(0x42A3BF),
        }
    );
}