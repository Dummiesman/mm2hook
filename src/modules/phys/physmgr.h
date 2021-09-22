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
    struct LuaRaycastResult 
    {
    private:
        lvlSegment* segment;
        lvlIntersection* isect;
    private:
        inline Vector3 getIntersectionPoint()
        {
            return isect->IntersectionPoint.Point;
        }

        inline Vector3 getNormal()
        {
            return isect->IntersectionPoint.Normal;
        }

        inline float getNormalizedDistance()
        {
            return isect->IntersectionPoint.NormalizedDistance;
        }

        inline float getPenetration()
        {
            return isect->IntersectionPoint.Penetration;
        }

        inline phBound* getBound()
        {
            return isect->Bound;
        }

        inline phPolygon* getPolygon()
        {
            return isect->Poly;
        }
    public:
        LuaRaycastResult(lvlSegment* segment, lvlIntersection* isect) 
        {
            this->segment = segment;
            this->isect = isect;
        }

        static void BindLua(LuaState L) {
            LuaBinding(L).beginClass<LuaRaycastResult>("LuaRaycastResult")
                .addPropertyReadOnly("NormalizedDistance", &getNormalizedDistance)
                .addPropertyReadOnly("Penetration", &getPenetration)
                .addPropertyReadOnly("Point", &getIntersectionPoint)
                .addPropertyReadOnly("Bound", &getBound)
                .addPropertyReadOnly("Normal", &getNormal)
                .addPropertyReadOnly("Polygon", &getPolygon)
                .endClass();
        }
    };

    class dgPhysManager {
    private:
        std::shared_ptr<LuaRaycastResult> collideLua(Vector3 start, Vector3 end)
        {
            lvlSegment segment;
            lvlIntersection isect;
            segment.Set(start, end, 0, nullptr);
            
            bool collided = dgPhysManager::Collide(segment, &isect, 0, nullptr, 0x20, 0);
            LuaRaycastResult* result = (collided) ? new LuaRaycastResult(&segment, &isect) : nullptr;
            return std::shared_ptr<LuaRaycastResult>(result);
        }
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