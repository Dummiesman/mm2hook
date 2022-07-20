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
    protected:
        hook::Field<0x1E4, vehCarSim *> _sim;
        hook::Field<0x1E8, vehTrailerInstance> _instance;
        hook::Field<0xCC, dgTrailerJoint> _joint;
        hook::Field<0x528, vehWheel> _twhl0;
        hook::Field<0x794, vehWheel> _twhl1;
        hook::Field<0xA00, vehWheel> _twhl2;
        hook::Field<0xC6C, vehWheel> _twhl3;
    public:
        inline vehCarSim* getCarSim() 
        {
            return _sim.get(this);
        }

        inline dgTrailerJoint* getJoint()  
        {
            return _joint.ptr(this);
        }

        inline vehWheel * getWheel(int num) 
        {
            switch (num) {
            case 0:
                return _twhl0.ptr(this);
            case 1:
                return _twhl1.ptr(this);
            case 2:
                return _twhl2.ptr(this);
            case 3:
                return _twhl3.ptr(this);
            }
            return nullptr;
        }

        inline vehTrailerInstance* getInstance()
        {
            return _instance.ptr(this);
        }

        AGE_API void Init(const char* basename, const Vector3* a2, vehCarSim* a3, int a4)
        {
            hook::Thunk<0x4D72F0>::Call<void>(this, basename, a2, a3, a4);

            Matrix34 diffMatrix;

            if (GetPivot(diffMatrix, basename, "trailer_twhl4")) {
                getCarSim()->TrailerBackBackLeftWheelPosDiff = diffMatrix.GetRow(3) - getWheel(2)->GetCenter();
            }

            if (GetPivot(diffMatrix, basename, "trailer_twhl5")) {
                getCarSim()->TrailerBackBackLeftWheelPosDiff = diffMatrix.GetRow(3) - getWheel(3)->GetCenter();
            }
        }

        static void BindLua(LuaState L) {
            LuaBinding(L).beginExtendClass<vehTrailer, dgPhysEntity>("vehTrailer")
                .addFunction("GetWheel", &getWheel)
                .addPropertyReadOnly("CarSim", &getCarSim)
                .addPropertyReadOnly("Joint", &getJoint)
                .addPropertyReadOnly("Instance", &getInstance)
            .endClass();
        }
    };

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
        Vector3 getTrailerHitchOffsetLua()
        {
            Vector3 vec;
            this->GetTrailerHitch(&vec);
            return vec;
        }
    protected:
        hook::Field<0x14, vehTrailer *> _trailer;
    public:
        inline vehTrailer * getTrailer(void) const {
            return _trailer.get(this);
        }

        AGE_API bool GetTrailerHitch(Vector3* out)      { return hook::Thunk<0x4D8420>::Call<bool>(this, out); }

        static void BindLua(LuaState L) {
            LuaBinding(L).beginExtendClass<vehTrailerInstance, lvlInstance>("vehTrailerInstance")
                //properties
                .addPropertyReadOnly("Trailer", &getTrailer)
                .addPropertyReadOnly("TrailerHitchOffset", &getTrailerHitchOffsetLua)
            .endClass();
        }
    };

    // Lua initialization

}