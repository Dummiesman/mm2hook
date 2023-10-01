#pragma once
#include <mm2_common.h>
#include "phbound.h"

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
        BOOL m_IsActive;
        Vector3 m_Center;
        float m_CurrentRadius;
        float m_ForceCoef;
        float m_ImpulseCoef;
        float m_MaxRadius;
        float m_RadiusRate;
        BOOL m_IsGrowing;
        float m_TimeLeftOriginal;
        float m_TimeLeft;
        float m_FadeFactor;
        Vector3 m_Direction;
        bool m_Directional;
    private:
        void UpdateBB()
        {
            float radius = this->m_CurrentRadius;
            this->Min = m_Center - Vector3(radius, radius, radius);
            this->Max = m_Center + Vector3(radius, radius, radius);
            this->Radius = radius;
        }
    public:
        phForceSphere()
        {
            scoped_vtable x(this);
            hook::Thunk<0x4871B0>::Call<void>(this, static_cast<int>(phBound::BoundType::ForceSphere));
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

            this->m_Direction = normalized;
            this->m_Directional = true;
        }

        void SetTimeAndFade(float time, float fade)
        {
            this->m_TimeLeft = time;
            this->m_FadeFactor = fade;
            this->m_TimeLeftOriginal = time;
        }

        void SetRadiusGrow(float radius, float maxRadius, float rate)
        {
            this->m_CurrentRadius = radius;
            this->m_MaxRadius = maxRadius;
            this->m_RadiusRate = rate;
            this->m_IsGrowing = TRUE;
            this->UpdateBB();
        }
        
        void SetRadius(float radius)
        {         
            this->m_CurrentRadius = radius;
            this->m_IsGrowing = FALSE;
            this->UpdateBB();
        }

        void SetCenter(const Vector3 & center)
        {
            this->m_Center = center;
            this->UpdateBB();
        }

        void Start()
        {
            this->m_IsActive = TRUE;
        }

        void Stop()
        {
            this->m_IsActive = FALSE;
        }

        void Reset()
        {
            this->m_IsActive = FALSE;
            this->m_IsGrowing = FALSE;
            this->m_TimeLeft = FLT_MAX;
        }

        void Update()
        {
            if (this->m_IsActive == FALSE)
                return;

            this->m_TimeLeft = this->m_TimeLeft - datTimeManager::Seconds;
            this->m_ForceCoef = this->m_ForceCoef * this->m_FadeFactor;

            if (this->m_TimeLeft < 0.f || (this->m_ImpulseCoef == 0.f && this->m_ForceCoef == 0.f))
            {
                this->m_IsActive = false;
                return;
            }

            if (this->m_IsGrowing == TRUE)
            {
                this->m_CurrentRadius += datTimeManager::Seconds * this->m_RadiusRate;
                if (this->m_CurrentRadius > this->m_MaxRadius)
                {
                    this->m_IsGrowing = false;
                    this->m_CurrentRadius = this->m_MaxRadius;
                }
                this->UpdateBB();
            }
        }

        //
        float getCurrentRadius()
        {
            return this->m_CurrentRadius;
        }

        float getMaxRadius()
        {
            return this->m_MaxRadius;
        }

        float getTimeLeft()
        {
            return this->m_TimeLeft;
        }

        bool getIsGrowing()
        {
            return this->m_IsGrowing == TRUE;
        }

        void setForceCoef(float force)
        {
            this->m_ForceCoef = force;
        }

        float getForceCoef()
        {
            return this->m_ForceCoef;
        }

        void setFadeFactor(float fadeFactor)
        {
            this->m_FadeFactor = fadeFactor;
        }

        float getFadeFactor()
        {
            return this->m_FadeFactor;
        }
        
        bool IsActive()
        {
            return this->m_IsActive == TRUE;
        }

        Vector3 getCenter()
        {
            return this->m_Center;
        }

        static void BindLua(LuaState L) {
            LuaBinding(L).beginExtendClass<phForceSphere, phBound>("phForceSphere")
                .addPropertyReadOnly("MaxRadius", &getMaxRadius)
                .addPropertyReadOnly("IsGrowing", &getIsGrowing)
                .addPropertyReadOnly("TimeLeft", &getTimeLeft)

                .addProperty("CurrentRadius", &getCurrentRadius, &SetRadius)
                .addProperty("Radius", &getCurrentRadius, &SetRadius)
                .addProperty("Center", &getCenter, &SetCenter)

                .addVariable("ForceCoef", &phForceSphere::m_ForceCoef)
                .addVariable("ImpulseCoef", &phForceSphere::m_ImpulseCoef)

                .addFunction("Direct", &Direct)
                .addFunction("Update", &Update)
                .endClass();
        }
    };
    //ASSERT_SIZEOF(phForceSpehere, 0x80);

    // Lua initialization

}