#pragma once
#include <modules\effects.h>

namespace MM2
{
    // Forward declarations
    class asParticles;

    // External declarations


    // Class definitions
    class asParticles {
    public:
        uint dword4;
        int SparkCount;
        uint dwordC;
        struct asSparkPos *pSparks;
        gfxTexture *Texture;
        asMeshCardInfo MeshCardInfo;
        uint32_t field_28;
        uint32_t field_2C;
        float dword30;
        uint dword34;
        uint dword38;
        asBirthRule *pBirthRule;
        uint dword40;
        uint dword44;
        uint dword48;
        uint dword4C;
        float Gravity;
    public:
        ANGEL_ALLOCATOR;

        AGE_API asParticles(void) {
            scoped_vtable x(this);
            hook::Thunk<0x460EB0>::Call<void>(this);
        }

        AGE_API virtual ~asParticles(void) {
            scoped_vtable x(this);
            hook::Thunk<0x460F20>::Call<void>(this);
        }

        void InitLua(int count, int wt, int ht) {
            Init(count, wt, ht, 4, nullptr);
        }

        AGE_API void Init(int nParticles, int nTilesW, int nTilesH, int nVertices, asMeshCardVertex *mesh)
                                                            { hook::Thunk<0x460FB0>::Call<void>(this, nParticles, nTilesW, nTilesH, nVertices, mesh); }
        AGE_API void Blast(int a1, asBirthRule *rule)       { hook::Thunk<0x461490>::Call<void>(this, a1, rule); }
        AGE_API void Reset()                                { hook::Thunk<0x461040>::Call<void>(this); }
        AGE_API void Update()                               { hook::Thunk<0x4610F0>::Call<void>(this); }
        AGE_API void SetTexture(const char *tex)            { hook::Thunk<0x461090>::Call<void>(this, tex); }
        AGE_API void SetTexture(gfxTexture *tex)            { hook::Thunk<0x461050>::Call<void>(this, tex); }

        //member hlepers
        inline asBirthRule * getBirthRule(void) {
            return pBirthRule;
        }

        inline void setBirthRule(asBirthRule *rule) {
            pBirthRule = rule;
        }

        /*
            asParticles Virtuals
        */
        virtual AGE_API void Cull()                         { hook::Thunk<0x4615A0>::Call<void>(this); }

        //lua
        static void BindLua(LuaState L) {
            LuaBinding(L).beginClass<asParticles>("asParticles")
                .addConstructor(LUA_ARGS())
                .addFunction("Blast", &Blast)
                .addFunction("Update", &Update)
                .addFunction("Init", &InitLua)
                .addFunction("Reset", &Reset)
                .addFunction("Cull", &Cull)
                .addFunction<void (asParticles::*)(const char* a1)>("SetTexture", &SetTexture)
                .addProperty("BirthRule", &getBirthRule, &setBirthRule)
            .endClass();
        }
    };

    // Lua initialization

}