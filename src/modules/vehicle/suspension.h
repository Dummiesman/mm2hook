#pragma once
#include <modules\vehicle.h>
#include <modules\model\getpivot.h>

namespace MM2
{
    // Forward declarations
    class vehSuspension;

    // External declarations
    extern class vehCarSim;
    extern class vehWheel;

    // Class definitions
    class vehSuspension : public asNode {
    private:
        Matrix34 m_SuspensionPivot;
        Matrix34 m_SuspensionMatrix;
        vehCarSim* m_CarSimPtr;
        vehWheel* m_Wheel;
        float m_InvRestLength;
        BOOL m_IsVertical;
    public:
        vehWheel* GetWheel() {
            return this->m_Wheel;
        }

        Matrix34 GetSuspensionMatrix()
        {
            return this->m_SuspensionMatrix;
        }
    public:
        AGE_API vehSuspension()                             { hook::Thunk<0x4D9990>::Call<void>(this); }

        AGE_API void Init(vehCarSim* carSim, const char* vehicleBasename, const char* suspensionName, vehWheel* wheel)
        {
            m_CarSimPtr = carSim;
            m_Wheel = wheel;

            if (GetPivot(m_SuspensionPivot, vehicleBasename, suspensionName))
            {
                Vector3 suspensionOrigin = m_SuspensionPivot.GetRow(3);
                Vector3 wheelDist = (wheel->GetCenter() - suspensionOrigin);
                Vector3 suspensionAxis = m_SuspensionPivot.GetRow(2);
                m_InvRestLength = 1.0f / wheelDist.Dot(suspensionAxis);
                m_IsVertical = (fabsf(suspensionAxis.Y) >= 0.5f) ? 1 : 0;
                this->setActive(true);
            }
            else
            {
                this->setActive(false);
            }
        }


        /*
            asNode virtuals
        */

        AGE_API void Update() override                      { hook::Thunk<0x4D98B0>::Call<void>(this); }
        AGE_API void FileIO(datParser& parser) override     { hook::Thunk<0x4D9920>::Call<void>(this); }
        AGE_API char* GetClassName() override               { return hook::Thunk<0x4D9980>::Call<char*>(this); }

        static void BindLua(LuaState L) {
            LuaBinding(L).beginExtendClass<vehSuspension, asNode>("vehSuspension")
                //properties
                .addFunction("Init", &Init)

                .endClass();
        }
    };

    ASSERT_SIZEOF(vehSuspension, 0x88);

    // Lua initialization

}