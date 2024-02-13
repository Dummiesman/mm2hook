#pragma once
#include <modules\node.h>
//#include <mm2_game.h>
//#include <modules\vehicle.h>

namespace MM2
{
    // Forward declarations
    class camBaseCS;
    class camAppCS;
    class camCarCS;
    class camPointCS;
    class camPovCS;
    class camTrackCS;
    class camPolarCS;
    class camAICS;
    class camViewCS;
    class asCamera;

    // External declarations
    extern class asNode;
    extern class vehCar;

    // Class definitions
    class asCamera : public asNode {
    private:
        byte _buffer[0x158];
    public:
        AGE_API asCamera(void) {
            scoped_vtable x(this);
            hook::Thunk<0x4A2340>::Call<void>(this);
        };

        virtual AGE_API ~asCamera(void) {
            scoped_vtable x(this);
            hook::Thunk<0x4A22E0>::Call<void>(this);
        };

        AGE_API void Update() override                      { hook::Thunk<0x4A24A0>::Call<void>(this); }
    };
    ASSERT_SIZEOF(asCamera, 0x170);

    class camBaseCS : public asNode {
    protected:
        uint16_t unk_18; // flags?
        uint16_t unk_1A; // unused/padding ?

        Matrix34 matrix_1; // matrix
        Matrix34 matrix_2; // "exact matrix" as tdor calls it

        camViewCS *view;

        float BlendTime;
        float BlendGoal;
        float CameraFOV;
        float CameraNear;
    public:
        AGE_API camBaseCS(void) {
            scoped_vtable x(this);
            hook::Thunk<0x521D60>::Call<void>(this);
        }

        virtual AGE_API ~camBaseCS(void) {
            scoped_vtable x(this);
            hook::Thunk<0x521DF0>::Call<void>(this);
        }

        //fields
        Matrix34 GetMatrix() const
        {
            return this->matrix_1;
        }

        void SetMatrix(Matrix34 matrix)
        {
            this->matrix_1 = matrix;
        }
        
        Vector3 GetPosition() const
        {
            return this->matrix_1.GetRow(3);
        }

        void SetPosition(Vector3 position)
        {
            this->matrix_1.SetRow(3, position);
        }

        float GetFOV() const
        {
            return this->CameraFOV;
        }
        
        void SetFOV(float fov)
        {
            this->CameraFOV = fov;
        }

        void SetCamView(camViewCS* view)
        {
            this->view = view;
        }

        //asNode overrides
        AGE_API void AfterLoad() override                   { hook::Thunk<0x521F30>::Call<void>(this); }
        AGE_API void FileIO(datParser &parser) override     { hook::Thunk<0x521EA0>::Call<void>(this, &parser); }
        AGE_API const char * GetDirName() override          { return hook::Thunk<0x521E00>::Call<const char*>(this); }

        //virtuals
        virtual AGE_API void MakeActive()                   { hook::Thunk<0x521520>::Call<void>(this); }
        virtual AGE_API void UpdateInput()                  { hook::Thunk<0x520410>::Call<void>(this); }
        virtual AGE_API void ForceMatrixDelta(const Vector3 &a1)
                                                            { hook::Thunk<0x521E60>::Call<void>(this, &a1); }
        virtual AGE_API void SetST(float *a1)               { hook::Thunk<0x51D750>::Call<void>(this, a1); }

        //members
        AGE_API BOOL IsViewCSInTransition()                 { return hook::Thunk<0x521E10>::Call<BOOL>(this);}
        AGE_API void UpdateView()                           { hook::Thunk<0x521E30>::Call<void>(this); }

