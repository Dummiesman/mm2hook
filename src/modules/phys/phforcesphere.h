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
            float radius = m_CurrentRadius;
            Min = m_Center - Vector3(radius, radius, radius);
            Max = m_Center + Vector3(radius, radius, radius);
            Radius = radius;
        }
    public:
        phForceSphere()
        {
            scoped_vtable x(this);

            m_IsActive = false;
            hook::Thunk<0x4871B0>::Call<void>(this, static_cast<int>(phBound::BoundType::ForceSphere));
            Zero();
            m_Directional = false;
        }

        /*
            phBound virtuals
        */
        AGE_API virtual phMaterial const* GetMaterial(int a1)  override
        {
            return nullptr;
        }

        AGE_API virtual bool TestAIPoint(phSegment& a1, phIntersectionPoint* a2) override
        {
            Warningf("phForceSphere::TestAIPoint");
            //use phBoundSphere::TestAIPoint
            return hook::Thunk<0x484D70>::Call<bool>(this, a1, a2);
        }
            
        AGE_API virtual int  TestEdge(phSegment& a1, phIntersection* a2, int a3) override
        {
            Warningf("phForceSphere::TestEdge");
            //use phBoundSphere::TestEdge
            return hook::Thunk<0x484F10>::Call<int>(this, a1, a2, a3);
        }

        AGE_API virtual bool TestProbe(phSegment& a1, phIntersection* a2, float a3) override
        {
            Warningf("phForceSphere::TestProbe");
            float radiusSquared = Radius * Radius;
            Vector3 startDifference = a2->StartPos - this->m_Center;
            Vector3 endDifference = a2->EndPos - this->m_Center;

            if (startDifference.Mag2() >= radiusSquared)
            {
                Vector3 dir = endDifference - startDifference;
                float normalizedDist, unk;
                if (SegmentToSphereIntersections(startDifference, dir, radiusSquared, &normalizedDist, &unk))
                {
                    auto& point = a2->IntersectionPoint;
                    point.Set(startDifference + (dir * normalizedDist), startDifference + (dir * normalizedDist), normalizedDist, 1.0f, true);
                    return true;
                }
            }

            return false;
        }

        AGE_API virtual bool TestProbePoint(phSegment& a1, phIntersectionPoint* a2, float a3)  override
        {
            Warningf("phForceSphere::TestProbePoint");
            //use phBoundSphere::TestProbePoint
            return hook::Thunk<0x484BC0>::Call<bool>(this, a1, a2, a3);
        }

        /*
            phForceSphere
        */
        void Zero()
        {
            m_ForceCoef = 0.0;
            m_ImpulseCoef = 0.0;
            m_Center.X = 0.0;
            m_Center.Y = 0.0;
            m_Center.Z = 0.0;
            m_CurrentRadius = 0.0;
            m_IsGrowing = false;
            UpdateBB();
            m_FadeFactor = 1.0f;
            m_TimeLeft = FLT_MAX;
        }

        void Direct(const Vector3 & direction)
        {
            Vector3 normalized = Vector3(direction);
            normalized.Normalize();

            m_Direction = normalized;
            m_Directional = true;
        }

        void SetTimeAndFade(float time, float fade)
        {
            m_TimeLeft = time;
            m_FadeFactor = fade;
            m_TimeLeftOriginal = time;
        }

        void SetRadiusGrow(float radius, float maxRadius, float rate)
        {
            m_CurrentRadius = radius;
            m_MaxRadius = maxRadius;
            m_RadiusRate = rate;
            m_IsGrowing = TRUE;
            UpdateBB();
        }
        
        void SetRadius(float radius)
        {         
            m_CurrentRadius = radius;
            m_IsGrowing = FALSE;
            UpdateBB();
        }

        void SetCenter(const Vector3 & center)
        {
            m_Center = center;
            UpdateBB();
        }

        void Start()
        {
            m_IsActive = TRUE;
        }

        void Stop()
        {
            m_IsActive = FALSE;
        }

        void Reset()
        {
            m_IsActive = FALSE;
            m_IsGrowing = FALSE;
            m_TimeLeft = FLT_MAX;
        }

        void Update()
        {
            if (m_IsActive == FALSE)
                return;

            m_TimeLeft = m_TimeLeft - datTimeManager::Seconds;
            m_ForceCoef = m_ForceCoef * m_FadeFactor;

            if (m_TimeLeft < 0.f || (m_ImpulseCoef == 0.f && m_ForceCoef == 0.f))
            {
                m_IsActive = false;
                return;
            }

            if (m_IsGrowing == TRUE)
            {
                m_CurrentRadius += datTimeManager::Seconds * m_RadiusRate;
                if (m_CurrentRadius > m_MaxRadius)
                {
                    m_IsGrowing = false;
                    m_CurrentRadius = m_MaxRadius;
                }
                UpdateBB();
            }
        }

        //
        float getCurrentRadius()
        {
            return m_CurrentRadius;
        }

        float getMaxRadius()
        {
            return m_MaxRadius;
        }

        float getTimeLeft()
        {
            return m_TimeLeft;
        }

        bool getIsGrowing()
        {
            return m_IsGrowing == TRUE;
        }

        void setForceCoef(float force)
        {
            m_ForceCoef = force;
        }

        float getForceCoef()
        {
            return m_ForceCoef;
        }

        void setFadeFactor(float fadeFactor)
        {
            m_FadeFactor = fadeFactor;
        }

        float getFadeFactor()
        {
            return m_FadeFactor;
        }
        
        bool IsActive()
        {
            return m_IsActive == TRUE;
        }

        Vector3 getCenter()
        {
            return m_Center;
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