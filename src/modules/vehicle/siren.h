#pragma once
#include <mm2_common.h>
#include <modules\lights\light.h>
#include <modules\lights\flare.h>

namespace MM2
{
    // Forward declarations

    // External declarations
    extern class ltLight;

    // Class definitions
    class vehSiren
    {
    private:
        static const int MAX_LIGHTS = 8;
    public:
        static float SirenRotationSpeed;
    private:
        bool HasLights;
        bool Active;
        int LightCount;
        ltLight* ltLightPool;
        Vector3 lightPositions[4]; //we didn't use this field because it conflicts with the extra light positions which doesn't make them read their mtx files properly.
        float RotationRate;
        ltLensFlare* LensFlarePtr;
        //EXTRA FIELD. The hook expands on this class, this is only possible because it's only used like a pointer in the original MM code
        Vector3 extraLightPositions[vehSiren::MAX_LIGHTS]; //SRN0-7
    public:
        bool IsActive() const      { return Active;}
        void SetActive(bool active) { Active = active; }
        bool GetHasLights() const  { return HasLights; }
        int GetLightCount() const  { return LightCount; }

        ltLight* GetLight(int index) const 
        {
            if (index < 0 || index >= vehSiren::MAX_LIGHTS)
                return nullptr;
            return &ltLightPool[index];
        }

        Vector3 GetLightPosition(int index) const 
        {
            if (index < 0 || index >= vehSiren::MAX_LIGHTS)
                return Vector3::ORIGIN;
            return extraLightPositions[index];
        }

        void SetLightPosition(int index, Vector3 position) 
        {
            if (index < 0 || index >= vehSiren::MAX_LIGHTS)
                return;
            extraLightPositions[index] = position;
        }

        //member funcs
        AGE_API bool Init()
        {
            if (this->ltLightPool == nullptr)
            {
                this->ltLightPool = new ltLight[vehSiren::MAX_LIGHTS];
            }
            if (this->LensFlarePtr == nullptr)
            {
                this->LensFlarePtr = new ltLensFlare(20);
            }
            return true;
        }

        void RemoveAllLights()
        {
            this->HasLights = false;
            this->LightCount = 0;
        }

        AGE_API bool AddLight(Vector3 const & position, Vector3 const & color)                    
        { 
            if (this->ltLightPool != nullptr && this->LightCount < vehSiren::MAX_LIGHTS)
            {
                this->HasLights = true;
                ltLightPool[LightCount].Type = 1;
                ltLightPool[LightCount].Color = color;
                extraLightPositions[LightCount] = position;
                ltLightPool[LightCount].SpotExponent = 3.f;
                ltLightPool[LightCount].Direction = Vector3(1.0f, 0.0f, 0.0f);
                ltLightPool[LightCount].Direction.RotateY(LightCount * 1.5707964f);
                ++this->LightCount;
                return true;
            }

            return false;
        }

        AGE_API void Reset()
        {
            this->Active = false;
        }

        AGE_API void Update()
        {
            float rotationAmount = vehSiren::SirenRotationSpeed;

            if (this->ltLightPool != nullptr && this->Active)
            {
                for (int i = 0; i < this->LightCount; i++)
                {
                    this->ltLightPool[i].Direction.RotateY(datTimeManager::Seconds * this->RotationRate * rotationAmount);
                }
            }
        }

        AGE_API void Draw(Matrix34 const& mtx)
        {
            if (this->ltLightPool == nullptr)
                return;

            ltLight::DrawGlowBegin();
            for (int i = 0; i < LightCount; i++)
            {
                auto light = &this->ltLightPool[i];
                auto lightPos = this->extraLightPositions[i];

                light->Position = mtx.Transform(lightPos);
                light->DrawGlow(static_cast<Vector3>(gfxRenderState::GetCameraMatrix().GetRow(3)));
            }
            ltLight::DrawGlowEnd();

            //draw lensFlare
            hook::Thunk<0x4D6880>::Call<void>(this, &mtx);
        }

        static void BindLua(LuaState L) {
            LuaBinding(L).beginClass<vehSiren>("vehSiren")
                //variables
                .addPropertyReadOnly("HasLights", &GetHasLights)
                .addPropertyReadOnly("LightCount", &GetLightCount)
                .addProperty("Active", &IsActive, &SetActive)
                .addVariable("RotationRate", &vehSiren::RotationRate)

                //lua members
                .addFunction("GetLight", &GetLight)
                .addFunction("GetLightPosition", &GetLightPosition)
                .addFunction("SetLightPosition", &SetLightPosition)

                //members
                .addFunction("Init", &Init)
                .addFunction("AddLight", &AddLight)
              
                .endClass();
        }
    };

    ASSERT_SIZEOF(vehSiren, 0x44 + (sizeof(Vector3) * 8)); // Base size + extra lights size
}