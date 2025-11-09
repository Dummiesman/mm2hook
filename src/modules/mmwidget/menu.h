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
    extern class UITextField;

    // Class definitions
    class UIMenu : public asNode {
    public:
        uint32_t ActionSource;
        uint32_t ActionState;
        uint32_t dword20;
        int ID;
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
        int FocusWidgetIndex;
        float dword80;
        uint32_t dword84;
        const char *Background;
    public:
        static float WIDGET_HEIGHT;
        static float WIDGET_WIDTH;
        static float UI_LEFT_MARGIN2;
        static float UI_LEFT_MARGIN;
        static float UI_TOP_MARGIN;
        static float UI_BOTTOM_MARGIN;
    private:
        // lua helpers
        UIButton* AddButtonLua(int id, LPCSTR text, float x, float y, float w, float h, int fontNum, int type, LuaRef callback, bool unscaled)
        {
            return this->AddButton(id, (LocString*)text, x, y, w, h, fontNum, type, *datCallback::CreateParamaterlessLuaCallback(callback), (unscaled) ? TRUE : FALSE);
        }

        UIBMLabel* AddBMLabelLua(int id, LPCSTR name, LPCSTR bitmapNames, float x, float y, IntBox* pBoxedValue)
        {
            auto tempString = string(bitmapNames);
            return this->AddBMLabel(id, name, &tempString, x, y, pBoxedValue->GetBoxedValuePointer());
        }

        UIBMButton* AddBMButtonLua(int id, LPCSTR name, float x, float y, int numStates, LuaRef onToggleCallback, LuaRef pBoxedValue, int radioButtonValue, int soundIndex, LuaRef hoverCallback);

        UITextRoller2* AddTextRoller2Lua(int id, LPCSTR name, IntBox* pBoxedValue, float x, float y, float w, float h, LPCSTR valuesString, bool wrapAround, int maxValue, int nameTextMode, int fontNum, LuaRef onValueChangedCallback)
        {
            auto tempString = string(valuesString);
            return this->AddTextRoller2(id, name, pBoxedValue->GetBoxedValuePointer(), x, y, w, h, tempString, wrapAround ? TRUE : FALSE, maxValue, nameTextMode, fontNum, *datCallback::CreateParamaterlessLuaCallback(onValueChangedCallback));
        }

        uiWidget* AddTextRollerLua(int id, LPCSTR name, IntBox* pBoxedValue, float x, float y, float w, float h, LPCSTR valuesString, bool wrapAround, int maxValue, int nameTextMode, int fontNum, LuaRef onValueChangedCallback)
        {
            auto tempString = string(valuesString);
            return this->AddTextRoller(id, name, pBoxedValue->GetBoxedValuePointer(), x, y, w, h, tempString, wrapAround ? TRUE : FALSE, maxValue, nameTextMode, fontNum, *datCallback::CreateParamaterlessLuaCallback(onValueChangedCallback));
        }

        UITextDropdown* AddTextDropdownLua(int id, LPCSTR name, IntBox* pBoxedValue, float x, float y, float w, float h, LPCSTR choicesString, int nameTextMode, LuaRef onValueChangedCallback, LPCSTR backgroundName, LuaRef hoverCallback);

        UILabel* AddLabelLua(int id, LPCSTR text, float x, float y, float w, float h, int effectFlags, int fontType)
        {
            return this->AddLabel(id, (LocString*)text, x, y, w, h, effectFlags, fontType);
        }

        UISlider* AddSliderLua(int id, LPCSTR name, FloatBox* pValue, float x, float y, float w, float h, float minValue, float maxValue, int textMode, bool isBalanceSlider, LuaRef onValueChangedCallback, LuaRef hoverCallback);
        UITextField* AddTextFieldLua(int id, LPCSTR name, CharBox* buffer, float x, float y, float w, float h, int maxLength, int somelen2, int someflags, int fontSize, LuaRef onChangedCallback);
        uiWidget* AddIconWLua(int id, LPCSTR name, LPCSTR bitmap, float x, float y, float w, float h, LuaRef callback);
        uiWidget* AddToggleLua(int id, LPCSTR name, IntBox* pValue, float x, float y, float w, float h, int fontSize, int buttonType, LuaRef callback);
        uiWidget* AddVScrollBarLua(int id, IntBox* pValue, float x, float y, float w, float h, float rangeMin, float rangeMax, int a10, int a11, LuaRef callback);

        int luaWidgetUpcast(lua_State* L, uiWidget* widget)
        {
            if (widget == nullptr)
            {
                LuaState(L).push(nullptr);
            }
            else 
            {
                auto vtblPtr = *reinterpret_cast<uintptr_t*>(widget);
                switch (vtblPtr)
                {
                case 0x5B3778:
                    LuaState(L).push(reinterpret_cast<UIBMLabel*>(widget));
                    break;
                case 0x5B3714:
                    LuaState(L).push(reinterpret_cast<UILabel*>(widget));
                    break;
                case 0x5B364C:
                    LuaState(L).push(reinterpret_cast<UISlider*>(widget));
                    break;
                case 0x5B33E0:
                    LuaState(L).push(reinterpret_cast<UITextDropdown*>(widget));
                    break;
                case 0x5B34B4:
                    LuaState(L).push(reinterpret_cast<UITextRoller2*>(widget));
                    break;
                case 0x5B3310:
                    LuaState(L).push(reinterpret_cast<UITextField*>(widget));
                    break;
                case 0x5B39FC:
                    LuaState(L).push(reinterpret_cast<UIBMButton*>(widget));
                    break;
                default:
                    LuaState(L).push(widget);
                    break;
                }
            }
            return 1;
        }

        int luaGetWidgetByID(lua_State* L, int id)
        {
            return luaWidgetUpcast(L, GetWidgetByID(id));
        }

        int luaGetWidget(lua_State* L, int index)
        {
            return luaWidgetUpcast(L, GetWidget(index));
        }
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
        AGE_API UIButton * AddButton(int id, LocString *text, float x, float y, float w, float h, int fontNum, int type, MM2::datCallback callback, BOOL unscaled) {
            return hook::Thunk<0x4E1A90>::Call<UIButton*>(this, id, text, x, y, w, h, fontNum, type, callback, unscaled);
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

        AGE_API uiWidget* AddTextRoller(int id, LPCSTR name, int* pValue, float x, float y, float w, float h, string valuesString, BOOL wrapAround, int maxValue, int nameTextMode, int fontNum, datCallback onValueChanged)
        {
            return hook::Thunk<0x4E1120>::Call<uiWidget*>(this, id, name, pValue, x, y, w, h, valuesString, wrapAround, maxValue, nameTextMode, fontNum, onValueChanged);
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

        AGE_API UITextField* AddTextField(int id, LPCSTR name, char* buffer, float x, float y, float w, float h, int maxLength, int somelen2, int someflags, int fontSize, int UNUSED, datCallback onTextChanged)
        {
            return hook::Thunk<0x4E1700>::Call<UITextField*>(this, id, name, buffer, x, y, w, h, maxLength, somelen2, someflags, fontSize, UNUSED, onTextChanged);
        }

        AGE_API uiWidget* AddIconW(int id, LPCSTR name, LPCSTR bitmap, float x, float y, float w, float h, datCallback callback)
        {
            return hook::Thunk<0x4E1ED0>::Call<uiWidget*>(this, id, name, bitmap, x, y, w, h, callback);
        }

        AGE_API uiWidget* AddToggle(int id, LPCSTR name, int* pValue, float x, float y, float w, float h, int fontSize, int buttonType, datCallback callback)
        {
            return hook::Thunk<0x4E1FE0>::Call<uiWidget*>(this, id, name, pValue, x, y, w, h, fontSize, buttonType, callback);
        }

        AGE_API uiWidget* AddVScrollBar(int id, int* pValue, float x, float y, float w, float h, float rangeMin, float rangeMax, int a10, int a11, datCallback callback)
        {
            return hook::Thunk<0x4E2610>::Call<uiWidget*>(this, id, pValue, x, y, w, h, rangeMin, rangeMax, a10, a11, callback);
        }

        AGE_API void AssignBackground(LPCSTR imageName)
        {
            hook::Thunk<0x4E0980>::Call<void>(this, imageName);
        }

        AGE_API void AssignName(LPCSTR name)
        {
            hook::Thunk<0x4E0930>::Call<void>(this, name);
        }

        AGE_API void SetBstate(int state)
        {
            hook::Thunk<0x4E0B20>::Call<void>(this, state);
        }

        AGE_API void SetFocusWidget(int widgetId)
        {
            hook::Thunk<0x4E0B50>::Call<void>(this, widgetId);
        }

        int GetID() const
        {
            return this->ID;
        }

        int GetWidgetCount() const
        {
            return this->WidgetCount;
        }

        int GetWidgetID() const
        {
            return this->WidgetID;
        }


        uiWidget* GetWidget(int index)
        {
            if (ppWidgets == nullptr || index < 0 || index >= this->GetWidgetCount())
                return nullptr;
            return ppWidgets[index];
        }

        uiWidget* GetWidgetByID(int id)
        {
            if (ppWidgets == nullptr)
                return nullptr;
            for (int i = 0; i < this->GetWidgetCount(); i++)
            {
                auto widget = ppWidgets[i];
                if (widget->m_WidgetID == id)
                {
                    return widget;
                }
            }
            return nullptr;
        }

        void SetPreviousMenu(int ID)
        {
            this->PreviousMenu = ID;
        }

        const char* GetName() const
        {
            return this->Name;
        }

        static void BindLua(LuaState L) {
            LuaBinding(L).beginExtendClass<UIMenu, asNode>("UIMenu")
                .addFactory([](int id) {
                    auto object = new UIMenu(id);
                    return object;
                })
                .addPropertyReadOnly("ID", &GetID)
                .addPropertyReadOnly("NumWidgets", &GetWidgetCount)
                .addPropertyReadOnly("WidgetID", &GetWidgetID)
                .addPropertyReadOnly("Name", &GetName)
                .addFunction("GetWidget", &luaGetWidget)
                .addFunction("GetWidgetByID", &luaGetWidgetByID)
                .addFunction("SetPreviousMenu", &SetPreviousMenu)
                .addFunction("SetBstate", &SetBstate)
                .addFunction("AssignBackground", &AssignBackground)
                .addFunction("AssignName", &AssignName)
                .addFunction("AddButton", &AddButtonLua)
                .addFunction("AddIcon", &AddIcon)
                .addFunction("AddBMLabel", &AddBMLabelLua)
                .addFunction("AddBMButton", &AddBMButtonLua)
                .addFunction("AddTextRoller", &AddTextRollerLua)
                .addFunction("AddTextRoller2", &AddTextRoller2Lua)
                .addFunction("AddTextDropdown", &AddTextDropdownLua)
                .addFunction("AddTextField", &AddTextFieldLua)
                .addFunction("AddLabel", &AddLabelLua)
                .addFunction("AddSlider", &AddSliderLua)
                .addFunction("AddIconW", &AddIconWLua)
                .addFunction("AddToggle", &AddToggleLua)
                .addFunction("AddVScrollBar", &AddVScrollBarLua)
                .endClass();
        }
    };
}