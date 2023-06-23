#pragma once
#include "common.h"
#include "patch.h"
#include "mm2.h"

using namespace MM2;

class NetSelectMenuHandler {
public:
	UIBMButton* AddBMButton(int id, LPCSTR name, float x, float y, int numStates, datCallback onToggle, int* pValue, int radioButtonValue, int soundIndex, datCallback onHover);
    static void Install();
};