#pragma once
#include <modules\mmwidget\menu.h>

namespace MM2
{
    // Forward declarations
    class PUMenuBase;

    // External declarations


    // Class definitions
    class PUMenuBase : public UIMenu
    {
    private:
        int dword_8c;
        gfxBitmap* BMBackground;
        UIButton* pExitButton;
        int pixelPosX;
        int pixelPosY;
        int cornerButtonFont;
        float dword_a4;
        float dword_a8;
        float cornerButtonX;
        float cornerButtonY;
        float cornerButtonW;
        float cornerButtonH;
    public:
        AGE_API PUMenuBase(int id, float x, float y, float w, float h, LPCSTR backgroundImageName, bool scaleBackground)
        {
            scoped_vtable v(this);
            hook::Thunk<0x50CC90>::Call<void>(this, id, x, y, w, h, backgroundImageName, scaleBackground);
        }

        AGE_API ~PUMenuBase()
        {
            scoped_vtable v(this);
            hook::Thunk<0x50CE70>::Call<void>(this);
        }

        /*
            asNode virtuals
        */

        virtual AGE_API void Update(void) override          { hook::Thunk<0x50D170>::Call<void>(this); };
        virtual AGE_API void Cull(void) override            { hook::Thunk<0x50D1A0>::Call<void>(this); };

        /*
            PUMenuBase members
        */
        
        AGE_API void EnableExit()                           { hook::Thunk<0x50D1F0>::Call<void>(this); }
        AGE_API void DisableExit()                          { hook::Thunk<0x50D1E0>::Call<void>(this); }
        AGE_API float CreateTitle(int a2)                   { return hook::Thunk<0x50CF90>::Call<float>(this, a2); }
        AGE_API float AddPrevious(float a2, float a3, float a4, float a5)  
                                                            { return hook::Thunk<0x50CFD0>::Call<float>(this, a2, a3, a4, a5); }
        AGE_API void AddOKCancel(datCallback cancelCB, datCallback okCB)
                                                            { hook::Thunk<0x50D050>::Call<void>(this, cancelCB, okCB); }

        static void BindLua(LuaState L) {
            LuaBinding(L).beginExtendClass<PUMenuBase, UIMenu>("PUMenuBase")
                .addFactory([](int id, float x, float y, float w, float h, LuaRef backgroundImageName, bool scaleBackground)
                {
                    LPCSTR bgImageName = nullptr;
                    if (backgroundImageName.type() != LuaTypeID::NIL)
                    {
                        bgImageName = backgroundImageName.toValue<LPCSTR>();
                    }

                    auto object = new PUMenuBase(id, x, y, w, h, bgImageName, scaleBackground);
                    return object;
                })
                .endClass();
        }
    };
}