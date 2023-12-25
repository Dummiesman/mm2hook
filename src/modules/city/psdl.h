#pragma once
#include <modules\phys.h>
namespace MM2
{
    // Forward declarations
    class sdlCommon;
    class lvlSDL;
    struct sdlPoly;
    struct sdlPolyCached;
    class sdlPage16;
    class sdlPolyCollection;

    // External declarations


    // Class definitions
    class sdlCommon {
    public:
        static hook::Type<int> sm_RoomCount;
        static hook::Type<Vector3> sm_CamPos;
        static hook::Type<uint> sm_LightTable;
        static hook::Type<ushort> sm_CurrentRoom;

        AGE_API static void UpdateLighting(void) {
            hook::StaticThunk<0x448090>::Call<void>();
        }

        AGE_API static bool BACKFACE(const Vector3& vec1, const Vector3& vec2) {
            return  hook::StaticThunk< 0x45CBC0>::Call<bool>(&vec1, &vec2);
        }
    };

    class lvlSDL  // TODO: Does this extend sdlCommon?
    {
    private:
        static hook::Type<Vector3*> sdlVertices;
        static hook::Field<0x54, sdlPage16**> _rooms;
    public:
        static Vector3* GetVerts()
        {
            return sdlVertices.get();
        }

        sdlPage16* GetPage(int roomId) const
        {
            return _rooms.get(this)[roomId];
        }

        static void BindLua(LuaState L)
        {
            LuaBinding(L).beginClass<lvlSDL>("lvlSDL")
                .addFunction("GetPage", &GetPage)
                .endClass();
        }
    };

    struct sdlPoly : phPolygon 
    {
    public:
        static hook::Type<int> sm_Available;
        static hook::Type<int> sm_Count;
        static hook::Type<Vector3*> sm_Verts;
    public:
        static void BindLua(LuaState L) {
            LuaBinding(L).beginExtendClass<sdlPoly, phPolygon>("sdlPoly")
                .endClass();
        }
    };

    struct sdlPolyCached : public sdlPoly
    {
        Vector3 Verts[4];
    };
    ASSERT_SIZEOF(sdlPolyCached, 0x90);

    // For Lua to use Collect()
    class sdlPolyCollection
    {
    private:
        sdlPoly* m_Polys;
        int m_PolyCount;
        int m_Used;
    public:
        sdlPolyCollection(int count)
        {
            m_Polys = new sdlPoly[count];
            m_PolyCount = count;
            m_Used = 0;
        }

        ~sdlPolyCollection()
        {
            delete[] m_Polys;
        }

        void BeginCollect(int room)
        {
            sdlPoly::sm_Available.set(m_PolyCount);
            sdlPoly::sm_Count.set(0);
            sdlPoly::sm_Verts.set(lvlSDL::GetVerts());
            sdlCommon::sm_CurrentRoom.set(static_cast<ushort>(room));
        }

        void EndCollect()
        {
            sdlPoly::sm_Available.set(m_PolyCount);
            sdlPoly::sm_Count.set(0);
            sdlPoly::sm_Verts.set(nullptr);
        }

        void SetUsed(int count)         { m_Used = count; }
        int GetUsed() const             { return m_Used; }

        int GetCount() const            { return m_PolyCount; }

        sdlPoly* GetPoly(int num) const { return (num >= 0 && num < m_PolyCount) ? &m_Polys[num] : nullptr; }
        sdlPoly* GetPolyBuf()           { return m_Polys; }

        static void BindLua(LuaState L)
        {
            LuaBinding(L).beginClass<sdlPolyCollection>("sdlPolyCollection")
                .addConstructor(LUA_ARGS(int))
                .addPropertyReadOnly("Used", &GetUsed)
                .addFunction("GetPolygon", &GetPoly)
                .addFunction("BeginCollect", &BeginCollect)
                .addFunction("EndCollect", &EndCollect)
                .endClass();
        }
    };

