#pragma once
#include <modules\node.h>
#include "hud.h"
#include "hudmap.h"

namespace MM2
{
    // Forward declarations
    class mmPlayer;

    // External declarations


    // Class definitions
    class mmPlayer : public asNode {
    private:
        byte _buffer[0x238C];

        AGE_API camCarCS * _GetCurrentCameraPtr() const       { return hook::Thunk<0x4048E0>::Call<camCarCS*>(this); }

    protected:
        static hook::Field<0x2C, vehCar> _car;
        static hook::Field<0x288, mmHUD> _hud;

        static hook::Field<0x2A4, mmDashView> _dashView;
        
        static hook::Field<0xE28, mmHudMap *> _hudmap;
        static hook::Field<0xE2C, camViewCS *> _camView;

        static hook::Field<0xE60, camTrackCS> _nearCam;
        static hook::Field<0x10F8, camTrackCS> _farCam;
        static hook::Field<0x1390, camTrackCS> _indCam;
        static hook::Field<0x1628, camPovCS> _povCam;
        static hook::Field<0x1770, camPovCS> _dashCam;
        static hook::Field<0x18B8, camPolarCS> _thrillCam;
        static hook::Field<0x19E0, camPolarCS> _xCam;
        static hook::Field<0x1B08, camAICS> _freeCam;
        static hook::Field<0x1C2C, camPointCS> _pointCam;
        static hook::Field<0x1D70, camCarCS> _preCam;
        static hook::Field<0x1E98, camCarCS> _postCam;
        static hook::Field<0x1FBC, camPolarCS> _mpPostCam;

        static hook::Field<0x2254, int> _currentWaypoint;
    public:
        vehCar * GetCar() const                  { return _car.ptr(this); }
        mmHUD * GetHUD() const                   { return _hud.ptr(this); }

        mmDashView * GetDashView() const         { return _dashView.ptr(this); }

        camViewCS * GetCamView() const           { return _camView.get(this); }

        camTrackCS * GetNearCam() const          { return _nearCam.ptr(this); }
        camTrackCS * GetFarCam() const           { return _farCam.ptr(this); }
        camTrackCS * GetIndCam() const           { return _indCam.ptr(this); }
        camPovCS * GetPovCam() const             { return _povCam.ptr(this); }
        camPovCS * GetDashCam() const            { return _dashCam.ptr(this); }
        camAICS * GetFreecam() const             { return _freeCam.ptr(this); }
        camPointCS * GetPointCam() const         { return _pointCam.ptr(this); }
        camCarCS * GetPreCam() const             { return _preCam.ptr(this); }
        camCarCS * GetPostCam() const            { return _postCam.ptr(this); }
        camPolarCS* GetThrillCam() const         { return _thrillCam.ptr(this); }
        camPolarCS* GetXCam() const              { return _xCam.ptr(this); }
        camPolarCS* GetMPPostCam() const         { return _mpPostCam.ptr(this); }

        camCarCS * GetCurrentCameraPtr() const   { return this->_GetCurrentCameraPtr(); }

        AGE_API void FFImpactCallback(float damage)         { hook::Thunk<0x406240>::Call<void>(this, damage); }
        AGE_API void EnableRegen(bool a1)                   { hook::Thunk<0x406160>::Call<void>(this, a1); }
        AGE_API float FilterSteering(float a1)              { return hook::Thunk<0x404C90>::Call<float>(this, a1); }
        AGE_API bool IsMaxDamaged() const                   { return hook::Thunk<0x406140>::Call<bool>(this); }
        AGE_API bool IsPOV() const                          { return hook::Thunk<0x404550>::Call<bool>(this); }
        AGE_API void ResetDamage()                          { hook::Thunk<0x406180>::Call<void>(this); }
        AGE_API void SetCamera(int a1, int a2)              { hook::Thunk<0x404710>::Call<void>(this, a1, a2); }
        AGE_API void SetMPPostCam(Matrix34 *a1, float a2)   { hook::Thunk<0x404460>::Call<void>(this, a1, a2); }
        AGE_API void SetPostRaceCam()                       { hook::Thunk<0x404350>::Call<void>(this); }
        AGE_API void SetPreRaceCam()                        { hook::Thunk<0x404250>::Call<void>(this); }
        AGE_API void SetSteering(float a1)                  { hook::Thunk<0x404C50>::Call<void>(this, a1); }
        AGE_API void SetWideFOV(bool a1)                    { hook::Thunk<0x404580>::Call<void>(this, a1); }
        AGE_API void ReInit(const char *basename)           { hook::Thunk<0x4039B0>::Call<void>(this, basename); }

        int GetCurrentWaypoint() const                      { return _currentWaypoint.get(this); }

        /*
            asNode virtuals
        */

        virtual AGE_API void BeforeSave()                   { hook::Thunk<0x403990>::Call<void>(this); }
        virtual AGE_API void AfterLoad()                    { hook::Thunk<0x4039A0>::Call<void>(this); }
        virtual AGE_API void FileIO(datParser &parser)      { hook::Thunk<0x406320>::Call<void>(this, &parser); }
        virtual AGE_API void Reset()                        { hook::Thunk<0x404A60>::Call<void>(this); }
        virtual AGE_API void Update()                       { hook::Thunk<0x405760>::Call<void>(this); }

        static void BindLua(LuaState L) {
            LuaBinding(L).beginExtendClass<mmPlayer, asNode>("mmPlayer")
                //properties
                .addPropertyReadOnly("Car", &GetCar)
                .addPropertyReadOnly("HUD", &GetHUD)
                .addPropertyReadOnly("CamView", &GetCamView)
                .addPropertyReadOnly("FreeCam", &GetFreecam)
                .addPropertyReadOnly("FarCam", &GetFarCam)
                .addPropertyReadOnly("NearCam", &GetNearCam)
                .addPropertyReadOnly("IndCam", &GetIndCam)
                .addPropertyReadOnly("DashCam", &GetDashCam)
                .addPropertyReadOnly("PovCam", &GetPovCam)
                .addPropertyReadOnly("PointCam", &GetPointCam)
                .addPropertyReadOnly("PreCam", &GetPreCam)
                .addPropertyReadOnly("PostCam", &GetPostCam)
                .addPropertyReadOnly("ThrillCam", &GetThrillCam)
                .addPropertyReadOnly("XCam", &GetXCam)
                .addPropertyReadOnly("MPPostCam", &GetMPPostCam)
                .addPropertyReadOnly("CurrentWaypoint", &GetCurrentWaypoint)

                //functions
                .addFunction("FFImpactCallback", &FFImpactCallback)
                .addFunction("EnableRegen", &EnableRegen)
                .addFunction("FilterSteering", &FilterSteering)
                .addFunction("IsMaxDamaged", &IsMaxDamaged)
                .addFunction("ReInit", &ReInit)
                .addFunction("ResetDamage", &ResetDamage)
                .addFunction("SetCamera", &SetCamera)
                .addFunction("SetMPPostCam", &SetMPPostCam)
                .addFunction("SetPostRaceCam", &SetPostRaceCam)
                .addFunction("SetPreRaceCam", &SetPreRaceCam)
                .addFunction("SetSteering", &SetSteering)
                .addFunction("SetWideFOV", &SetWideFOV)
            .endClass();
        }
    };

    ASSERT_SIZEOF(mmPlayer, 0x23A4);
}