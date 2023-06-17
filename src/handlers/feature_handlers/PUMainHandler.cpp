#include "PUMainHandler.h"

using namespace MM2;

ConfigValue<bool> cfgInstantReplay("InstantReplay", true);


/*
    PUMainHandler
*/

static float PauseMenuAdjustment = 0.0f;
static float ReplayButtonOffsetY = 0.0f;

class PUMenuHook : public UIMenu {
public:
    UIButton* AddPauseButton(int id, LocString* text, float offsetX, float offsetY, float width, float height, int a7, int a8, datCallback cb, int a10) {
        offsetY += PauseMenuAdjustment;

        Warningf("**** PUMenuHook::AddPauseButton(%d, \"%s\", %.2f, %.2f, %.2f, %.2f, %d, %d, %d) ****",
            id, text, offsetX, offsetY, width, height, a7, a8, a10);

        UIButton* result = this->AddButton(id, text, offsetX, offsetY, width, height, a7, a8, cb, a10);

        // 'Restart Race'
        if (id == 10) {
            // adding it here causes crashes,
            // so just reserve space for it and save its position
            PauseMenuAdjustment = 0.12f;
            ReplayButtonOffsetY = offsetY;
        }
        // 'Exit to Windows'
        else if (id == 14)
        {
            // our custom method keeps spacing consistent (no need to adjust offsets)
            UIButton* rplButton = this->AddPauseButton(
                16,
                (LocString*)AngelReadString(465),
                offsetX,
                ReplayButtonOffsetY,
                width,
                height,
                a7,
                a8,
                datCallback::NullCallback,
                0);

            // set the data
            setPtr(this, 0xBC, rplButton);

            if (datArgParser::Get("pudebug"))
            {
                // add 'DEBUG' button
                this->AddPauseButton(
                    15,
                    (LocString*)AngelReadString(454),
                    offsetX,
                    offsetY + 0.00425f,
                    width,
                    height,
                    a7,
                    2,
                    datCallback::NullCallback,
                    0);
            }

            // reset the adjustment value
            PauseMenuAdjustment = 0.0f;
        }

        return result;
    }
};

void PUMainHandler::Install() {
    if (cfgInstantReplay) {
        InstallCallback("PUMain::ctor", "Overrides button placement for the pause menu.",
            &PUMenuHook::AddPauseButton, {
                cb::call(0x50A6AE),
                cb::call(0x50A712),
                cb::call(0x50A776),
                cb::call(0x50A7D0),
            }
        );

        InstallPatch("Add replay button to main menu", { 0x3C }, {
            0x505EC3 + 2, // MainMenu::MainMenu(int)
        });
    }
}