        //lua
        static void BindLua(LuaState L) {
            LuaBinding(L).beginExtendClass<camBaseCS, asNode>("camBaseCS")
                //properties
                .addFactory([]() {  
                    auto obj = new camBaseCS();
                    MM2Lua::MarkForCleanupOnShutdown(obj);
                    return obj;
                 })
                .addProperty("FOV", &GetFOV, &SetFOV)
                .addProperty("CameraFOV", &GetFOV, &SetFOV)

                .addFunction("GetPosition", &GetPosition)
                .addFunction("GetMatrix", &GetMatrix)
                .addFunction("SetMatrix", &SetMatrix)
                .addFunction("SetPosition", &SetPosition)
                .addFunction("SetCamView", &SetCamView)

                .addVariable("BlendTime", &camBaseCS::BlendTime)
                .addVariable("BlendGoal", &camBaseCS::BlendGoal)
                .addVariable("CameraNear", &camBaseCS::CameraNear)

                //members
                .addFunction("UpdateView", &UpdateView)
            .endClass();
        }
    };
    ASSERT_SIZEOF(camBaseCS, 0x90);

    class camAppCS : public camBaseCS {
    protected:
        Matrix34 *Target;

        Vector3 TrackTo;

        BOOL ApproachOn;
        BOOL AppAppOn;

        float AppRot;
        float AppXRot;
        float AppYPos;
        float AppXZPos;
        float AppApp;
        float AppRotMin;
        float AppPosMin;
        float LookAbove;

        void *unk_C8;

        float MaxDist;
        float MinDist;
        float LookAt;

        // approach stuff?
        Matrix34 unk_D8;
    protected:
        AGE_API void ApproachIt()                           { hook::Thunk<0x522060>::Call<void>(this); }

    public:
        AGE_API camAppCS(void) {
            scoped_vtable x(this);
            hook::Thunk<0x521F70>::Call<void>(this);
        }

        virtual AGE_API ~camAppCS(void) {
            scoped_vtable x(this);
            hook::Thunk<0x522050>::Call<void>(this);
        }

        void SetApproachOn(bool value) { ApproachOn = (value) ? TRUE : FALSE; }
        bool GetApproachOn() const     { return ApproachOn == TRUE; }
        void SetAppAppOn(bool value)   { AppAppOn = (value) ? TRUE : FALSE; }
        bool GetAppAppOn() const       { return AppAppOn == TRUE; }
            

        //overrides
        AGE_API void FileIO(datParser &parser) override     { hook::Thunk<0x5229D0>::Call<void>(this, &parser); }
    
        //lua
        static void BindLua(LuaState L) {
            LuaBinding(L).beginExtendClass<camAppCS, camBaseCS>("camAppCS")
                .addFactory([]() {  
                    auto obj = new camAppCS();
                    MM2Lua::MarkForCleanupOnShutdown(obj);
                    return obj;
                 })
                .addVariableRef("Target", &camAppCS::Target)
                .addProperty("ApproachOn", &GetApproachOn, &SetApproachOn)
                .addProperty("AppAppOn", &GetAppAppOn, &SetAppAppOn)
                .addVariable("AppRot", &camAppCS::AppRot)
                .addVariable("AppXRot", &camAppCS::AppXRot)
                .addVariable("AppYPos", &camAppCS::AppYPos)
                .addVariable("AppXZPos", &camAppCS::AppXZPos)
                .addVariable("AppApp", &camAppCS::AppApp)
                .addVariable("AppRotMin", &camAppCS::AppRotMin)
                .addVariable("AppPosMin", &camAppCS::AppPosMin)
                .addVariable("LookAbove", &camAppCS::LookAbove)
                .addVariable("TrackTo", &camAppCS::TrackTo)
                .addVariable("MaxDist", &camAppCS::MaxDist)
                .addVariable("MinDist", &camAppCS::MinDist)
                .addVariable("LookAt", &camAppCS::LookAt)
            .endClass();
        }
    };
    ASSERT_SIZEOF(camAppCS, 0x108);

    class camCarCS : public camAppCS {
    protected:
        vehCar *car;
        int ReverseMode; // one of: -1, 0, 1?
    public:
        vehCar * GetCar() const {
            return this->car;
        }

        void SetCar(vehCar* car) {
            this->car = car;
        }

        AGE_API camCarCS(void) {
            scoped_vtable x(this);
            hook::Thunk<0x521470>::Call<void>(this);
        }

        virtual AGE_API ~camCarCS(void) {
            scoped_vtable x(this);
            hook::Thunk<0x521490>::Call<void>(this);
        }

