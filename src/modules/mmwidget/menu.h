#pragma once
#include "widget.h"

namespace MM2
{
    // Forward declarations
    class UIMenu;

    // External declarations

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
        uint32_t dword64;
        uint32_t *pCurrentWidgetID;
        uint32_t ActiveWidgetID;
        uint32_t dword70;
        uint32_t WidgetID;
        uint32_t dword78;
        uint32_t dword7C;
        float dword80;
        uint32_t dword84;
        const char *Background;

        AGE_API UIButton * AddButton(int unk, LocString *name, float f1, float f2, float f3, float f4, int i1, int i2, MM2::datCallback callback, int i3) {
            return hook::Thunk<0x4E1A90>::Call<UIButton*>(this, unk, name, f1, f2, f3, f4, i1, i2, callback, i3);
        };
    };
}