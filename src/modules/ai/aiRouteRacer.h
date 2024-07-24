#pragma once
#include "aiVehiclePhysics.h"

namespace MM2
{
    // Forward declarations
    class aiRouteRacer;

    // External declarations
    

    // Class definitions

    class aiRouteRacer {
    private:
        byte _buffer[0x9794];
    protected:
        static hook::Field<0x00, aiVehiclePhysics> _vehiclePhysics;
        static hook::Field<0x9780, unsigned short> _id;
    public:
        aiRouteRacer(void)                                  DONOTCALL;
        aiRouteRacer(const aiRouteRacer &&)                 DONOTCALL;

        int GetId() const                            { return _id.get(this); }
        aiVehiclePhysics* GetVehiclePhysics() const  { return _vehiclePhysics.ptr(this); }
        vehCar* GetCar() const                       { return _vehiclePhysics.ptr(this)->GetCar(); }
        short GetState() const                       { return _vehiclePhysics.ptr(this)->GetState(); }
        int GetCurrentLap() const                    { return _vehiclePhysics.ptr(this)->GetCurrentLap(); }
        int GetLapCount() const                      { return _vehiclePhysics.ptr(this)->GetLapCount(); }
        bool CanRepairDamage() const                 { return _vehiclePhysics.ptr(this)->CanRepairDamage(); }
        void SetCanRepairDamage(bool value)          { _vehiclePhysics.ptr(this)->SetCanRepairDamage(true); }

        AGE_API void Init(int id, const char* raceDir)          { hook::Thunk<0x53D060>::Call<void>(this, id, raceDir);}
        AGE_API int Finished()                                  { return hook::Thunk<0x53D6E0>::Call<int>(this); }

        void DrawRouteThroughTraffic() 
        {
            this->GetVehiclePhysics()->DrawRouteThroughTraffic();
        }

        static void BindLua(LuaState L) {
            LuaBinding(L).beginClass<aiRouteRacer>("aiRouteRacer")
                .addPropertyReadOnly("ID", &GetId)
                .addPropertyReadOnly("Car", &GetCar)
                .addPropertyReadOnly("State", &GetState)
                .addPropertyReadOnly("CurrentLap", &GetCurrentLap)
                .addPropertyReadOnly("NumLaps", &GetLapCount)

                .addFunction("Init", &Init)
                .addFunction("DrawRouteThroughTraffic", &DrawRouteThroughTraffic)
                .addFunction("Finished", &Finished)
            .endClass();
        }
    };

    ASSERT_SIZEOF(aiRouteRacer, 0x9794);
}