        AGE_API void Init(vehCar *car, LPCSTR *name)        { hook::Thunk<0x5214A0>::Call<void>(this, car, name); }

        //overrides
        AGE_API void FileIO(datParser &parser) override     { hook::Thunk<0x5214E0>::Call<void>(this, &parser); }

        //lua
        static void BindLua(LuaState L) {
            LuaBinding(L).beginExtendClass<camCarCS, camAppCS>("camCarCS")
                .addProperty("Car", &GetCar, &SetCar)
                .addVariable("ReverseMode", &camCarCS::ReverseMode)
                .addFunction("Init", &Init)
            .endClass();
        }
    };
    ASSERT_SIZEOF(camCarCS, 0x110);

    class camPointCS : public camCarCS {
    protected:
        Vector3 position;
        Vector3 velocity;

        float maxDistSqr;
        float maxDist;
        float appRate;
        float minDist;
        float minDistSqr;
    public:
        AGE_API camPointCS(void) {
            scoped_vtable x(this);
            hook::Thunk<0x520610>::Call<void>(this);
        }

        virtual AGE_API ~camPointCS(void) {
            scoped_vtable x(this);
            hook::Thunk<0x406820>::Call<void>(this);
        }

        float GetMinDist() const {
            return this->minDist;
        }

        float GetMaxDist() const {
            return this->maxDist;
        }

        float GetAppRate()  const {
            return this->appRate;
        }
        AGE_API void SetAppRate(float rate)                 { hook::Thunk<0x520730>::Call<void>(this, rate); }
        AGE_API void SetMaxDist(float dist)                 { hook::Thunk<0x5206F0>::Call<void>(this, dist); }
        AGE_API void SetMinDist(float dist)                 { hook::Thunk<0x520710>::Call<void>(this, dist); }
        AGE_API void SetPos(Vector3 const &pos)             { hook::Thunk<0x5206B0>::Call<void>(this, &pos); }
        AGE_API void SetVel(Vector3 const &vel)             { hook::Thunk<0x520740>::Call<void>(this, &vel); }

        //camBaseCS overrides
        AGE_API void MakeActive() override                  { hook::Thunk<0x5206A0>::Call<void>(this); }

        //asNode overrides
        AGE_API void Reset() override                       { hook::Thunk<0x520690>::Call<void>(this); }
        AGE_API void Update() override                      { hook::Thunk<0x520790>::Call<void>(this); }
        AGE_API char * GetClassName() override              { return hook::Thunk<0x520A20>::Call<char*>(this); }

        //lua
        static void BindLua(LuaState L) {
            LuaBinding(L).beginExtendClass<camPointCS, camCarCS>("camPointCS")
                .addFactory([]() {  
                    auto obj = new camPointCS();
                    MM2Lua::MarkForCleanupOnShutdown(obj);
                    return obj;
                 })
                //properties
                .addProperty("MinDist", &GetMinDist, &SetMinDist)
                .addProperty("MaxDist", &GetMaxDist, &SetMaxDist)
                .addProperty("AppRate", &GetAppRate, &SetAppRate)
                .addFunction("SetPosition", &SetPos)
                .addFunction("SetVelocity", &SetVel)
            .endClass();
        }
    };
    ASSERT_SIZEOF(camPointCS, 0x13C);

    class camPovCS : public camCarCS {
    protected:
        int unk_110;

        Vector3 Offset;
        Vector3 ReverseOffset;
        float POVJitterAmp;
        
        Vector3 unk_130;

        float unk_13C; // related to camCarCS::unk_10C: if -1, use PI; otherwise, use this value (see: camPOVCS::Update @ 0x51D62A)
        float Pitch;
        float unk_144; // yaw? (roates matrix if != 0)
    private:
        AGE_API void UpdatePOV()                            { hook::Thunk<0x51D5A0>::Call<void>(this); }
    public:
        AGE_API camPovCS(void) {
            scoped_vtable x(this);
            hook::Thunk<0x51D410>::Call<void>(this);
        }

        virtual AGE_API ~camPovCS(void) {
            scoped_vtable x(this);
            hook::Thunk<0x51D520>::Call<void>(this);
        }

