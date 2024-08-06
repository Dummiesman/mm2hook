#pragma once
#include <modules\node\node.h>
#include <modules\banger\banger.h>
#include <modules/audio/aud3dambientobject.h>

namespace MM2
{
    // Forward declarations
    class gizFerry;
    class gizFerryMgr;

    // External declarations
    extern class gizPathspline;
    extern class dgPathSet;

    // Class definitions
    class gizFerry : public dgUnhitYBangerInstance
    {
    private:
        gizPathspline* Pathspline;
        Aud3DAmbientObject Audio;
    public:
        /*
            lvlInstance virtuals
        */
        virtual AGE_API void Reset(void) override           { hook::Thunk<0x579350>::Call<void>(this); }
        virtual AGE_API void Draw(int lod) override         { hook::Thunk<0x579A00>::Call<void>(this, lod); }
        virtual AGE_API unsigned int SizeOf(void) override  { return sizeof(gizFerry); }

        /*
            gizFerry
        */
        AGE_API void SetSpeed(float speed)                  { hook::Thunk<0x579520>::Call<void>(this, speed); }

        static void BindLua(LuaState L) {
            LuaBinding(L).beginExtendClass<gizFerry, dgUnhitYBangerInstance>("gizFerry")
                .addFunction("SetSpeed", &SetSpeed)
                .endClass();
        }
    };
    ASSERT_SIZEOF(gizFerry, 0xC0);

    class gizFerryMgr : public asNode
    {
    private:
        int FerryCount;
        dgPathSet* PathSet;
        gizFerry* Ferries;
        float BaseSpeed;
        float SpeedVar;
        float LODDistMod;
        bool PauseUpdate;
    public:
        /*
            asNode virtuals
        */
        AGE_API void Cull() override                { hook::Thunk<0x579820>::Call<void>(this); }
        AGE_API void Reset() override               { hook::Thunk<0x579650>::Call<void>(this); }
        AGE_API void Update() override              { hook::Thunk<0x5797D0>::Call<void>(this); }

        /*
            gizSailboatMgr
        */
        AGE_API bool Init(LPCSTR modelName, LPCSTR pathsetName, LPCSTR raceDir)
                                                            { return hook::Thunk<0x579680>::Call<bool>(this, modelName, pathsetName, raceDir); }

        int GetFerryCount() const { return FerryCount; }
        gizFerry* GetFerry(int id)
        {
            if (id < 0 || id >= GetFerryCount())
                return nullptr; 
            return &Ferries[id];
        }

        // lua
        static void BindLua(LuaState L) {
            LuaBinding(L).beginExtendClass<gizFerryMgr, asNode>("gizFerryMgr")
                .addPropertyReadOnly("NumFerries", &GetFerryCount)
                .addFunction("GetFerry", &GetFerry)
                .endClass();
        }
    };
    ASSERT_SIZEOF(gizFerryMgr, 0x34);
}