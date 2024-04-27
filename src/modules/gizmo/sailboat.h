#pragma once
#include <modules\node\node.h>
#include <modules\gizmo\gizmoinst.h>

namespace MM2
{
    // Forward declarations
    class gizSailboat;
    class gizSailboatMgr;

    // External declarations
    extern class gizPathspline;
    extern class dgPathSet;

    // Class definitions
    class gizSailboat : public gizInstance
    {
    private:
        gizPathspline* Spline;
    public:
        /*
            lvlInstance virtuals
        */
        virtual AGE_API void Reset(void) override           { hook::Thunk<0x578320>::Call<void>(this); }
        virtual AGE_API unsigned int SizeOf(void) override  { return sizeof(gizSailboat); }

        /*
            gizSailboat
        */
        AGE_API void SetSpeed(float speed)                  { hook::Thunk<0x578490>::Call<void>(this, speed); }

        static void BindLua(LuaState L) {
            LuaBinding(L).beginExtendClass<gizSailboat, gizInstance>("gizSailboat")
                .addFunction("SetSpeed", &SetSpeed)
                .endClass();
        }
    };
    ASSERT_SIZEOF(gizSailboat, 0x50);

    class gizSailboatMgr : public asNode
    {
    private:
        int SailboatCount;
        dgPathSet* PathSet;
        gizSailboat* Sailboats;
        float BaseSpeed;
        float SpeedVariation;
    public:
        /*
            asNode virtuals
        */
        AGE_API void Reset() override               { hook::Thunk<0x5785C0>::Call<void>(this); }
        AGE_API void Update() override              { hook::Thunk<0x578770>::Call<void>(this); }

        /*
            gizSailboatMgr
        */
        AGE_API bool Init(LPCSTR modelName, LPCSTR pathsetName, LPCSTR raceDir)
                                                            { return hook::Thunk<0x5785F0>::Call<bool>(this, modelName, pathsetName, raceDir); }

        int GetSailboatCount() const { return SailboatCount; }
        gizSailboat* GetSailboat(int id)
        {
            if (id < 0 || id >= GetSailboatCount())
                return nullptr; 
            return &Sailboats[id]; 
        }

        // lua
        static void BindLua(LuaState L) {
            LuaBinding(L).beginExtendClass<gizSailboatMgr, asNode>("gizSailboatMgr")
                .addPropertyReadOnly("NumSailboats", &GetSailboatCount)
                .addFunction("GetSailboat", &GetSailboat)
                .endClass();
        }
    };
    ASSERT_SIZEOF(gizSailboatMgr, 0x2C);
}