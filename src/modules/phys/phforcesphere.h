#pragma once
#include <modules\phys.h>
#include <mm2_common.h>

namespace MM2
{
    // Forward declarations
    class phForceSphere;

    // External declarations
    extern class phMaterial;

    // Class definitions
    class phForceSphere : public phBound
    {
    private:
        BOOL DoRunUpdate;
        Vector3 Center;
        float CurrentRadius;
        float ForceCoef;
        float ImpulseCoef;
        float MaxRadius;
        float RadiusRate;
        BOOL IsGrowing;
        float TimeLeftOriginal;
        float TimeLeft;
        float FadeFactor;
        Vector3 Direction;
        bool Directional;
    private:
        void UpdateBB()
        {
            float radius = this->CurrentRadius;
            this->Min = Center - Vector3(radius, radius, radius);
            this->Max = Center + Vector3(radius, radius, radius);
            this->Radius = radius;
        }
    public:
        phForceSphere() : phBound(BoundType::ForceSphere)
        {
            
        }

        /*
            phBound virtuals
        */
        AGE_API virtual phMaterial const* GetMaterial(int a1)  override
        {
            return nullptr;
        }

        AGE_API virtual bool TestProbePoint(phSegment& a1, phIntersectionPoint* a2, float a3)  override
        {
            //use phBoundSphere::TestProbePoint
            return hook::Thunk<0x484BC0>::Call<bool>(this, a1, a2, a3);
        }

        AGE_API virtual bool TestAIPoint(phSegment& a1, phIntersectionPoint* a2) override
        {
            //use phBoundSphere::TestAIPoint
            return hook::Thunk<0x484D70>::Call<bool>(this, a1, a2);
        }
            
        AGE_API virtual int  TestEdge(phSegment& a1, phIntersection* a2, int a3) override
        {
            //use phBoundSphere::TestEdge
            return hook::Thunk<0x484F10>::Call<int>(this, a1, a2, a3);
        }

        AGE_API virtual bool TestProbe(phSegment& a1, phIntersection* a2, float a3) override
        {
            //use phBoundSphere::TestProbe
            return hook::Thunk<0x485210>::Call<bool>(this, a1, a2, a3);
        }

        /*
            phForceSphere
        */
        void Direct(const Vector3 & direction)
        {
            Vector3 normalized = Vector3(direction);
            normalized.Normalize();

            this->Direction = normalized;
            this->Directional = true;
        }

        void SetTimeAndFade(float time, float fade)
        {
            this->TimeLeft = time;
            this->FadeFactor = fade;
            this->TimeLeftOriginal = time;
        }

        void SetRadiusGrow(float radius, float maxRadius, float rate)
        {
            this->CurrentRadius = radius;
            this->MaxRadius = maxRadius;
            this->RadiusRate = rate;
            this->IsGrowing = TRUE;
            this->UpdateBB();
        }
        
        void SetRadius(float radius)
        {
            this->CurrentRadius = radius;
            this->IsGrowing = FALSE;
            this->UpdateBB();
        }

        void SetCenter(const Vector3 & center)
        {
            this->Center = center;
            this->UpdateBB();
        }

        void Start()
        {
            this->DoRunUpdate = TRUE;
        }

        void Stop()
        {
            this->DoRunUpdate = FALSE;
        }

        void Reset()
        {
            this->DoRunUpdate = FALSE;
            this->IsGrowing = FALSE;
            this->TimeLeft = FLT_MAX;
        }

        void Update()
        {
            if (this->DoRunUpdate == FALSE)
                return;

            this->TimeLeft = this->TimeLeft - datTimeManager::Seconds;
            this->ForceCoef = this->ForceCoef * this->FadeFactor;

            if (this->TimeLeft < 0.f || (this->ImpulseCoef == 0.f && this->ForceCoef == 0.f))
            {
                this->DoRunUpdate = false;
                return;
            }

            if (this->IsGrowing == TRUE)
            {
                this->CurrentRadius += datTimeManager::Seconds * this->RadiusRate;
                if (this->CurrentRadius > this->MaxRadius)
                {
                    this->IsGrowing = false;
                    this->CurrentRadius = this->MaxRadius;
                }
                this->UpdateBB();
            }
        }

        //
        inline float getCurrentRadius()
        {
            return this->CurrentRadius;
        }

        inline float getMaxRadius()
        {
            return this->MaxRadius;
        }

        inline float getTimeLeft()
        {
            return this->TimeLeft;
        }

        inline bool getIsGrowing()
        {
            return this->IsGrowing == TRUE;
        }

        inline void setForceCoef(float force)
        {
            this->ForceCoef = force;
        }

        inline float getForceCoef()
        {
            return this->ForceCoef;
        }

        inline void setFadeFactor(float fadeFactor)
        {
            this->FadeFactor = fadeFactor;
        }

        inline float getFadeFactor()
        {
            return this->FadeFactor;
        }
        
        inline bool getIsRunning()
        {
            return this->DoRunUpdate == TRUE;
        }

        inline Vector3 getCenter()
        {
            return this->Center;
        }

        static void BindLua(LuaState L) {
            LuaBinding(L).beginExtendClass<phForceSphere, phBound>("phForceSphere")
                .addPropertyReadOnly("MaxRadius", &getMaxRadius)
                .addPropertyReadOnly("IsGrowing", &getIsGrowing)
                .addPropertyReadOnly("TimeLeft", &getTimeLeft)

                .addProperty("CurrentRadius", &getCurrentRadius, &SetRadius)
                .addProperty("Radius", &getCurrentRadius, &SetRadius)
                .addProperty("Center", &getCenter, &SetCenter)

                .addVariableRef("ForceCoef", &phForceSphere::ForceCoef)
                .addVariableRef("ImpulseCoef", &phForceSphere::ImpulseCoef)

                .addFunction("Direct", &Direct)
                .addFunction("Update", &Update)
                .endClass();
        }
    };
    ASSERT_SIZEOF(phBoundSphere, 0x80);

    // Lua initialization

}