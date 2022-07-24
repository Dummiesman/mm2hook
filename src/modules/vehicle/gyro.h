#pragma once
#include <modules\vehicle.h>

namespace MM2
{
    // Forward declarations
    class vehGyro;

    // External declarations


    // Class definitions
    class vehGyro : public asNode
    {
    private:
        vehCarSim* m_CarSimPtr;
        float m_Drift;
        float m_Spin180;
        float m_Reverse180;
        float m_Pitch;
        float m_Roll;
    public:
        AGE_API vehGyro() { hook::Thunk<0x4D5B80>::Call<void>(this); }

        AGE_API void Init(vehCarSim *carSim, const char *name) 
                                                            { hook::Thunk<0x4D5BD0>::Call<void>(this, carSim, name); }

        /*
            asNode virtuals
        */

        AGE_API void Update() override                      { hook::Thunk<0x4D5C00>::Call<void>(this); }
        AGE_API void FileIO(datParser& parser) override     { hook::Thunk<0x4D5EE0>::Call<void>(this); }
        AGE_API const char* GetDirName() override           { return hook::Thunk<0x4D5BC0 >::Call<const char*>(this); }
        AGE_API char* GetClassName() override               { return hook::Thunk<0x4D5FA0 >::Call<char*>(this); }

        /*
            vehGyro
        */
        bool GetDriftable() const {
            return (this->flags & 0x20000) != 0;
        }

        inline bool GetSpinable() const {
            return (this->flags & 0x10000) != 0;
        }

        inline bool GetRightable() const {
            return (this->flags & 0x40000) != 0;
        }

        void SetDriftable(bool enabled) {
            if (enabled)
                this->flags |= 0x20000;
            else
                this->flags &= ~(0x20000);
        }

        void SetSpinable(bool enabled) {
            if (enabled)
                this->flags |= 0x10000;
            else
                this->flags &= ~(0x10000);
        }

        void SetRightable(bool enabled) {
            if (enabled)
                this->flags |= 0x40000;
            else
                this->flags &= ~(0x40000);
        }

        float GetDrift() const                              { return this->m_Drift; }
        void SetDrift(float drift)                          { this->m_Drift = drift; }

        float GetSpin180() const                            { return this->m_Spin180; }
        void SetSpin180(float spin)                         { this->m_Spin180 = spin; }

        float GetReverse180() const                         { return this->m_Reverse180; }
        void SetReverse180(float reverse)                   { this->m_Reverse180 = reverse; }

        float GetPitch() const                              { return this->m_Pitch; }
        void SetPitch(float pitch)                          { this->m_Pitch = pitch; }

        float GetRoll() const                               { return this->m_Roll; }
        void SetRoll(float roll)                            { this->m_Roll = roll; }

        static void BindLua(LuaState L) {
            LuaBinding(L).beginExtendClass<vehGyro, asNode>("vehGyro")
                //properties
                .addProperty("Driftable", &GetDriftable, &SetDriftable)
                .addProperty("Spinable", &GetSpinable, &SetSpinable)
                .addProperty("Rightable", &GetRightable, &SetRightable)

                .addProperty("Drift", &GetDrift, &SetDrift)
                .addProperty("Spin180", &GetSpin180, &SetSpin180)
                .addProperty("Reverse180", &GetReverse180, &SetReverse180)
                .addProperty("Pitch", &GetPitch, &SetPitch)
                .addProperty("Roll", &GetRoll, &SetRoll)

                .addFunction("Init", &Init)

                .endClass();
        }
    };

    ASSERT_SIZEOF(vehGyro, 0x30);
}