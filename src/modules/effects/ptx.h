#pragma once
#include <modules\effects.h>

namespace MM2
{
    // Forward declarations
    class asParticles;

    // External declarations


    // Class definitions
    class asParticles {
    private:
        uint dword4;
        int SparkCount;
        uint dwordC;
        struct asSparkPos *pSparks;
        gfxTexture *Texture;
        asMeshCardInfo MeshCardInfo;
        uint32_t field_28;
        uint32_t field_2C;
        float Intensity;
        uint SpewRate;
        uint dword38;
        asBirthRule *pBirthRule;
        Vector3 Wind;
        float WindDensity;
        float Gravity;
    private:
        void initLua(int count, int wt, int ht) {
            Init(count, wt, ht, 4, nullptr);
        }
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

        AGE_API void Init(int nParticles, int nTilesW, int nTilesH, int nVertices, asMeshCardVertex *mesh)
                                                            { hook::Thunk<0x460FB0>::Call<void>(this, nParticles, nTilesW, nTilesH, nVertices, mesh); }
        AGE_API void Blast(int a1, asBirthRule *rule)       { hook::Thunk<0x461490>::Call<void>(this, a1, rule); }
        AGE_API void Reset()                                { hook::Thunk<0x461040>::Call<void>(this); }
        AGE_API void Update()                               { hook::Thunk<0x4610F0>::Call<void>(this); }
        AGE_API void SetTexture(const char *tex)            { hook::Thunk<0x461090>::Call<void>(this, tex); }
        AGE_API void SetTexture(gfxTexture *tex)            { hook::Thunk<0x461050>::Call<void>(this, tex); }

        //member hlepers
        int GetSparkCount() const {
            return this->SparkCount;
        }

        asBirthRule * GetBirthRule() const {
            return pBirthRule;
        }

        void SetBirthRule(asBirthRule *rule) {
            pBirthRule = rule;
        }

        Vector3 GetWind() const {
            return this->Wind;
        }

        void SetWind(Vector3 wind) {
            this->Wind = wind;
        }

        float GetIntensity() const {
            return this->Intensity;
        }

        void SetIntensity(float intensity) {
            this->Intensity = intensity;
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
                .addFunction("Init", &initLua)
                .addFunction("Reset", &Reset)
                .addFunction("Cull", &Cull)
                .addFunction<void (asParticles::*)(const char* a1)>("SetTexture", &SetTexture)
                .addProperty("BirthRule", &GetBirthRule, &SetBirthRule)
                .addProperty("Wind", &GetWind, &SetWind)
                .addProperty("Intensity", &GetIntensity, &SetIntensity)
                .addPropertyReadOnly("SparkCount", &GetSparkCount)
            .endClass();
        }
    };
    ASSERT_SIZEOF(asParticles, 0x54);

    // Lua initialization

}