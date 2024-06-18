#pragma once
#include <modules\node\node.h>

namespace MM2
{
    // Forward declarations
    class MenuManager;

    // External declarations
    extern class UIMenu;
    extern class asCamera;
    extern class asViewCS;

    // Class definitions
    class MenuManager : public asNode {
    private:
        char _buf[0x138];
    private:
        static hook::Type<MenuManager*> Instance;

        static hook::Field<0x1C, asCamera*> _camera;
        static hook::Field<0x20, asViewCS*> _view;
        static hook::Field<0xEC, int> _activeDialogID;
        static hook::Field<0xF8, UIMenu**> _menus;
        static hook::Field<0x138, int> _currentMenuId;
        static hook::Field<0x114, int> _lastMenuId;
        static hook::Field<0x10C, int> _menuCount;
    public:
        static MenuManager* GetInstance()
        {
            return Instance.get();
        }

        int GetCurrentMenuID() const {
            return _currentMenuId.get(this);
        }

        int GetCurrentDialogID() const {
            return _activeDialogID.get(this);
        }

        int GetMenuCount() const {
            return _menuCount.get(this);
        }

        UIMenu* GetMenu(int index) const
        {
            if (index < 0 || index >= _menuCount.get(this))
                return nullptr;
            return _menus.get(this)[index];
        }

        UIMenu* GetMenuByID(int id) const
        {
            for (int i = 0; i < _menuCount.get(this); i++)
            {
                auto menu = _menus.get(this)[i];
                if (menu->GetID() == id)
                {
                    return menu;
                }
            }
            return nullptr;
        }

        asCamera* GetCamera() const
        {
            return _camera.get(this);
        }

        asViewCS* GetView() const
        {
            return _view.get(this);
        }

       AGE_API int AddMenu2(UIMenu* menu)                                { return hook::Thunk<0x4E5B20>::Call<int>(this, menu); }
       AGE_API void DeleteMenu(UIMenu* menu)                             { hook::Thunk<0x4E5B80>::Call<void>(this); }
       AGE_API void Switch(int menuID)                                   { hook::Thunk<0x4E5A30>::Call<void>(this, menuID); }
       AGE_API void CloseDialog()                                        { hook::Thunk<0x4E5190>::Call<void>(this); }
       AGE_API void OpenDialog(int menuID)                               { hook::Thunk<0x4E5110>::Call<void>(this, menuID); }
       AGE_API void EnableNavBar()                                       { hook::Thunk<0x4E5270>::Call<void>(this); }
       AGE_API void DisableNavBar()                                      { hook::Thunk<0x4E5290>::Call<void>(this); }
       AGE_API void PlaySound(int id) const                              { hook::Thunk<0x4E5320>::Call<void>(this, id); }
       AGE_API UIMenu* GetCurrentMenu() const                            { return hook::Thunk<0x4E58D0>::Call<UIMenu*>(this); }
       AGE_API int FindMenu(int menuID) const                            { return hook::Thunk<0x4E5900>::Call<int>(this, menuID); }
       AGE_API int MenuState(int menuID = -1) const                      { return hook::Thunk<0x4E5960>::Call<int>(this, menuID); }

       static void BindLua(LuaState L) {
           LuaBinding(L).beginExtendClass<MenuManager, asNode>("MenuManager")
               .addStaticProperty("Instance", &GetInstance)
               .addPropertyReadOnly("CurrentMenuID", &GetCurrentMenuID)
               .addPropertyReadOnly("CurrentDialogID", &GetCurrentDialogID)
               .addPropertyReadOnly("NumMenus", &GetMenuCount)
               .addPropertyReadOnly("Camera", &GetCamera)
               .addPropertyReadOnly("View", &GetView)
               .addFunction("AddMenu2", &AddMenu2)
               .addFunction("DeleteMenu", &DeleteMenu)
               .addFunction("Switch", &Switch)
               .addFunction("OpenDialog", &OpenDialog)
               .addFunction("CloseDialog", &CloseDialog)
               .addFunction("EnableNavBar", &EnableNavBar)
               .addFunction("DisableNavBar", &DisableNavBar)
               .addFunction("PlaySound", &PlaySound)
               .addFunction("FindMenu", &FindMenu)
               .addFunction("GetCurrentMenu", &GetCurrentMenu)
               .addFunction("GetMenu", &GetMenu)
               .addFunction("GetMenuByID", &GetMenuByID)
               .endClass();
       }
    };
    ASSERT_SIZEOF(MenuManager, 0x150);
}