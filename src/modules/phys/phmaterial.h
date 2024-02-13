#pragma once

namespace MM2
{
    // Forward declarations
    class phMaterial;

    // External declarations
    extern class datAsciiTokenizer;
    extern class Stream;

    // Class definitions
    class phMaterial {
    private:
        char Name[32];
        short EffectIndex;
        short SoundIndex;
        float Elasticity;
        float Friction;
    public:
        AGE_API phMaterial() {
            scoped_vtable x(this);
            hook::Thunk<0x492FF0>::Call<void>(this);
        }

        /*
            phMaterial Virtuals
        */
        virtual AGE_API void Copy(const phMaterial &copyfrom)   { hook::Thunk<0x493270>::Call<void>(this, &copyfrom);}
        virtual AGE_API void Save(datAsciiTokenizer *writer)    { hook::Thunk<0x493160>::Call<void>(this, writer);}
        virtual AGE_API void SaveBinary(Stream *stream)         { hook::Thunk<0x493340>::Call<void>(this, stream); }

        /*
            phMaterial
        */
        AGE_API void SetName(const char *name)                  { hook::Thunk<0x493050>::Call<void>(this, name);}
        AGE_API void Load(datAsciiTokenizer *reader)            { hook::Thunk<0x493070>::Call<void>(this, reader); }
        AGE_API void LoadBinary(Stream *stream)                 { hook::Thunk<0x4932A0>::Call<void>(this, stream); }

        const char* GetName() const {
            return this->Name;
        }

        static void BindLua(LuaState L) {
            LuaBinding(L).beginClass<phMaterial>("phMaterial")
                //properties
                .addFactory([]() -> phMaterial* { return new phMaterial(); })
                .addVariable("EffectIndex", &phMaterial::EffectIndex)
                .addVariable("SoundIndex", &phMaterial::SoundIndex)
                .addVariable("Elasticity", &phMaterial::Elasticity)
                .addVariable("Friction", &phMaterial::Friction)
                .addProperty("Name", &GetName, &SetName)

                .addFunction("Load", &Load)
                .addFunction("LoadBinary", &LoadBinary)
                .addFunction("Copy", &Copy)

                .endClass();
        }
    };

    ASSERT_SIZEOF(phMaterial, 0x30);
}