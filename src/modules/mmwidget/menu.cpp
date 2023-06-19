#pragma once
#include "menu.h"

using namespace MM2;

UIBMButton* UIMenu::AddBMButtonLua(int id, LPCSTR name, float x, float y, int numStates, LuaRef onToggleCallback, LuaRef pBoxedValue, int radioButtonValue, int soundIndex, LuaRef hoverCallback)
{
	int* pValue = nullptr;
	if (pBoxedValue.type() != LuaTypeID::NIL)
	{
		pValue = pBoxedValue.toValue<IntBox*>()->GetBoxedValuePointer();
	}

	return this->AddBMButton(id, name, x, y, numStates,
							 *datCallback::CreateParamaterlessLuaCallback(onToggleCallback),
							 pValue,
							 radioButtonValue,
							 soundIndex,
							 *datCallback::CreateLuaCallback<int>(hoverCallback));
			
}

UITextDropdown* MM2::UIMenu::AddTextDropdownLua(int id, LPCSTR name, IntBox* pBoxedValue, float x, float y, float w, float h, LPCSTR choicesString, int nameTextMode, LuaRef onValueChangedCallback, LPCSTR backgroundName, LuaRef hoverCallback)
{
	{
		auto tempString = string(choicesString);
		return this->AddTextDropdown(id, name, pBoxedValue->GetBoxedValuePointer(), 
									 x, y, w, h, tempString, 0, nameTextMode, 0,
									 *datCallback::CreateParamaterlessLuaCallback(onValueChangedCallback), 
									 backgroundName, 
									 *datCallback::CreateLuaCallback<int>(hoverCallback));
	}
}

UISlider* MM2::UIMenu::AddSliderLua(int id, LPCSTR name, FloatBox* pValue, float x, float y, float w, float h, float minValue, float maxValue, int textMode, bool isBalanceSlider, LuaRef onValueChangedCallback, LuaRef hoverCallback)
{
	return this->AddSlider(id, (LocString*)name, pValue->GetBoxedValuePointer(),
						   x, y, w, h, minValue, maxValue, 0, textMode, 0,
						   (isBalanceSlider) ? TRUE : FALSE, 
						   *datCallback::CreateParamaterlessLuaCallback(onValueChangedCallback),
						   *datCallback::CreateLuaCallback<int>(hoverCallback));
}
  