        //camBaseCS overrides
        AGE_API void MakeActive() override                  { hook::Thunk<0x51D530>::Call<void>(this); }
        AGE_API void UpdateInput() override                 { hook::Thunk<0x51D590>::Call<void>(this); }

        //asNode overrides
        AGE_API void Reset() override                       { hook::Thunk<0x51D540>::Call<void>(this); }
        AGE_API void Update() override                      { hook::Thunk<0x51D570>::Call<void>(this); }
        AGE_API void FileIO(datParser &parser) override     { hook::Thunk<0x51D690>::Call<void>(this, &parser); }
        AGE_API void AfterLoad() override                   { hook::Thunk<0x51D710>::Call<void>(this); }
        AGE_API char * GetClassName() override              { return hook::Thunk<0x51D760>::Call<char*>(this); }

        //lua
        static void BindLua(LuaState L) {
            LuaBinding(L).beginExtendClass<camPovCS, camCarCS>("camPovCS")
                .addFactory([]() {  
                    auto obj = new camPovCS();
                    MM2Lua::MarkForCleanupOnShutdown(obj);
                    return obj;
                 })
                .addVariable("Offset", &camPovCS::Offset)
                .addVariable("ReverseOffset", &camPovCS::ReverseOffset)
                .addVariable("Pitch", &camPovCS::Pitch)
                .addVariable("POVJitterAmp", &camPovCS::POVJitterAmp)
                .endClass();
        }
    };
    ASSERT_SIZEOF(camPovCS, 0x148);

    class camTrackCS : public camCarCS {
    protected:
        int dword_110;
        Vector3 Offset;
        int CollideType; //0, 1, or 2
        BOOL MinMaxOn;
        int TrackBreak; //0, 1, or 2
        float MinAppXZPos;
        float MaxAppXZPos;
        float MinSpeed;
        float MaxSpeed;
        float AppInc;
        float AppDec;
        float MinHardSteer;
        float DriftDelay;
        float VertOffset;
        float FrontRate;
        float RearRate;
        float FlipDelay;
        BOOL SteerOn;
        float SteerMin;
        float SteerAmt;
        float HillMin;
        float HillMax;
        float HillLerp;
        float RevDelay;
        float RevOnApp;
        float RevOffApp;
        byte _buffer[0x118];
    private:
        AGE_API void Collide(Vector3 a1)                    { hook::Thunk<0x51EED0>::Call<void>(this, a1); }
        AGE_API void Front(float a1)                        { hook::Thunk<0x51F980>::Call<void>(this, a1); }
        AGE_API void MinMax(Matrix34 a1)                    { hook::Thunk<0x51ECC0>::Call<void>(this, a1); }
        AGE_API void PreApproach()                          { hook::Thunk<0x51EB40>::Call<void>(this); }
        AGE_API void Rear(float a1)                         { hook::Thunk<0x51FA00>::Call<void>(this, a1); }

        AGE_API void UpdateCar()                            { hook::Thunk<0x51DC70>::Call<void>(this); }
        AGE_API void UpdateHill()                           { hook::Thunk<0x51DEE0>::Call<void>(this); }
        AGE_API void UpdateTrack()                          { hook::Thunk<0x51E400>::Call<void>(this); }
    public:
        AGE_API camTrackCS(void) {
            scoped_vtable x(this);
            hook::Thunk<0x51D770>::Call<void>(this);
        }

        virtual AGE_API ~camTrackCS(void) {
            scoped_vtable x(this);
            hook::Thunk<0x51DA80>::Call<void>(this);
        }

        AGE_API void SwingToRear()                          { hook::Thunk<0x51F920>::Call<void>(this); }

        bool GetSteerOn() const     { return SteerOn == TRUE; }
        void SetSteerOn(bool value) { SteerOn = (value) ? TRUE : FALSE; }
        bool GetMinMaxOn() const    { return MinMaxOn == TRUE; }
        void SetMinMaxOn(bool value) { MinMaxOn = (value) ? TRUE : FALSE; }

