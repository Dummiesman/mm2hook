#pragma once
#include "pathspline.h"

namespace MM2
{
    // Forward declarations
    class gizPathsplineWrapper;

    // External declarations
    extern class dgPath;

    // Class definitions
    class gizPathsplineWrapper : public gizPathspline {
    private:
        Vector3 position;
        Vector3 forward;
    public:
        Matrix34 GetMatrix()
        {
            Matrix34 mtx;
            mtx.Identity();
            mtx.SetRow(2, forward);
            mtx.SetRow(3, position);
            mtx.Normalize();
            return mtx;
        }

        Vector3 GetPosition()
        {
            return position;
        }

        void Update(float t) 
        {
            gizPathspline::Update(position, forward, t);
        }

        static void BindLua(LuaState L) {
            LuaBinding(L).beginClass<gizPathsplineWrapper>("gizPathspline")
                .addConstructor(LUA_ARGS())
                .addProperty("Speed", &getSpeed, &SetSpeed)
                .addFunction("Init", &Init)
                .addFunction("Update", &Update)
                .addFunction("GetPosition", &GetPosition)
                .addFunction("GetMatrix", &GetMatrix)
                .addFunction("Reset", &Reset)
            .endClass();
        }
    };
}