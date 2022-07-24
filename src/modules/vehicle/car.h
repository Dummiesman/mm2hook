#pragma once
#include <modules\vehicle.h>

namespace MM2
{
    // Forward declarations
    class vehCar;

    // External declarations
    extern class dgPhysEntity;
    extern class lvlInstance;
    extern class phInertialCS;

    extern class vehCarAudioContainer;
    extern class vehCarDamage;
    extern class vehCarModel;
    extern class vehCarSim;
    extern class vehSplash;
    extern class vehTrailer;
    extern class vehSiren;
    extern class vehGyro;
    extern class vehStuck;
    extern class vehWheelPtx;

    // Class definitions

    class vehCar : public dgPhysEntity {
        byte _buffer[0x1A4];
    protected:
        static hook::Field<0xC0, vehCarDamage *> _damage;
        static hook::Field<0xB8, vehCarSim *> _sim; // size: ~0x1560
        static hook::Field<0xBC, vehCarModel *> _model;
        static hook::Field<0xE0, vehSplash *> _splash;
        static hook::Field<0x254, vehCarAudioContainer *> _audio;
        static hook::Field<0xD8, vehTrailer *> _trailer;
        static hook::Field<0xC8, vehSiren *> _siren;
        static hook::Field<0xC4, vehWheelPtx *> _wheelPtx;
        static hook::Field<0xD0, vehGyro *> _gyro;
        static hook::Field<0xCC, vehStuck*> _stuck;
    private:
        /*
            Valid Modes:
            1 - Forced neutral transmission. Brake input is forced to 1.0.
            2 - Brake input is forced to 1.0. Other inputs are cleared.
            3 - Like mode 2, additionally neutral transmission set on function call, but not enforced. 
        */
        void setDrivable(bool drivable, int mode)
        {
            this->SetDrivable(drivable ? TRUE : FALSE, mode);
        }
    public:
        AGE_API vehCar(BOOL a1)                             { hook::Thunk<0x42BAB0>::Call<void>(this, a1); }
        AGE_API ~vehCar()                                   { hook::Thunk<0x42BCC0>::Call<void>(this); }

        static hook::Type<bool> sm_DrawHeadlights;

        vehCarDamage * GetCarDamage(void) const {
            return _damage.get(this);
        };

        vehCarSim * GetCarSim() const {
            return _sim.get(this);
        }

        vehCarModel * GetModel() const {
            return _model.get(this);
        }

        vehSplash * GetSplash() const {
            return _splash.get(this);
        }

        vehCarAudioContainer * GetCarAudioContainerPtr() const {
            return _audio.get(this);
        }

        vehTrailer * GetTrailer() const {
            return _trailer.get(this);
        }

        vehSiren * GetSiren() const {
            return _siren.get(this);
        }

        vehWheelPtx * GetWheelPtx() const {
            return _wheelPtx.get(this);
        }

        vehGyro * GetGyro() const {
            return _gyro.get(this);
        }

        vehStuck * GetStuck() const {
            return _stuck.get(this);
        }

        AGE_API void Reset()                                { hook::Thunk<0x42C330>::Call<void>(this); }

        AGE_API void ClearDamage()                          { hook::Thunk<0x42C450>::Call<void>(this); }
        AGE_API bool IsPlayer()                             { return hook::Thunk<0x42C890>::Call<bool>(this); }
        AGE_API void Init(char const *a1, int a2, int a3, bool a4, bool a5)
                                                            { hook::Thunk<0x42BE10>::Call<void>(this, a1, a2, a3, a4, a5); }
        AGE_API void InitAudio(char const *a1, int a2)      { hook::Thunk<0x42C1F0>::Call<void>(this, a1, a2); }
        AGE_API void SetDrivable(BOOL drivable, int mode)   { hook::Thunk<0x42C2C0>::Call<void>(this, drivable, mode); }

        /*
            dgPhysEntity virtuals
        */

        AGE_API bool RequiresTerrainCollision() override
                                                            { return hook::Thunk<0x42CA90>::Call<bool>(this); }
        AGE_API lvlInstance* GetInst() override             { return hook::Thunk<0x42CA80>::Call<lvlInstance *>(this); }
        AGE_API phInertialCS* GetICS()                      { return hook::Thunk<0x42CA70>::Call<phInertialCS *>(this); }
        AGE_API void PostUpdate() override                  { hook::Thunk<0x42C8B0>::Call<void>(this); }
        AGE_API void Update() override                      { hook::Thunk<0x42C690>::Call<void>(this); }
        AGE_API void PreUpdate() override                   { hook::Thunk<0x42C480>::Call<void>(this); }

        static void BindLua(LuaState L) {
            LuaBinding(L).beginExtendClass<vehCar, dgPhysEntity>("vehCar")
                .addConstructor(LUA_ARGS(bool))
                //properties
                .addPropertyReadOnly("CarDamage", &GetCarDamage)
                .addPropertyReadOnly("CarSim", &GetCarSim)
                .addPropertyReadOnly("Splash", &GetSplash)
                .addPropertyReadOnly("Audio", &GetCarAudioContainerPtr)
                .addPropertyReadOnly("Trailer", &GetTrailer)
                .addPropertyReadOnly("Siren", &GetSiren)
                .addPropertyReadOnly("WheelPtx", &GetWheelPtx)
                .addPropertyReadOnly("Gyro", &GetGyro)
                .addPropertyReadOnly("Stuck", &GetStuck)

                .addPropertyReadOnly("Instance", &GetModel)

                //functions
                .addFunction("Init", &Init)
                .addFunction("InitAudio", &InitAudio)
                .addFunction("Reset", &Reset)
                .addFunction("ClearDamage", &ClearDamage)
                .addFunction("SetDrivable", &setDrivable, LUA_ARGS(bool, _def<int, 3>))
                .addFunction("IsPlayer", &IsPlayer)
            .endClass();
        }
    };

    ASSERT_SIZEOF(vehCar, 0x258);
}