        //asNode overrides
        AGE_API void AfterLoad() override                   { hook::Thunk<0x51DAF0>::Call<void>(this); }
        AGE_API void FileIO(datParser &parser) override     { hook::Thunk<0x51FA80>::Call<void>(this, &parser); }
        AGE_API char * GetClassName() override              { return hook::Thunk<0x51FD60>::Call<char*>(this); }
        AGE_API void Reset() override                       { hook::Thunk<0x51DB00>::Call<void>(this); }
        AGE_API void Update() override                      { hook::Thunk<0x51DB50>::Call<void>(this); }

        //camBaseCS overrides
        AGE_API void MakeActive() override                  { hook::Thunk<0x51DAE0>::Call<void>(this); }
        AGE_API void UpdateInput() override                 { hook::Thunk<0x51DC60>::Call<void>(this); }

        //lua
        static void BindLua(LuaState L) {
            LuaBinding(L).beginExtendClass<camTrackCS, camCarCS>("camTrackCS")
                .addFactory([]() {  
                    auto obj = new camTrackCS();
                    MM2Lua::MarkForCleanupOnShutdown(obj);
                    return obj;
                 })
                .addFunction("SwingToRear", &SwingToRear)
                .addVariable("Offset", &camTrackCS::Offset)
                .addVariable("CollideType", &camTrackCS::CollideType)
                .addProperty("MinMaxOn", &GetMinMaxOn, &SetMinMaxOn)
                .addVariable("TrackBreak", &camTrackCS::TrackBreak)
                .addVariable("MinAppXZPos", &camTrackCS::MinAppXZPos)
                .addVariable("MaxAppXZPos", &camTrackCS::MaxAppXZPos)
                .addVariable("MinSpeed", &camTrackCS::MinSpeed)
                .addVariable("MaxSpeed", &camTrackCS::MaxSpeed)
                .addVariable("AppInc", &camTrackCS::AppInc)
                .addVariable("AppDec", &camTrackCS::AppDec)
                .addVariable("MinHardSteer", &camTrackCS::MinHardSteer)
                .addVariable("DriftDelay", &camTrackCS::DriftDelay)
                .addVariable("VertOffset", &camTrackCS::VertOffset)
                .addVariable("FrontRate", &camTrackCS::FrontRate)
                .addVariable("RearRate", &camTrackCS::RearRate)
                .addVariable("FlipDelay", &camTrackCS::FlipDelay)
                .addProperty("SteerOn", &GetSteerOn, &SetSteerOn)
                .addVariable("SteerMin", &camTrackCS::SteerMin)
                .addVariable("SteerAmt", &camTrackCS::SteerAmt)
                .addVariable("HillMin", &camTrackCS::HillMin)
                .addVariable("HillMax", &camTrackCS::HillMax)
                .addVariable("HillLerp", &camTrackCS::HillLerp)
                .addVariable("RevDelay", &camTrackCS::RevDelay)
                .addVariable("RevOnApp", &camTrackCS::RevOnApp)
                .addVariable("RevOffApp", &camTrackCS::RevOffApp)
                .endClass();
        }
    };
    ASSERT_SIZEOF(camTrackCS, 0x298);

    class camPolarCS : camCarCS {
    private:
        float PolarHeight;
        float PolarDistance;
        float PolarAzimuth;
        float PolarIncline;
        float PolarDelta;
        BOOL AzimuthLock;
    public:
        AGE_API camPolarCS(void) {
            scoped_vtable x(this);
            hook::Thunk<0x520FB0>::Call<void>(this);
        }

        virtual AGE_API ~camPolarCS(void) {
            scoped_vtable x(this);
            hook::Thunk<0x406800>::Call<void>(this);
        }

        bool GetAzimuthLock()
        {
            return this->AzimuthLock == TRUE;
        }

        void SetAzimuthLock(bool lock)
        {
            this->AzimuthLock = (lock) ? TRUE : FALSE;
        }

        //asNode overrides
        AGE_API void FileIO(datParser &parser) override     { hook::Thunk<0x521380 >::Call<void>(this, &parser); }
        AGE_API char * GetClassName() override              { return hook::Thunk<0x521460>::Call<char*>(this); }
        AGE_API void Reset() override                       { hook::Thunk<0x521010>::Call<void>(this); }
        AGE_API void Update() override                      { hook::Thunk<0x521030>::Call<void>(this); }

