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

UITextField* MM2::UIMenu::AddTextFieldLua(int id, LPCSTR name, CharBox* buffer, float x, float y, float w, float h, int maxLength, int somelen2, int someflags, int fontSize, LuaRef onChangedCallback)
{
	return this->AddTextField(id, name, buffer->GetBoxedValuePointer(), x, y, w, h, maxLength, somelen2, someflags, fontSize, 0, *datCallback::CreateParamaterlessLuaCallback(onChangedCallback));
}

uiWidget* MM2::UIMenu::AddIconWLua(int id, LPCSTR name, LPCSTR bitmap, float x, float y, float w, float h, LuaRef callback)
{
	return this->AddIconW(id, name, bitmap, x, y, w, h, *datCallback::CreateParamaterlessLuaCallback(callback));
}

uiWidget* MM2::UIMenu::AddToggleLua(int id, LPCSTR name, IntBox* pValue, float x, float y, float w, float h, int fontSize, int buttonType, LuaRef callback)
{
	return this->AddToggle(id, name, pValue->GetBoxedValuePointer(), x, y, w, h, fontSize, buttonType, *datCallback::CreateParamaterlessLuaCallback(callback));
}

uiWidget* MM2::UIMenu::AddVScrollBarLua(int id, IntBox* pValue, float x, float y, float w, float h, float rangeMin, float rangeMax, int a10, int a11, LuaRef callback)
{
	return this->AddVScrollBar(id, pValue->GetBoxedValuePointer(), x, y, w, h, rangeMin, rangeMax, a10, a11, *datCallback::CreateParamaterlessLuaCallback(callback));
}
  