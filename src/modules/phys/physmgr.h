#pragma once
#include <modules\phys\segment.h>
#include <modules\phys\phintersection.h>

namespace MM2
{
    // Forward declarations
    class dgPhysManager;

    // External declarations
    extern class lvlInstance;
    extern struct lvlSegment;
    extern struct phIntersectionPoint;
    extern struct lvlIntersection;

    // Class definitions
    class dgPhysManager {
    private:
        //lua collide, returns tuple of (hit, distance, position, normal)
        phIntersectionPoint* collideLua(Vector3 start, Vector3 end) 
        {
            lvlSegment segment;
            lvlIntersection isect;
            segment.Set(start, end, 0, nullptr);
            
            bool collided = dgPhysManager::Collide(segment, &isect, 0, nullptr, 0x20, 0);
            return collided ? new phIntersectionPoint(isect.IntersectionPoint) : nullptr;
        }
    public:
        static inline float getGravity() 
        {
            return dgPhysManager::Gravity.get();
        }

        static inline void setGravity(float gravity) 
        {
            dgPhysManager::Gravity.set(gravity);
        }
    public:
        static hook::Type<dgPhysManager *> Instance;
        static hook::Type<float> Gravity;

        AGE_API void IgnoreMover(lvlInstance* instance)                  { hook::Thunk<0x468860>::Call<void>(this, instance); }
        AGE_API void DeclareMover(lvlInstance* instance, int a2, int a3) { hook::Thunk<0x468370>::Call<void>(this, instance, a2, a3); }
        AGE_API bool Collide(lvlSegment& segment, lvlIntersection* intersection, int a4, lvlInstance* ignoreInstance, ushort flags1, int flags2 )
                                                                         { return hook::Thunk<0x468E40>::Call<bool>(this, &segment, intersection, a4, ignoreInstance, flags1, flags2); }

        static void BindLua(LuaState L) {
            LuaBinding(L).beginClass<dgPhysManager>("dgPhysManager")
                //statics
                .addStaticProperty("Instance", [] {return (dgPhysManager *)Instance; })

                //properties
                .addStaticProperty("Gravity", &getGravity, &setGravity)
                 
                //functions
                .addFunction("Collide", &collideLua)
                .addFunction("IgnoreMover", &IgnoreMover)
                .addFunction("DeclareMover", &DeclareMover)

                .endClass();
        }
    };

    // Lua initialization

}