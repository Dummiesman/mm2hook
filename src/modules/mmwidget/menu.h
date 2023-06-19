#pragma once
#include "widget.h"

namespace MM2
{
    // Forward declarations
    class UIMenu;

    // External declarations
    extern class UIBMButton;
    extern class UIBMLabel;
    extern class UIButton;
    extern class UIIcon;
    extern class UITextRoller2;
    extern class UITextDropdown;
    extern class UILabel;
    extern class UISlider;

    // Class definitions
    class UIMenu : public asNode {
    public:
        uint32_t ActionSource;
        uint32_t ActionState;
        uint32_t dword20;
        uint32_t MenuID;
        uint32_t dword28;
        uint32_t WidgetCount;
        uint32_t dword30;
        uiWidget **ppWidgets;
        const char *Name;
        uint32_t dword3C;
        uint32_t dword40;
        uint32_t dword44;
        Vector2 Position;
        Vector2 Scale;
        uint32_t dword58;
        float ScaleX;
        float ScaleY;
        int dword64;
        int *pCurrentWidgetID;
        int ActiveWidgetID;
        int dword70;
        int WidgetID;
        int PreviousMenu;
        uint32_t dword7C;
        float dword80;
        uint32_t dword84;
        const char *Background;
    private:
        // lua helpers
        UIButton* AddButtonLua(int id, LPCSTR text, float x, float y, float w, float h, int fontNum, int type, LuaRef callback, int i3)
        {
            return this->AddButton(id, (LocString*)text, x, y, w, h, fontNum, type, *datCallback::CreateParamaterlessLuaCallback(callback), i3);
        }

        UIBMLabel* AddBMLabelLua(int id, LPCSTR name, LPCSTR bitmapNames, float x, float y, IntBox* pBoxedValue)
        {
            auto tempString = string(bitmapNames);
            return this->AddBMLabel(id, name, &tempString, x, y, pBoxedValue->GetBoxedValuePointer());
        }

        UIBMButton* AddBMButtonLua(int id, LPCSTR name, float x, float y, int numStates, LuaRef onToggleCallback, LuaRef pBoxedValue, int radioButtonValue, int soundIndex, LuaRef hoverCallback);

        //(int id, LPCSTR name, float x, float y, float w, float h, string valuesString, BOOL wrapAround, int maxValue, int nameTextMode, int fontNum, datCallback onValueChanged)
        UITextRoller2* AddTextRoller2Lua(int id, LPCSTR name, IntBox* pBoxedValue, float x, float y, float w, float h, LPCSTR valuesString, bool wrapAround, int maxValue, int nameTextMode, int fontNum, LuaRef onValueChangedCallback)
        {
            auto tempString = string(valuesString);
            return this->AddTextRoller2(id, name, pBoxedValue->GetBoxedValuePointer(), x, y, w, h, tempString, wrapAround ? TRUE : FALSE, maxValue, nameTextMode, fontNum, *datCallback::CreateParamaterlessLuaCallback(onValueChangedCallback));
        }

        UITextDropdown* AddTextDropdownLua(int id, LPCSTR name, IntBox* pBoxedValue, float x, float y, float w, float h, LPCSTR choicesString, int nameTextMode, LuaRef onValueChangedCallback, LPCSTR backgroundName, LuaRef hoverCallback);

        UILabel* AddLabelLua(int id, LPCSTR text, float x, float y, float w, float h, int effectFlags, int fontType)
        {
            return this->AddLabel(id, (LocString*)text, x, y, w, h, effectFlags, fontType);
        }

        UISlider* AddSliderLua(int id, LPCSTR name, FloatBox* pValue, float x, float y, float w, float h, float minValue, float maxValue, int textMode, bool isBalanceSlider, LuaRef onValueChangedCallback, LuaRef hoverCallback);
    protected:
        // constructor for inherited classes to use like PUMenuBase
        UIMenu() {}
    public:
        AGE_API UIMenu(int id) {
            scoped_vtable x(this);
            hook::Thunk<0x4E0770>::Call<void>(this, id);
        }

        AGE_API ~UIMenu() {
            scoped_vtable x(this);
            hook::Thunk<0x4E0870>::Call<void>(this);
        }

        /*
            asNode virtuals
        */

        AGE_API void Update() override                      { hook::Thunk<0x4E0A80>::Call<void>(this); }

        /*
            uiMenu virtuals
        */

