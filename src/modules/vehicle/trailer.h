#pragma once
#include <modules\vehicle.h>
#include <modules\phys.h>
#include "carsim.h"

namespace MM2
{
    // Forward declarations
    class vehTrailer;
    class vehTrailerInstance;

    // External declarations
    extern class asNode;
    extern class dgPhysEntity;
    extern class dgTrailerJoint;

    // Class definitions
    class vehTrailer : public dgPhysEntity, public asNode {
    private:
        byte _buffer[0xF6C];
    protected:
        static hook::Field<0x1E4, vehCarSim *> _sim;
        static hook::Field<0x1E8, vehTrailerInstance> _instance;
        static hook::Field<0xCC, dgTrailerJoint> _joint;
        static hook::Field<0x528, vehWheel> _twhl0;
        static hook::Field<0x794, vehWheel> _twhl1;
        static hook::Field<0xA00, vehWheel> _twhl2;
        static hook::Field<0xC6C, vehWheel> _twhl3;
    public:
        vehCarSim* GetCarSim() const;
        dgTrailerJoint* GetJoint() const; 
        vehWheel* GetWheel(int num) const;
        vehTrailerInstance* GetInstance() const;

        AGE_API void Init(const char* basename, const Vector3* a2, vehCarSim* a3, int a4);

        static void BindLua(LuaState L);
    };

    ASSERT_SIZEOF(vehTrailer, 0x1038);

    class vehTrailerInstance : public lvlInstance {
    public:
        static const int TLIGHT_GEOM_ID = 2;
        static const int TWHL0_GEOM_ID = 3;
        static const int TWHL1_GEOM_ID = 4;
        static const int TWHL2_GEOM_ID = 5;
        static const int TWHL3_GEOM_ID = 6;
        static const int TWHL4_GEOM_ID = 15;
        static const int TWHL5_GEOM_ID = 16;
        static const int RLIGHT_GEOM_ID = 8;
        static const int BLIGHT_GEOM_ID = 9;
        static const int HLIGHT_GEOM_ID = 10;
        static const int SLIGHT0_GEOM_ID = 11;
        static const int SLIGHT1_GEOM_ID = 12;
        static const int SIREN0_GEOM_ID = 13;
        static const int SIREN1_GEOM_ID = 14;
        static const int TSWHL0_GEOM_ID = 17;
        static const int TSWHL1_GEOM_ID = 18;
        static const int TSWHL2_GEOM_ID = 19;
        static const int TSWHL3_GEOM_ID = 20;
        static const int TSWHL4_GEOM_ID = 21;
        static const int TSWHL5_GEOM_ID = 22;
        static const int TSLIGHT0_GEOM_ID = 23;
        static const int TSLIGHT1_GEOM_ID = 24;
    private:
        byte _buffer[0x12];
        Vector3 getTrailerHitchOffsetLua() const;
    protected:
        static hook::Field<0x14, vehTrailer *> _trailer;
    public:
        /*
            lvlInstance virtuals
        */
        virtual AGE_API const Vector3& GetPosition() override;
        virtual AGE_API const Matrix34& GetMatrix(Matrix34& a1) override;
        virtual AGE_API void SetMatrix(const Matrix34& a1) override;
        virtual AGE_API void Draw(int) override;
        virtual AGE_API unsigned int SizeOf(void) override;

        /*
            vehTrailerInstance
        */
        vehTrailer* GetTrailer(void) const;
        AGE_API bool GetTrailerHitch(Vector3* out) const;

        static void BindLua(LuaState L);
    };

    ASSERT_SIZEOF(vehTrailerInstance, 0x28);
}