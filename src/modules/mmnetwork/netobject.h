#pragma once
#include <modules\node\node.h>
#include "network.h"

namespace MM2
{
    // Forward declarations
    struct NET_POSDATA;
    struct mmAccelCompute;
    class asNetObject;
    class mmNetObject;

    // External declarations
    extern class vehCar;
    extern class phInertialCS;

    // Class definitions
    struct NET_POSDATA
    {
        float dword_00;
        char steering;
        char throttle;
        char brake;
        char gear;
        Vector3 angle;
        Vector3 position;
        float velocity_len_2;
        char xvel2;
        char yvel2;
        char zvel2;
        float velocity_len;
        char xvel;
        char yvel;
        char zvel;
        float angvel_scale;
        char xangvel;
        char yangvel;
        char zangvel;
        float damage;
        __int16 score;
        short flags;
        int generation;
    };

    struct NET_POSDATA2 : NET_POSDATA
    {
        int objectId;
    };

    struct mmAccelCompute
    {
        Vector3 lastVelocity;
        Vector3 vecs[10];
        Vector3 unk;

        void SetLatest(Vector3 velocity, float time)      { hook::Thunk<0x43BC60>::Call<void>(this, velocity, time); }
        void Init(Vector3 initialValue, Vector3 velocity) { hook::Thunk<0x043BC10>::Call<void>(this, initialValue, velocity); }
    };



    class asNetObject : public asNode
    {
    protected:
        int ID;
        phInertialCS* ICS;
    public:
        int GetID() const
        {
            return this->ID;
        }

        void SetID(int id)
        {
            this->ID = id;
        }

        static void BindLua(LuaState L)
        {
            LuaBinding(L).beginExtendClass<asNetObject, asNode>("asNetObject")
                .addProperty("ID", &GetID, &SetID)
                .endClass();
        }
    };
    ASSERT_SIZEOF(asNetObject, 0x20);

    class mmNetObject : public asNetObject
    {
    private:
        char _buffer[0x65C];
    protected:
        static hook::Field<0x30, short> _score;
        static hook::Field<0x2C, short> _someFlagMask;
        static hook::Field<0x34, float> _setToDw00; // idk what this is, possibly net time?
        static hook::Field<0xC4, NET_POSDATA> _posData;
        static hook::Field<0x668, int> _debugMode;
        static hook::Field<0x110, int> _generation;
        static hook::Field<0x114, vehCar*> _car;
        static hook::Field<0x118, BOOL> _set; // seems to indicate if the player is "set to go" (e.g. fully loaded)
        static hook::Field<0x11C, BOOL> _active;
        static hook::Field<0x1E4, float> _lastAccelComputeTime;
        static hook::Field<0x218, mmAccelCompute> _accelcompute;
    private:
        void initLua(vehCar* car, const char* basename, int variant, int id, const char* name, bool unk)
        {
            this->Init(car, basename, NULL, variant, id, name, unk);
        }

        void reinitLua(vehCar* car, const char* basename, int variant, int id, const char* name, bool unk)
        {
            this->ReInit(car, basename, NULL, variant, id, name, unk);
        }

        void setActiveLua(bool active)
        {
            this->SetActive(active ? TRUE : FALSE);
        }

        bool getIsSetLua() const
        {
            return IsSet() == TRUE;
        }

        void setLua(bool value)
        {
            Set(value ? TRUE : FALSE);
        }
    public:
        void Init(vehCar* car, const char* basename, float physicsRealism, int variant, int id, const char* name, bool unk)
        {
            hook::Thunk<0x43C4E0>::Call<void>(this, car, basename, physicsRealism, variant, id, name, unk);
        }

        void ReInit(vehCar* car, const char* basename, float physicsRealism, int variant, int id, const char* name, bool unk)
        {
            hook::Thunk<0x43C5D0>::Call<void>(this, car, basename, physicsRealism, variant, id, name, unk);
        }

        void PositionUpdate(NET_POSDATA* data, float time)  { hook::Thunk<0x43CD10 >::Call<void>(this, data, time); }
        int SetPositionData(DPID sendTo, bool reset)        { return SetPositionDataExtended(sendTo, reset); }
        int SetInputData()                                  { return hook::Thunk<0x43C790>::Call<int>(this); }
        void SetCar(vehCar* car)                            { hook::Thunk<0x43C740>::Call<void>(this, car); }
        void SetActive(BOOL active)                         { hook::Thunk<0x43C760>::Call<void>(this, active); }
        void Activate()                                     { hook::Thunk<0x43C480>::Call<void>(this); }
        void Deactivate()                                   { hook::Thunk<0x43C4B0>::Call<void>(this); }
        void Set(BOOL value)                                { _set.set(this, value); }
        BOOL IsSet() const                                  { return _set.get(this); }

        BOOL GetActive() const                              { return _active.get(this); }
        int GetDebugMode() const                            { return _debugMode.get(this); }
        void SetDebugMode(int mode)                         { _debugMode.set(this, mode); }
        vehCar* GetCar() const                              { return _car.get(this); }

        int SetPositionDataExtended(DPID sendTo, bool a3);

        static void BindLua(LuaState L)
        {
            LuaBinding(L).beginExtendClass<mmNetObject, asNetObject>("mmNetObject")
                .addProperty("DebugMode", &GetDebugMode, &SetDebugMode)
                .addProperty("Car", &GetCar, &SetCar)
                .addProperty("Set", &getIsSetLua, &setLua)
                .addFunction("Init", &initLua)
                .addFunction("ReInit", &reinitLua)
                .addFunction("SetPositionData", &SetPositionData)
                .addFunction("SetInputData", &SetInputData)
                .addFunction("SetCar", &SetCar)
                .addFunction("SetActive", &setActiveLua)
                .addFunction("Activate", &Activate)
                .addFunction("Deactivate", &Deactivate)
                .endClass();
        }
    };
    ASSERT_SIZEOF(mmNetObject, 0x67C);

}