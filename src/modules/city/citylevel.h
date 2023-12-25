#pragma once
#include <mm2_common.h>
#include "..\level\level.h"
#include "..\level\sky.h"
#include "psdl.h"

namespace MM2
{
    // Forward declarations
    struct cityTimeWeatherLighting;
    class cityLevel;
    
    // External declarations
    extern class gfxViewport;
    extern class lvlInstance;
    extern struct lvlSegment;
    extern struct lvlIntersection;

    // Class definitions
    struct cityTimeWeatherLighting {
        float KeyHeading;
        float KeyPitch;
        Vector3 KeyColor;

        float Fill1Heading;
        float Fill1Pitch;
        Vector3 Fill1Color;

        float Fill2Heading;
        float Fill2Pitch;
        Vector3 Fill2Color;

        // "ambient light levels" that get calculated using the Ambient color?
        // doesn't care for any of the Key/Pitch values
        int AmbientLevel3; // fullbright
        int AmbientLevel2;
        int AmbientLevel1;

        int Ambient; // looks like the alpha is ignored

        void ComputeAmbientLightLevels();
    };

    class cityLevel : public lvlLevel {
    private:
        int unk28;
    private:
        static hook::Type<unsigned char[512]> sm_PvsBuffer;
        static hook::Type<bool> sm_EnablePVS;
        static hook::Type<lvlSDL> SDL;
    public:
        static hook::Type<lvlSky> Sky;

        AGE_API cityLevel();
        virtual AGE_API ~cityLevel();

        /*
            lvlLevel virtuals
        */

        AGE_API virtual void Load(char const* a1) override;
        AGE_API virtual void Update() override;
        AGE_API virtual void PreDraw() override;
        AGE_API virtual void PostDraw() override;
        AGE_API virtual void Draw(const gfxViewport& a1, uint a2) override;
        AGE_API virtual int FindRoomId(Vector3 const& a1, int a2) const override;
        AGE_API virtual int GetNeighborCount(int a1) const override;
        AGE_API virtual int GetNeighbors(int* neighbourRooms, int room) const override;
        AGE_API virtual int GetTouchedNeighbors(int* a1, int a2, int a3, const Vector4& a4) override;
        AGE_API virtual int GetRoomPerimeter(int roomId, Vector3* out, int outSize) const override;
        AGE_API virtual int GetVisitList(int* a1, int a2, Vector3 const& a3, Vector3 const& a4, int a5, int a6) override;
        AGE_API virtual bool Collide(int a1, lvlSegment& a2, lvlIntersection& a3) override;
        AGE_API virtual bool GetBoundSphere(Vector4& a1, int a2) override;
        AGE_API virtual const class lvlLevelBound* GetBound() override;
        AGE_API virtual void SetObjectDetail(int a1) override;
        AGE_API virtual float GetWaterLevel(int a1) const override;
        AGE_API virtual float GetLightingIntensity(Vector3 const& a1) const override;
        AGE_API virtual void SetPtxHeight(asParticles& a1) override;
        AGE_API virtual gfxTexture* GetEnvMap(int room, Vector3 const& position, float& intensity) override;


        /*
            cityLevel functions
        */
        AGE_API void EnablePVS(bool enable);
        AGE_API void EnableSky(bool enable);
        AGE_API static void LoadPath(const char* a1, const Matrix34& a2, bool a3);
        AGE_API static void LoadPathSet(const char* a1, const char* a2);
        AGE_API static void LoadProp(int a1, const char* a2, const Matrix34& a3);
        AGE_API void DrawRooms(const gfxViewport* a1, uint a2, LPVOID a3, int a4); //should be protected but we use this in the hook
        AGE_API static void SetupLighting(Vector3 const& multiplyColor);
        
        /*
            Helpers
        */
        bool IsRoomVisible(int roomId) const;
        static lvlSDL* GetSDL();

        //lua
        static void BindLua(LuaState L);
    };

    // Lua initialization

}