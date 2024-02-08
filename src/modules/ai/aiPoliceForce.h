#pragma once
#include <mm2_common.h>

namespace MM2
{
    // Forward declarations
    class aiPoliceForce;

    // External declarations
    extern class vehCar;

    // Class definitions
    class aiPoliceForce {
    private:
        static const int NUM_COPS = 16; // Default 3
        static const int NUM_TARGETS = 16; // Default 3
    private:
        Timer timer; // Tracks the last time a cop started giving chase
        short NumPerps;
        short NumChasers[NUM_TARGETS];
        vehCar* PerpCars[NUM_TARGETS];
        vehCar* CopCars[NUM_TARGETS][NUM_COPS];
    public:
        aiPoliceForce(void)                                       DONOTCALL;
        aiPoliceForce(const aiPoliceForce &&)                     DONOTCALL;

        int GetNumChasers(vehCar* perp) const;
        vehCar* GetPerp(int perpIndex) const;
        vehCar* GetChaser(vehCar* perp, int chaserIndex) const;
        int GetNumPerps() const;
        BOOL UnRegisterCop(vehCar* cop, vehCar* perp);
        BOOL RegisterPerp(vehCar* cop, vehCar* perp);
        int Find(vehCar* cop, vehCar* perp);
        int State(vehCar* cop, vehCar* perp, float distance);
        void Reset();
        
        static void BindLua(LuaState L);
    };

    // Lua initialization

}