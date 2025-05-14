#pragma once
#include <modules/vector.h>
#include <modules/level/inst.h>

namespace MM2
{
    // Forward declarations
    class vehBreakable;
    class vehBreakableMgr;

    // External declarations
    extern class modStatic;
    
    class vehBreakable 
    {
    public:
        bool isAttached; // still attached to the parent?
        int index; // index into the list of breakables
        Matrix34 matrix;
        lvlInstance::GeomTableEntry* model;
        int bangerDataIndex;
        int geometryIndex;
        lvlInstance* instance;
        vehBreakable *next;
    public:
        AGE_API vehBreakable(modStatic** a1, Matrix34* a2, int a3, int a4, int a5);
        AGE_API void Add(vehBreakable* breakable);

        static void BindLua(LuaState L);
    };

    class vehBreakableMgr 
    {
    public:
        static bool EnableReflections;
    private:
        vehBreakable *first; // linked-list
        float maxImpact; // educated guess (default: 10000.0)
        Matrix34 *matrix; // origin?
        int variant; // always zero by default, used in Eject to set Banger variant

        // TODO: figure out what these do

        float p5; // (default: 4.0)
        float p6; // (default: 0.0)
        float p7; // (default: 2.0)
        float p8; // (default: 0.0)
    public:
        int GetVariant();
        void SetVariant(int variant);
    public:
        ANGEL_ALLOCATOR

        AGE_API vehBreakableMgr();
        AGE_API ~vehBreakableMgr();

        AGE_API bool Init(Matrix34* a1);
        AGE_API void Add(LPCSTR a1, LPCSTR a2, modStatic** a3, int a4, int a5);
        AGE_API void Create(LPCSTR a1, LPCSTR a2, modStatic** a3, int a4, int a5);
        AGE_API void Eject(vehBreakable* a1, int room);
        AGE_API void EjectAll(int room);
        AGE_API void Reset();
        AGE_API void Draw(const Matrix34& world, modShader* shaders, int lod);
        AGE_API vehBreakable* Get(int index);

        static void BindLua(LuaState L);
    };

    ASSERT_SIZEOF(vehBreakable, 0x4C);
    ASSERT_SIZEOF(vehBreakableMgr, 0x20);
}