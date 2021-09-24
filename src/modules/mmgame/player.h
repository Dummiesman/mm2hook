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
        byte _buffer[0x23A4];

        AGE_API camCarCS * GetCurrentCameraPtr()             { return hook::Thunk<0x4048E0>::Call<camCarCS*>(this); }

    protected:
        hook::Field<0x2C, vehCar> _car;
        hook::Field<0x288, mmHUD> _hud;

        hook::Field<0x2A4, mmDashView> _dashView;
        hook::Field<0xD0C, mmTimer> _timer;
        
        hook::Field<0xE28, mmHudMap *> _hudmap;
        hook::Field<0xE2C, camViewCS *> _camView;

        hook::Field<0xE60, camTrackCS> _nearCam;
        hook::Field<0x10F8, camTrackCS> _farCam;
        hook::Field<0x1390, camTrackCS> _indCam;
        hook::Field<0x1628, camPovCS> _povCam;
        hook::Field<0x1770, camPovCS> _dashCam;
        hook::Field<0x18B8, camCarCS> _polarCam1;
        hook::Field<0x19E0, camCarCS> _polarCam2;
        hook::Field<0x1B08, camAICS> _freeCam;
        hook::Field<0x1C2C, camPointCS> _pointCam;
        hook::Field<0x1D70, camCarCS> _preCam;
        hook::Field<0x1E98, camCarCS> _postCam;
        hook::Field<0x1FBC, camCarCS> _polarCam3;

    public:
        inline vehCar * getCar(void) const                  { return _car.ptr(this); }
        inline mmHUD * getHUD(void) const                   { return _hud.ptr(this); }

        inline mmDashView * getDashView(void) const         { return _dashView.ptr(this); }
        inline mmTimer * getTimer(void) const               { return _timer.ptr(this); }

        inline mmHudMap * getHudmap(void) const             { return _hudmap.get(this); }
        inline camViewCS * getCamView(void) const           { return _camView.get(this); }

        inline camTrackCS * getNearCam(void) const          { return _nearCam.ptr(this); }
        inline camTrackCS * getFarCam(void) const           { return _farCam.ptr(this); }
        inline camTrackCS * getIndCam(void) const           { return _indCam.ptr(this); }
        inline camPovCS * getPovCam(void) const             { return _povCam.ptr(this); }
        inline camPovCS * getDashCam(void) const            { return _dashCam.ptr(this); }
        inline camAICS * getFreecam(void) const             { return _freeCam.ptr(this); }
        inline camPointCS * getPointCam(void) const         { return _pointCam.ptr(this); }
        inline camCarCS * getPreCam(void) const             { return _preCam.ptr(this); }
        inline camCarCS * getPostCam(void) const            { return _postCam.ptr(this); }
  
        inline camCarCS * getPolarCamOne(void) const        { return _polarCam1.ptr(this); }
        inline camCarCS * getPolarCamTwo(void) const        { return _polarCam2.ptr(this); }
        inline camCarCS * getPolarCamThree(void) const      { return _polarCam3.ptr(this); }

        inline camCarCS * getCurrentCameraPtr(void)         { return this->GetCurrentCameraPtr(); }

        AGE_API void EnableRegen(bool a1)                   { hook::Thunk<0x406160>::Call<void>(this, a1); }
        AGE_API float FilterSteering(float a1)              { return hook::Thunk<0x404C90>::Call<float>(this, a1); }
        AGE_API bool IsMaxDamaged()                         { return hook::Thunk<0x406140>::Call<bool>(this); }
        AGE_API bool IsPOV()                                { return hook::Thunk<0x404550>::Call<bool>(this); }
        AGE_API void ResetDamage()                          { hook::Thunk<0x406180>::Call<void>(this); }
        AGE_API void SetCamera(int a1, int a2)              { hook::Thunk<0x404710>::Call<void>(this, a1, a2); }
        AGE_API void SetMPPostCam(Matrix34 *a1, float a2)   { hook::Thunk<0x404460>::Call<void>(this, a1, a2); }
        AGE_API void SetPostRaceCam()                       { hook::Thunk<0x404350>::Call<void>(this); }
        AGE_API void SetPreRaceCam()                        { hook::Thunk<0x404250>::Call<void>(this); }
        AGE_API void SetSteering(float a1)                  { hook::Thunk<0x404C50>::Call<void>(this, a1); }
        AGE_API void SetWideFOV(bool a1)                    { hook::Thunk<0x404580>::Call<void>(this, a1); }
        AGE_API void ReInit(const char *basename)           { hook::Thunk<0x4039B0>::Call<void>(this, basename); }

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
                .addPropertyReadOnly("Car", &getCar)
                .addPropertyReadOnly("HUD", &getHUD)
                .addPropertyReadOnly("Hudmap", &getHudmap)
                .addPropertyReadOnly("CamView", &getCamView)
                .addPropertyReadOnly("FreeCam", &getFreecam)
                .addPropertyReadOnly("FarCam", &getFarCam)
                .addPropertyReadOnly("NearCam", &getNearCam)
                .addPropertyReadOnly("IndCam", &getIndCam)
                .addPropertyReadOnly("DashCam", &getDashCam)
                .addPropertyReadOnly("PovCam", &getPovCam)
                .addPropertyReadOnly("PointCam", &getPointCam)

                .addPropertyReadOnly("PreCam", &getPreCam)
                .addPropertyReadOnly("PostCam", &getPostCam)
                .addPropertyReadOnly("PolarCamOne", &getPolarCamOne)
                .addPropertyReadOnly("PolarCamTwo", &getPolarCamTwo)
                .addPropertyReadOnly("PolarCamThree", &getPolarCamThree)

                //functions
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
}