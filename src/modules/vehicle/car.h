#pragma once
#include <modules\phys\physentity.h>
#include <modules\vehicle\cardamage.h>
#include <modules\vehicle\carsim.h>
#include <modules\vehicle\splash.h>
#include <modules\vehicle\caraudiocontainer.h>
#include <modules\vehicle\siren.h>
#include <modules\vehicle\stuck.h>
#include <modules\vehicle\gyro.h>
#include <modules\vehicle\carmodel.h>
#include <modules\vehicle\trailer.h>

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
        void setDrivable(bool drivable, int mode);
    public:
        ANGEL_ALLOCATOR

        AGE_API vehCar(BOOL a1);
        AGE_API ~vehCar();

        static hook::Type<bool> sm_DrawHeadlights;

        vehCarDamage* GetCarDamage(void) const;
        vehCarSim* GetCarSim() const;
        vehCarModel* GetModel() const;
        vehSplash* GetSplash() const;
        vehCarAudioContainer* GetCarAudioContainerPtr() const;
        vehTrailer* GetTrailer() const;
        vehSiren* GetSiren() const;
        vehWheelPtx* GetWheelPtx() const;
        vehGyro* GetGyro() const;
        vehStuck* GetStuck() const;

        AGE_API void Reset();

        AGE_API void ClearDamage();
        AGE_API bool IsPlayer();
        AGE_API void Init(char const* basename, int variant, int colliderID, bool useFullBound, bool hasTrailer);
        AGE_API void InitAudio(char const* basename, int audioType);
        AGE_API void SetDrivable(BOOL drivable, int mode);
        AGE_API void SetColliderID(int id);

        /*
            dgPhysEntity virtuals
        */

        AGE_API bool RequiresTerrainCollision() override;
                                                       
        AGE_API lvlInstance* GetInst() override;
        AGE_API phInertialCS* GetICS();
        AGE_API void PostUpdate() override;
        AGE_API void Update() override;
        AGE_API void PreUpdate() override;

        static void BindLua(LuaState L);
    };

    ASSERT_SIZEOF(vehCar, 0x258);
}