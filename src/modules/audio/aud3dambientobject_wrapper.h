#pragma once
#include <mm2_common.h>
#include "aud3dambientobject.h"

namespace MM2
{
    // Forward declarations
    class Aud3DAmbientObjectWrapper;

    // External declarations

    // Class definitions
    class Aud3DAmbientObjectWrapper : public Aud3DAmbientObject {
    private:
        Vector3 position;
    public:
        Aud3DAmbientObjectWrapper() 
        {
        }

        /*
            Aud3DAmbientObject
        */
        void Init(LPCSTR csvName) {
            Aud3DAmbientObject::Init(&position, csvName);
        }

        void SetPosition(Vector3 position) {
            this->position = position;
        }

        Vector3 GetPosition() const {
            return this->position;
        }

        static void BindLua(LuaState L) {
            LuaBinding(L).beginExtendClass<Aud3DAmbientObjectWrapper, Aud3DAmbientObject>("Aud3DAmbientObjectWrapper")
                .addFactory([]() { return new Aud3DAmbientObjectWrapper(); })
                .addFunction("Init", &Init)
                .addFunction("SetPosition", &SetPosition)
                .addFunction("GetPosition", &GetPosition)
                .endClass();
        }
    };
}