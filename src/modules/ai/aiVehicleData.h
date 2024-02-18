#pragma once
#include <modules\phys\bound.h>
#include <modules\node\node.h>

namespace MM2
{
    // Forward declarations
    class aiVehicleData;

    // External declarations
    extern class dgBoundBox;

    // Class definitions
    class aiVehicleData : public asNode {
    private:
        Vector3 Size;
        Vector3 MaxAng;
        Vector3 CG;
        Vector3 WheelPositions[6];
        float Mass;
        float Friction;
        float Elasticity;
        float MaxDamage;
        float PtxThresh;
        float Spring;
        float Damping;
        float RubberSpring;
        float RubberDamp;
        float Limit;
        float WheelRadius;
        int unknown176;
        int DataId;
        dgBoundBox* BoundingBox;

    public:
        //overrides
        AGE_API char* GetClassName() override           { return hook::Thunk<0x56F940>::Call<char*>(this); }
        AGE_API const char* GetDirName() override       { return hook::Thunk<0x56F950>::Call<const char*>(this); }
        AGE_API void FileIO(datParser &parser) override { hook::Thunk<0x56F7C0>::Call<void>(this, &parser); }

        //lua helpers
        Vector3 GetWheelPosition(int id) {
            if (id > 5) id = 5;
            return WheelPositions[id];
        }

        void SetWheelPosition(int id, Vector3 const& position) {
            if (id > 5) id = 5;
            WheelPositions[id] = position;
        }

        int getDataId() {
            return DataId;
        }

        //lua
        static void BindLua(LuaState L) {
            LuaBinding(L).beginExtendClass<aiVehicleData, asNode>("aiVehicleData")
                .addPropertyReadOnly("DataId", &getDataId)
                .addVariable("Size", &aiVehicleData::Size)
                .addVariable("CG", &aiVehicleData::CG)
                .addVariable("MaxAng", &aiVehicleData::MaxAng)
                .addVariable("Mass", &aiVehicleData::Mass)
                .addVariable("Friction", &aiVehicleData::Friction)
                .addVariable("MaxDamage", &aiVehicleData::MaxDamage)
                .addVariable("PtxThresh", &aiVehicleData::PtxThresh)
                .addVariable("Spring", &aiVehicleData::Spring)
                .addVariable("Damping", &aiVehicleData::Damping)
                .addVariable("RubberSpring", &aiVehicleData::RubberSpring)
                .addVariable("RubberDamp", &aiVehicleData::RubberDamp)
                .addVariable("Limit", &aiVehicleData::Limit)

                .addVariable("WheelRadius", &aiVehicleData::WheelRadius)

                .addFunction("SetWheelPosition", &SetWheelPosition)
                .addFunction("GetWheelPosition", &GetWheelPosition)

                .endClass();
        }
    };

    ASSERT_SIZEOF(aiVehicleData, 0xBC);
}