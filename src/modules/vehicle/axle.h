#pragma once
#include <modules\vehicle.h>

namespace MM2
{
    // Forward declarations
    class vehAxle;

    // External declarations
    extern class vehCarSim;
    extern class vehWheel;

    // Class definitions
    class vehAxle : public asNode {
    private:
        vehCarSim *m_CarSimPtr;
        Matrix34 AxlePivot;
        Matrix34 AxleMatrix;
        vehWheel *LeftWheel;
        vehWheel *RightWheel;
        float unknown132; //something to do with angle from wheel to pivot
        float unknown136; //something to do with angle from wheel to pivot
        float ScaledTorqueCoef;
        float ScaledDampCoef;
        float TorqueCoef;
        float DampCoef;
    public:
        AGE_API vehAxle()                                   { hook::Thunk<0x4D9990>::Call<void>(this); }

        AGE_API void Init(vehCarSim *carSim, const char *vehicleBasename, const char *axleName, vehWheel *wheelL, vehWheel *wheelR)                     
                                                            { hook::Thunk<0x4D9A50>::Call<void>(this, carSim, vehicleBasename, axleName, wheelL, wheelR); }
        AGE_API void ComputeConstants()                     { hook::Thunk<0x4D9A20>::Call<void>(this); }

        /*
            asNode virtuals
        */

        AGE_API void Update() override                      { hook::Thunk<0x4D9B20>::Call<void>(this); }
        AGE_API void FileIO(datParser& parser) override     { hook::Thunk<0x4D9CA0>::Call<void>(this); }
        AGE_API char * GetClassName() override              { return hook::Thunk<0x4D9D20>::Call<char*>(this); }

        /*
            vehAxle
        */
        float GetTorqueCoef() const                         { return this->TorqueCoef; }
        void SetTorqueCoef(float torqueCoef)                { this->TorqueCoef = torqueCoef;}

        float GetDampCoef() const                           { return this->DampCoef; }
        void SetDampCoef(float dampCoef)                    { this->DampCoef = dampCoef;}

        Matrix34 GetAxleMatrix() const {
            return this->AxleMatrix;
        }

        vehWheel* GetLeftWheel() const {
            return this->LeftWheel;
        }

        vehWheel* GetRightWheel() const {
            return this->RightWheel;
        }

        static void BindLua(LuaState L) {
            LuaBinding(L).beginExtendClass<vehAxle, asNode>("vehAxle")
                //properties
                .addProperty("DampCoef", &GetDampCoef, &SetDampCoef)
                .addProperty("TorqueCoef", &GetTorqueCoef, &SetTorqueCoef)

                .addFunction("Init", &Init)
                .addFunction("ComputeConstants", &ComputeConstants)

                .endClass();
        }
    };

    ASSERT_SIZEOF(vehAxle, 0x9C);
}