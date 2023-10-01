#pragma once
#include "mm2_common.h"
#include "mm2_level.h"

#include <modules\city.h>
#include <modules\mmcityinfo.h>

namespace MM2
{
    // Forward declarations
    class sdlPage16;

    // External declarations
    extern class gfxTexture;
    extern class Stream;
    extern class lvlLevel;

    namespace $
    {
        declhook(0x6299A8, _Type<cityTimeWeatherLighting[16]>, timeWeathers);

        namespace sdlCommon
        {
            declhook(0x45CBC0, _Func<bool>, BACKFACE);
            declhook(0x448090, _Func<void>, UpdateLighting);
        }
    }

    typename typedef void(*SDLIteratorCB)(const void *, int, int, int, const ushort *, void *);

    class sdlCommon {
    public:
        static hook::Type<int> sm_RoomCount;
        static hook::Type<Vector3> sm_CamPos;
        static hook::Type<uint> sm_LightTable;

        AGE_API static void UpdateLighting(void) {
            $::sdlCommon::UpdateLighting();
        }

        AGE_API static bool BACKFACE(const Vector3 &vec1, const Vector3 &vec2) {
            return $::sdlCommon::BACKFACE(&vec1, &vec2);
        }
    };

    class sdlPage16 {
        char unk_00;
        char PerimeterCount;
        char unk_02;
        char unk_03;
        Vector3 *CodedVertices;
        float *Floats;
        int unk_0C;
        unsigned short *PerimeterVertices;
        unsigned short *Attributes;
        gfxTexture **Textures;
        uint unk_1C;
    public:
        AGE_API sdlPage16(int numPerimeterPoints, int numAttributes) {
            scoped_vtable x(this);
            hook::Thunk<0x45A4E0>::Call<void>(this, numPerimeterPoints, numAttributes);
        }

        AGE_API static sdlPage16 * LoadBinary(Stream *stream) 
        {
            return hook::StaticThunk<0x45BF90>::Call<sdlPage16*>(stream);
        }

        AGE_API void Draw(int lod, uint baseColor) const 
        {
            hook::Thunk<0x448330>::Call<void>(this, lod, baseColor);
        }

        AGE_API void ArcMap(float *p1, const ushort *p2, int p3, int p4, int p5) const {
            hook::Thunk<0x45A560>::Call<void>(this, p1, p2, p3, p4, p5);
        }

        AGE_API void WallMap(float *p1, const ushort *p2, float p3, int p4, int p5) const {
            hook::Thunk<0x45A760>::Call<void>(this, p1, p2, p3, p4, p5);
        }

        AGE_API int Collide(const Vector4 & position, sdlPoly* polyBuf, int polyBufCount, int & pAttribute)
        {
            return hook::Thunk<0x455830>::Call<int>(this, &position, polyBuf, polyBufCount, &pAttribute);
        }

        AGE_API bool PointInPerimeter(float p1, float p2) const {
            return hook::Thunk<0x45A900>::Call<bool>(this, p1, p2);
        }

        AGE_API void GetCentroid(Vector3 &p1) const {
            hook::Thunk<0x45A9F0>::Call<void>(this, &p1);
        }

        AGE_API int GetPerimeterCount() const 
        {
            return hook::Thunk<0x45D110>::Call<int>(this);
        }

        AGE_API int GetPerimeterVertexIndex(int p1) const {
            return hook::Thunk<0x45D120>::Call<int>(this, p1);
        }

        AGE_API const Vector3 & GetCodedVertex(int p1) const {
            return hook::Thunk<0x45D140>::Call<const Vector3&>(this, p1);
        }

        AGE_API float GetFloat(int p1) const {
            return hook::Thunk<0x45D160>::Call<float>(this, p1);
        }

        AGE_API gfxTexture * GetTexture(int p1) const {
            return hook::Thunk<0x45D170>::Call<gfxTexture*>(this, p1);
        }

    public:
        /* these are originally private, but they're pretty helpful */

        AGE_API static uint GetShadedColor(uint p1, uint p2) 
        {
            return hook::StaticThunk<0x450880 >::Call<unsigned int>(p1, p2);
        }

        AGE_API static uint GetShadedColor(uint p1, uint p2, uint p3) 
        {
            return hook::StaticThunk<0x450910 >::Call<unsigned int>(p1, p2, p3);
        }
    };

    template<>
    void luaAddModule<module_city>(LuaState L) {
        luaBind<cityLevel>(L);

        luaBind<mmCityInfo>(L);
        luaBind<mmCityList>(L);

        luaBind<mmRaceData>(L);
        luaBind<mmInfoBase>(L);
        luaBind<mmPlayerData>(L);
        luaBind<dgStatePack>(L);
        luaBind<mmStatePack>(L);
        luaBind<mmPositions>(L);
    }
}