    class sdlPage16 {
        char unk_00;
        char PerimeterCount;
        char unk_02;
        char unk_03;
        Vector3* CodedVertices;
        float* Floats;
        int unk_0C;
        unsigned short* PerimeterVertices;
        unsigned short* Attributes;
        gfxTexture** Textures;
        uint unk_1C;
    private:
        void CollectLua(sdlPolyCollection* collection)
        {
            int attributePtr = 0;
            int used = this->Collect(nullptr, collection->GetPolyBuf(), collection->GetCount(), &attributePtr);
            collection->SetUsed(used);
        }

        void CollectAtLua(sdlPolyCollection* collection, Vector4 posradius)
        {
            int attributePtr = 0;
            int used = this->Collect(&posradius, collection->GetPolyBuf(), collection->GetCount(), &attributePtr);
            collection->SetUsed(used);
        }
    public:
        AGE_API sdlPage16(int numPerimeterPoints, int numAttributes) {
            scoped_vtable x(this);
            hook::Thunk<0x45A4E0>::Call<void>(this, numPerimeterPoints, numAttributes);
        }

        AGE_API static sdlPage16* LoadBinary(Stream* stream)
        {
            return hook::StaticThunk<0x45BF90>::Call<sdlPage16*>(stream);
        }

        AGE_API void Draw(int lod, uint baseColor) const
        {
            hook::Thunk<0x448330>::Call<void>(this, lod, baseColor);
        }

        AGE_API void ArcMap(float* p1, const ushort* p2, int p3, int p4, int p5) const {
            hook::Thunk<0x45A560>::Call<void>(this, p1, p2, p3, p4, p5);
        }

        AGE_API void WallMap(float* p1, const ushort* p2, float p3, int p4, int p5) const {
            hook::Thunk<0x45A760>::Call<void>(this, p1, p2, p3, p4, p5);
        }

        AGE_API int Collide(const Vector4& position, sdlPoly* polyBuf, int polyBufCount, int& pAttribute)
        {
            return hook::Thunk<0x455830>::Call<int>(this, &position, polyBuf, polyBufCount, &pAttribute);
        }

        AGE_API bool PointInPerimeter(float p1, float p2) const {
            return hook::Thunk<0x45A900>::Call<bool>(this, p1, p2);
        }

        AGE_API void GetCentroid(Vector3& p1) const {
            hook::Thunk<0x45A9F0>::Call<void>(this, &p1);
        }

        AGE_API int GetPerimeterCount() const
        {
            return hook::Thunk<0x45D110>::Call<int>(this);
        }

        AGE_API int GetPerimeterVertexIndex(int p1) const {
            return hook::Thunk<0x45D120>::Call<int>(this, p1);
        }

        AGE_API const Vector3& GetCodedVertex(int p1) const {
            return hook::Thunk<0x45D140>::Call<const Vector3&>(this, p1);
        }

        AGE_API float GetFloat(int p1) const {
            return hook::Thunk<0x45D160>::Call<float>(this, p1);
        }

        AGE_API gfxTexture* GetTexture(int p1) const {
            return hook::Thunk<0x45D170>::Call<gfxTexture*>(this, p1);
        }

        AGE_API int Collect(Vector4 const* posradius, sdlPoly* polyBuf, int polyBufCount, int* pAttribute)
        {
            return hook::Thunk<0x455830>::Call<int>(this, posradius, polyBuf, polyBufCount, pAttribute);
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
    public:
        static void BindLua(LuaState L)
        {
            LuaBinding(L).beginClass<sdlPage16>("sdlPage16")
                .addFunction("Collect", &CollectLua)
                .addFunction("CollectAt", &CollectAtLua)
                .addFunction("GetCodedVertex", &GetCodedVertex)
                .addFunction("GetFloat", &GetFloat)
                .addFunction("GetTexture", &GetTexture)
                .addFunction("PointInPerimeter", &PointInPerimeter)
                .endClass();
        }
    };
    // Lua initialization

}