        //camBaseCS overrides
        AGE_API void MakeActive() override                  { hook::Thunk<0x521020>::Call<void>(this); }

        //lua
        static void BindLua(LuaState L) {
            LuaBinding(L).beginExtendClass<camPolarCS, camCarCS>("camPolarCS")
                .addFactory([]() {  
                    auto obj = new camPolarCS();
                    MM2Lua::MarkForCleanupOnShutdown(obj);
                    return obj;
                 })
                //properties
                .addProperty("AzimuthLock", &GetAzimuthLock, &SetAzimuthLock)
                .addVariable("Height", &camPolarCS::PolarHeight)
                .addVariable("Delta", &camPolarCS::PolarDelta)
                .addVariable("Distance", &camPolarCS::PolarDistance)
                .addVariable("Incline", &camPolarCS::PolarIncline)
                .addVariable("Azimuth", &camPolarCS::PolarAzimuth)
            .endClass();
        }
    };
    ASSERT_SIZEOF(camPolarCS, 0x128);

    class camAICS : public camCarCS {
    private:
        float Speed;
        float RotationSpeed;
        byte SegmentInfo[12]; // lvlSegmentInfo
    public:
        AGE_API camAICS(void) {
            scoped_vtable x(this);
            hook::Thunk<0x520A30>::Call<void>(this);
        }

        virtual AGE_API ~camAICS(void) {
            scoped_vtable x(this);
            hook::Thunk<0x406810>::Call<void>(this);
        }

        static float getHeight(void) {
            return *(float*)0x5D4340;
        }

        static void setHeight(float height) {
            auto heightPtr = (float*)0x5D4340;
            *heightPtr = height;
        }

        AGE_API void SetPosition(Vector3 *a1)               { hook::Thunk<0x520AC0>::Call<void>(this, a1); }
        AGE_API void SetMatrix(Matrix34 *a1)                { hook::Thunk<0x520AE0>::Call<void>(this, a1); }

        // asNode overrides
        AGE_API void Reset() override                       { hook::Thunk<0x520AB0>::Call<void>(this); }

        //lua
        static void BindLua(LuaState L) {
            LuaBinding(L).beginExtendClass<camAICS, camCarCS>("camAICS")
                .addFactory([]() {  
                    auto obj = new camAICS();
                    MM2Lua::MarkForCleanupOnShutdown(obj);
                    return obj;
                 })
                //properties
                .addStaticProperty("Height", &getHeight, &setHeight)
                .addVariable("Speed", &camAICS::Speed)
                .addVariable("RotationSpeed", &camAICS::RotationSpeed)
            .endClass();
        }
    };
    ASSERT_SIZEOF(camAICS, 0x124);

    class camTransitionCS : public camCarCS {
    private:
        camCarCS* FromCamera;
        camCarCS* ToCamera;
        camCarCS* dword_118;
        int dword_11c;
        int dword_120;
        int dword_124;
    public:
        AGE_API camTransitionCS(void) {
            scoped_vtable x(this);
            hook::Thunk<0x521530>::Call<void>(this);
        }

        virtual AGE_API ~camTransitionCS(void) {
            scoped_vtable x(this);
            hook::Thunk<0x5215C0>::Call<void>(this);
        }

        // asNode overrides
        AGE_API void Reset() override                       { hook::Thunk<0x520AB0>::Call<void>(this); }
        AGE_API void Update() override                      { hook::Thunk<0x521610>::Call<void>(this); }

        //lua
        static void BindLua(LuaState L) {
            LuaBinding(L).beginExtendClass<camTransitionCS, camCarCS>("camTransitionCS")
                .endClass();
        }
    };
    ASSERT_SIZEOF(camTransitionCS, 0x128);

    class camViewCS : public asNode {
    protected:
        BOOL ManualFOV;
        short LastTransitionType;
        float LastTransitionTime;

        datCallback callback;

        camCarCS *currentCamera;
        camCarCS *afterTransitionCamera;
        camTransitionCS *transitionCamera;