        AGE_API virtual void PreSetup()                     { hook::Thunk<0x4E0A60>::Call<void>(this); }
        AGE_API virtual void PostSetup()                    { hook::Thunk<0x4E0A70>::Call<void>(this); }
        AGE_API virtual void BackUp()                       { hook::Thunk<0x4E0B10>::Call<void>(this); }
        AGE_API virtual void CheckInput()                   { hook::Thunk<0x4E0DE0>::Call<void>(this); }
        AGE_API virtual BOOL IsAnOptionMenu()               { return hook::Thunk<0x4E2D70>::Call<BOOL>(this); }

        /*
            uiMenu members
        */
        AGE_API UIButton * AddButton(int id, LocString *text, float x, float y, float w, float h, int fontNum, int type, MM2::datCallback callback, int i3) {
            return hook::Thunk<0x4E1A90>::Call<UIButton*>(this, id, text, x, y, w, h, fontNum, type, callback, i3);
        };

        AGE_API UIIcon* AddIcon(int id, LPCSTR name, float x, float y) {
            return hook::Thunk<0x4E1DC0>::Call<UIIcon*>(this, id, name, x, y);
        };

        AGE_API UIBMLabel* AddBMLabel(int id, LPCSTR name, string* bitmapNames, float x, float y, int* pBitmapIndex =  nullptr)
        {
            return hook::Thunk<0x4E1CB0>::Call<UIBMLabel*>(this, id, name, bitmapNames, x, y, pBitmapIndex);
        }

        AGE_API UIBMButton* AddBMButton(int id, LPCSTR name, float x, float y, int numStates, datCallback onToggle, int* pValue, int radioButtonValue, int soundIndex, datCallback onHover)
        {
            return hook::Thunk<0x4E2340>::Call<UIBMButton*>(this, id, name, x, y, numStates, onToggle, pValue, radioButtonValue, soundIndex, onHover);
        }

        AGE_API UITextRoller2* AddTextRoller2(int id, LPCSTR name, int* pValue, float x, float y, float w, float h, string valuesString, BOOL wrapAround, int maxValue, int nameTextMode, int fontNum, datCallback onValueChanged)
        {
            return hook::Thunk<0x4E1270>::Call<UITextRoller2*>(this, id, name, pValue, x, y, w, h, valuesString, wrapAround, maxValue, nameTextMode, fontNum, onValueChanged);
        }

        AGE_API UITextDropdown* AddTextDropdown(int id, LPCSTR name, int* pValue, float x, float y, float w, float h, string choices, int unused, int nameTextMode, int unused2, datCallback onValueChanged, LPCSTR iconName, datCallback unkCallback)
        {
            return hook::Thunk<0x4E0F90>::Call<UITextDropdown*>(this,id, name, pValue, x, y, w, h, choices, unused, nameTextMode, unused2, onValueChanged, iconName, unkCallback);
        }

        AGE_API UILabel* AddLabel(int id, LocString* text, float x, float y, float w, float h, int effectFlags, int fontType)
        {
            return hook::Thunk<0x4E1BB0>::Call<UILabel*>(this, id, text, x, y, w, h, effectFlags, fontType);
        }

        AGE_API UISlider* AddSlider(int id, LocString* name, float* pValue, float x, float y, float w, float h, float minValue, float maxValue, int unused, int textMode, int unused2, BOOL isBalanceSlider, datCallback onValueChanged, datCallback onFocused)
        {
            return hook::Thunk<0x4E1940>::Call<UISlider*>(this, id, name, pValue, x, y, w, h, minValue, maxValue, unused, textMode, unused2, isBalanceSlider, onValueChanged, onFocused);
        }

        AGE_API void AssignBackground(LPCSTR imageName)
        {
            hook::Thunk<0x4E0980>::Call<void>(this, imageName);
        }

        AGE_API void AssignName(LPCSTR name)
        {
            hook::Thunk<0x4E0930>::Call<void>(this, name);
        }

        void SetPreviousMenu(int ID)
        {
            this->PreviousMenu = ID;
        }

        static void BindLua(LuaState L) {
            LuaBinding(L).beginExtendClass<UIMenu, asNode>("UIMenu")
                .addFactory([](int id) {
                    auto object = new UIMenu(id);
                    return object;
                })
                .addFunction("SetPreviousMenu", &SetPreviousMenu)
                .addFunction("AssignBackground", &AssignBackground)
                .addFunction("AssignName", &AssignName)
                .addFunction("AddButton", &AddButtonLua)
                .addFunction("AddIcon", &AddIcon)
                .addFunction("AddBMLabel", &AddBMLabelLua)
                .addFunction("AddBMButton", &AddBMButtonLua)
                .addFunction("AddTextRoller2", &AddTextRoller2Lua)
                .addFunction("AddTextDropdown", &AddTextDropdownLua)
                .addFunction("AddLabel", &AddLabelLua)
                .addFunction("AddSlider", &AddSliderLua)
                .endClass();
        }
    };
}