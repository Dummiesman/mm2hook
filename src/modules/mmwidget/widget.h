#pragma once
#include <modules\node.h>

namespace MM2
{
    // Forward declarations
    class mmWidget;

    // External declarations
    extern class UIMenu;
    extern class mmToolTip;

    // Class definitions
    class uiWidget : public asNode {
    private:
        bool getEnabledLua()
        {
            return this->Enabled == TRUE;
        }

        void setEnabledLua(bool enabled)
        {
            if (enabled)
                this->Enable();
            else
                this->Disable();
        }
    public:
        UIMenu *pParent;
        Vector2 MinPos;
        Vector2 MaxPos;
        Vector2 LastMousePos;
        uint32_t Selected;
        uint32_t LastMouseAction;
        uint32_t dword3C;
        uint32_t WidgetID;
        BOOL ReadOnly;
        const char *Name;
        uint32_t dword4C;
        Vector2 Position;
        Vector2 Size;
        BOOL Enabled;
        uint32_t dword64;
        mmToolTip *pTooltip;
    public:
        virtual AGE_API void Disable(void)                  { hook::Thunk<0x4E7330>::Call<void>(this); }
        virtual AGE_API void Enable(void)                   { hook::Thunk<0x4E7340>::Call<void>(this); }
        virtual AGE_API void TurnOn(void)                   { hook::Thunk<0x4E7350>::Call<void>(this); }
        virtual AGE_API void TurnOff(void)                  { hook::Thunk<0x4E7370>::Call<void>(this); }
        virtual AGE_API void SetReadOnly(BOOL readonly)     { hook::Thunk<0x4E7380>::Call<void>(this, readonly); }
        virtual AGE_API void Action(eqEvent event)          { hook::Thunk<0x4E73A0>::Call<void>(this, event); }
        virtual AGE_API void CaptureAction(eqEvent event)   { hook::Thunk<0x4E73B0>::Call<void>(this, event); }
        virtual AGE_API void Switch(int a1)                 { hook::Thunk<0x4E7480>::Call<void>(this, a1); }
        virtual AGE_API void EvalMouseX(float x)            { hook::Thunk<0x4E73C0>::Call<void>(this, x); }
        virtual AGE_API char * ReturnDescription(void)      { return hook::Thunk<0x4E73D0>::Call<char *>(this); }
        virtual AGE_API void SetPosition(float x, float y)
                                                            { hook::Thunk<0x4E73E0>::Call<void>(this, x, y); }
        virtual AGE_API float GetScreenHeight(void)         { return hook::Thunk<0x4E73F0>::Call<float>(this); }

        const char* GetName() const
        {
            return Name;
        }

        static void BindLua(LuaState L) {            
            LuaBinding(L).beginClass<uiWidget>("uiWidget")
                .addProperty("Enabled", &getEnabledLua, &setEnabledLua)
                .addPropertyReadOnly("Name", &GetName)
                .addFunction("Enable", &Enable)
                .addFunction("Disable", &Disable)
                .addFunction("TurnOn", &TurnOn)
                .addFunction("TurnOff", &TurnOff)
                .endClass();
        }
    };
}