        // overrides
        bool UseGlobalNearFar; 

        float OverrideFar; // camera far? (4th param in gfxViewport::Perspective)
        float OverrideNear; // camera near? (3rd param in gfxViewport::Perspective)
        int unk_48; // no idea; completely unused?
    private:
        //helpers for lua calls
        bool NewCam(camCarCS* cam, int type, float time) {
            return NewCam(cam, type, time, datCallback::NullCallback) == TRUE;
        }
    public:
        AGE_API camViewCS(void) {
            scoped_vtable x(this);
            hook::Thunk<0x51FD70>::Call<void>(this);
        }

        virtual AGE_API ~camViewCS(void) {
            scoped_vtable x(this);
            hook::Thunk<0x51FE20>::Call<void>(this);
        }

        camCarCS * getCurrentCamera(void) const             { return currentCamera; }
        bool isCurrentCamera(camCarCS *cam) const           { return currentCamera == cam; }
        bool isTransitioning() const                        { return isCurrentCamera(this->transitionCamera); }

        AGE_API static camViewCS * Instance(vehCar *a1)     { return hook::StaticThunk<0x51FE30>::Call<camViewCS *>(a1); }

        AGE_API void SetCam(camCarCS *cam)                  { hook::Thunk<0x51FE90>::Call<void>(this, cam); }
        AGE_API void OneShot()                              { hook::Thunk<0x5200C0>::Call<void>(this); }

        AGE_API BOOL NewCam(camCarCS *cam, int type, float time, datCallback postTransitionCB)
                                                            { return hook::Thunk<0x51FF30>::Call<bool>(this, cam, type, time, postTransitionCB); }

        AGE_API void Init(vehCar *car)                      { hook::Thunk<0x51FFF0>::Call<void>(this, car); }

        AGE_API void ForceMatrixDelta(const Vector3 *a1)    { hook::Thunk<0x5200A0>::Call<void>(this, a1); }
        AGE_API void ForceMatrixDelta(const Matrix34 *a1)   { hook::Thunk<0x520080>::Call<void>(this, a1); }

        //overrides
        AGE_API void Update() override                      { hook::Thunk<0x51FFC0>::Call<void>(this); }
        AGE_API void Reset() override                       { hook::Thunk<0x520010>::Call<void>(this); }
        AGE_API void FileIO(datParser &parser) override     { hook::Thunk<0x5200D0>::Call<void>(this, &parser); }

        static void BindLua(LuaState L) {
            LuaBinding(L).beginExtendClass<camViewCS, asNode>("camViewCS")
                //properties
                .addStaticFunction("Instance", &Instance)
                .addPropertyReadOnly("CurrentCamera", &getCurrentCamera)
                .addPropertyReadOnly("IsTransitioning", &isTransitioning)
                .addFunction("IsCurrentCamera", &isCurrentCamera)
                .addFunction("SetCam", &SetCam)
                .addFunction("NewCam", static_cast<bool(camViewCS::*)(camCarCS *, int, float)>(&NewCam))
                .addFunction("Init", &Init)
                .addFunction("OneShot", &OneShot)
                .addFunction("ForceMatrixDelta", static_cast<void(camViewCS::*)(const Matrix34 *)>(&ForceMatrixDelta))

                .addVariable("UseGlobalNearFar", &camViewCS::UseGlobalNearFar)
                .addVariable("OverrideFar", &camViewCS::OverrideFar)
                .addVariable("OverrideNear", &camViewCS::OverrideNear)
            .endClass();
        }
    };

    ASSERT_SIZEOF(camViewCS, 0x4C);

    // Lua initialization
    template<>
    void luaAddModule<module_camera>(LuaState L) {
        //cam base classes
        luaBind<camBaseCS>(L);
        luaBind<camAppCS>(L);
        luaBind<camCarCS>(L);

        //camera types
        luaBind<camPointCS>(L);
        luaBind<camPovCS>(L);
        luaBind<camPolarCS>(L);
        luaBind<camAICS>(L);
        luaBind<camTrackCS>(L);

        //view
        luaBind<camViewCS>(L);
    }
}