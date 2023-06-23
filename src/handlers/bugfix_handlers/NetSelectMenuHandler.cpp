#include "NetSelectMenuHandler.h"

using namespace MM2;

/*
    NetSelectMenuHandler
*/

UIBMButton* NetSelectMenuHandler::AddBMButton(int id, LPCSTR name, float x, float y, int numStates, datCallback onToggle, int* pValue, int radioButtonValue, int soundIndex, datCallback onHover)
{
    auto menu = reinterpret_cast<UIMenu*>(this);
	auto button = menu->AddBMButton(id, name, x, y, numStates, onToggle, pValue, radioButtonValue, soundIndex, onHover);
	button->Disable();
	return button;
}

void NetSelectMenuHandler::Install()
{
    InstallCallback("NetSelectMenu::NetSelectMenu", "Grey out zone button",
        &AddBMButton, {
            cb::call(0x5042DD), // NetSelectMenu::NetSelectMenu
        }
    );
}