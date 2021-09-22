#pragma once
#include <modules\input.h>

namespace MM2
{
    // Forward declarations


    // External declarations


    // Class definitions
    class ioKeyboard {
    private:
        static bool getActiveFlag()
        {
            return (*(int*)0x6A387C) != 0;
        }

        static byte getKeyState(bool active, byte keycode)
        {
            byte* keyStatesPtr = (byte*)0x6A3674; //m_Keys
            int base = active ? 256 : 0;
            return keyStatesPtr[base + keycode];
        }
    public:
        AGE_API static void Update() {
            hook::StaticThunk<0x4BABA0>::Call<void>();
        }

        static void ClearStates() {
            for (int i = 0; i < 256; i++) {
                SetKeyState(i, 0);
            }
        }

        //todo: don't use these evil magic numbers
        static void SetKeyState(byte keycode, byte state) {
            int activeFlag = *(int*)0x6A387C; //ioKeyboard::m_Active
            byte* keyStatesPtr = (byte*)0x6A3674; //m_Keys
            keyStatesPtr[(256 * activeFlag) + keycode] = state;
        }

        static bool GetKey(byte keycode) {
            return getKeyState(getActiveFlag(), keycode) != 0;
        }

        static bool GetKeyDown(byte keycode) {

            return ((getKeyState(false, keycode) ^ getKeyState(true, keycode)) & getKeyState(getActiveFlag(), keycode)) != 0;
        }

        static bool GetKeyUp(byte keycode) {
            return ((getKeyState(false, keycode) ^ getKeyState(true, keycode)) & getKeyState(!getActiveFlag(), keycode)) != 0;
        }

        //lua
        static void BindLua(LuaState L) {
            LuaBinding(L).beginClass<ioKeyboard>("ioKeyboard")
                .addStaticFunction("GetKey", &GetKey)
                .addStaticFunction("GetKeyDown", &GetKeyDown)
                .addStaticFunction("GetKeyUp", &GetKeyUp)
                .endClass();
        }
    };

    // Lua initialization

}