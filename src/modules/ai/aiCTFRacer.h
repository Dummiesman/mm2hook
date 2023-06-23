#pragma once
#include "aiVehiclePhysics.h"

namespace MM2
{
    // Forward declarations
    class aiCTFRacer;

    // External declarations
    extern class Base;

    // Class definitions

    class aiCTFRacer : public Base 
    {
    private:
        byte buffer[0x9874];
    protected:
        static hook::Field<0x04, Vector3> _flagPosition;
        static hook::Field<0x1C, Vector3> _deliverPosition;
        static hook::Field<0x028, aiVehiclePhysics> _physics;
        static hook::Field<0x979A, unsigned short> _id;
        static hook::Field<0x979C, unsigned short> _ctfState;
    public:
        aiCTFRacer(void)                                    DONOTCALL;
        aiCTFRacer(const aiCTFRacer &&)                     DONOTCALL;

        AGE_API void Init(int id, const char* basename)     { hook::Thunk<0x554470>::Call<void>(this, id, basename); }

        aiVehiclePhysics* GetVehiclePhysics() const
        {
            return _physics.ptr(this);
        }

        vehCar* GetCar() const
        {
            return GetVehiclePhysics()->GetCar();
        }

        int GetId() const
        {
            return _id.get(this);
        }

        short GetState() const
        {
            return GetVehiclePhysics()->GetState();
        }

        Vector3 GetFlagPosition() const { return _flagPosition.get(this); }
        void SetFlagPosition(Vector3 pos) { _flagPosition.set(this, pos); }

        Vector3 GetDeliverPosition() const { return _deliverPosition.get(this); }
        void SetDeliverPosition(Vector3 pos) { _deliverPosition.set(this, pos); }

        void EnterAcquireMode() { _ctfState.set(this, 0); }
        void EnterDeliverMode() { _ctfState.set(this, 2); }

        AGE_API void Reset() { hook::Thunk<0x554510>::Call<void>(this); }

        static void BindLua(LuaState L) {
            LuaBinding(L).beginExtendClass<aiCTFRacer, Base>("aiCTFRacer")
                .addPropertyReadOnly("Car", &GetCar)
                .addPropertyReadOnly("State", &GetState)
                .addPropertyReadOnly("ID", &GetId)
                .addProperty("FlagPosition", &GetFlagPosition, &SetFlagPosition)
                .addProperty("DeliverPosition", &GetDeliverPosition, &SetDeliverPosition)
                .addFunction("Acquire", &EnterAcquireMode)
                .addFunction("Deliver", &EnterDeliverMode)
                .addFunction("Reset", &Reset)
                .endClass();
        }

    };

    ASSERT_SIZEOF(aiCTFRacer, 0x9878);

    // Lua initialization

}