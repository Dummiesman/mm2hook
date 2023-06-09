#pragma once

namespace MM2
{
    // Forward declarations
    class aiPoliceForce;

    // External declarations
    extern class vehCar;

    // Class definitions

    class aiPoliceForce {
    private:
        static const int NUM_COPS = 3;
        static const int NUM_TARGETS = 3;
    private:
        Timer timer;
        short NumPerps;
        short NumChasers[NUM_TARGETS];
        vehCar* PerpCars[NUM_TARGETS];
        vehCar* CopCars[NUM_TARGETS][NUM_COPS];
    public:
        aiPoliceForce(void)                                       DONOTCALL;
        aiPoliceForce(const aiPoliceForce &&)                     DONOTCALL;

        AGE_API int UnRegisterCop(vehCar *cop, vehCar *perp)      { return hook::Thunk<0x551040>::Call<int>(this, cop, perp); }
        AGE_API int RegisterPerp(vehCar *cop, vehCar *perp)       { return hook::Thunk<0x550FA0>::Call<int>(this, cop, perp); }
        AGE_API int Find(vehCar *cop, vehCar *perp) const         { return hook::Thunk<0x550F40>::Call<int>(this, cop, perp); }
        AGE_API int State(vehCar *cop, vehCar *perp, float a3 ) const
                                                                  { return hook::Thunk<0x551120>::Call<int>(this, cop, perp, a3); }
        AGE_API void Reset()                                      { hook::Thunk<0x550EF0>::Call<void>(this); }

        int GetNumChasers(vehCar* perp) const
        {
            for (int i = 0; i < NUM_TARGETS; i++)
            {
                if (PerpCars[i] == perp)
                {
                    return this->NumChasers[i];
                }
            }
            return 0;
        }

        vehCar* GetChaser(vehCar* perp, int chaserIndex) const
        {
            if (chaserIndex < 0 || chaserIndex >= NUM_COPS)
                return nullptr;
            
            for (int i = 0; i < NUM_TARGETS; i++)
            {
                if (PerpCars[i] == perp && chaserIndex < NumChasers[i])
                {
                    return this->CopCars[i][chaserIndex];
                }
            }
            return nullptr;
        }

        int GetNumPerps() const
        {
            return this->NumPerps;
        }
        static void BindLua(LuaState L) {
            LuaBinding(L).beginClass<aiPoliceForce>("aiPoliceForce")
                .addPropertyReadOnly("NumPerps", &GetNumPerps)
                .addFunction("GetNumChasers", &GetNumChasers)
                .addFunction("GetChaser", &GetChaser)
                .addFunction("UnRegisterCop", &UnRegisterCop)
                .addFunction("RegisterPerp", &RegisterPerp)
                .addFunction("Find", &Find)
                .addFunction("State", &State)
                .addFunction("Reset", &Reset)
                .endClass();
        }
    };

    // Lua initialization

}