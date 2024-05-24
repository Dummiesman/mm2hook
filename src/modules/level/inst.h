#pragma once
#include <mm2_common.h>

namespace MM2
{
    // Forward declarations
    class lvlInstance;
    class lvlInstanceLuaIterator;

    // External declarations
    extern class cltLight;
    extern class dgPhysEntity;
    extern class gfxViewport;
    extern class modStatic;
    extern class modShader;
    extern class modPackage;
    extern class phBound;

    // External declarations for Lua casting

    // Class definitions
    class lvlInstance
    {
    public:
        // Instance flags
        static const int INST_BANGER = 1 << 1;
        static const int INST_VEHICLE = 1 << 3;
        static const int INST_WHEELCOLLISION = 1 << 5;
        static const int INST_SHADOW = 1 << 6;
        static const int INST_GLOW = 1 << 7;
        static const int INST_LANDMARK = 1 << 8;
        static const int INST_VISIBLE = 1 << 9;
        static const int INST_STATIC = 1 << 10;
        static const int INST_SHADOW_RGL = 1 << 13;
    private:
        byte Owner;
        byte Subtype;

        short room;
        ushort Flags;

        ushort GeomIndex;

        lvlInstance* Previous;
        lvlInstance *Next;

        int castLua(lua_State* L);
        int getBoundLua(lua_State* L, int a1);
    protected:
        static AGE_API int GetGeomSet(char const* a1, char const* a2, int a3);
        static AGE_API void CreateTempBounds();
        static AGE_API void DeleteTempBounds();
    public:
        struct GeomTableEntry
        {
            modStatic *LOD[4];
            modShader **pShaders;
            phBound *Bound;
            float Radius;
            char numShadersPerVariant;
            char numShaders;

            modStatic* GetLOD(int lod) const;
            modStatic* GetHighestLOD() const;
            modStatic* GetLowestLOD() const;
            modStatic* GetVeryLowLOD() const;
            modStatic* GetLowLOD() const;
            modStatic* GetMedLOD() const;
            modStatic* GetHighLOD() const;
            float GetRadius() const;
            phBound* GetBound() const;

            static void BindLua(LuaState L);

        };
        ASSERT_SIZEOF(GeomTableEntry, 32);

    protected:
        static GeomTableEntry* GetGeomTablePtr();
        static const char** GetGeomNameTablePtr();
        static int GetGeomSetCount();
    private:
        Matrix34 getMatrixLua();
        Vector3 getPositionLua();
        void setPositionLua(Vector3 position);
        modShader* getShaderLua(int variant, int index);
        static std::tuple<bool, Matrix34> computeShadowMatrixLua(Matrix34 const& inMtx, int room);
    public:
        LEVEL_ALLOCATOR

        AGE_API lvlInstance(void);
        AGE_API ~lvlInstance(void);

        lvlInstance* GetNext() const;
        lvlInstance* GetPrevious() const;
        short GetRoomId() const;
        short GetGeomIndex() const;
        void SetGeomIndex(unsigned short id);
        short GetFlags() const;
        void SetFlags(ushort flags);
        void SetFlag(ushort flag);
        byte GetOwner(void) const;
        void SetOwner(byte owner);
        byte GetSubType(void) const;
        void SetSubType(byte subtype);
        GeomTableEntry* GetGeomBase(int geom = 0) const;
        modStatic* GetGeom(int lod, int geom) const;
        const char* GetName() const;
        modShader* GetShader(int index) const;
        int GetShaderCount() const;
        int GetVariantCount() const;

        static AGE_API bool ComputeShadowMatrix(Matrix34& outMatrix, int room, Matrix34 const& inMatrix);
        static AGE_API bool ComputeShadowMatrix(Matrix34& outMatrix, int room, Matrix34 const& inMatrix, lvlInstance* ignoreInstance);
        static AGE_API bool ComputeShadowProjectionMatrix(Matrix34& outMatrix, int room, float lightPitch, float lightHeading, Matrix34 const& inMatrix);
        static AGE_API bool ComputeShadowProjectionMatrix(Matrix34& outMatrix, int room, float lightPitch, float lightHeading, Matrix34 const& inMatrix, lvlInstance* ignoreInstance);
        static AGE_API void ResetInstanceHeap();
        static AGE_API void ResetAll();;
        static AGE_API void SetShadowBillboardMtx(Matrix44& a1);
        static AGE_API int AddSphere(float a1);
        AGE_API bool LoadBoundOnLastEntry(const char* a1);
        AGE_API Vector4* GetBoundSphere(Vector4* a1);

        AGE_API bool BeginGeom(const char* a1, const char* a2, int a3);
        AGE_API int AddGeom(const char* a1, const char* a2, int a3);
        AGE_API void EndGeom();
        AGE_API bool InitBoundTerrain(const char* a1);
        AGE_API bool InitBoundTerrainLocal(const char* a1);
        AGE_API bool NeedGhostBound(const Vector3* a1, int a2);
        AGE_API bool InitGhostBound(phBound* a1, const Vector3* a2, int a3);
        AGE_API int InitGhost(const char* a1, const Matrix34& a2);
        AGE_API void PreLoadShader(int variant);
        AGE_API void Optimize(int variant);

        /*
            Custom additions
        */
        bool BeginGeomWithGroup(const char* a1, const char* a2, const char* group, int a3);

        /*
            lvlInstance virtuals
        */

        virtual AGE_API void Reset();
        virtual AGE_API const Vector3 & GetPosition() PURE;
        virtual AGE_API int IsVisible(const gfxViewport* a1);
        virtual AGE_API const Matrix34 & GetMatrix(Matrix34 *a1) PURE;
        virtual AGE_API void SetMatrix(const Matrix34 & a1) PURE;
        
        virtual AGE_API void SetVariant(int a1);
        virtual AGE_API const float GetRadius();
        virtual AGE_API dgPhysEntity* GetEntity();
        virtual AGE_API dgPhysEntity* AttachEntity();
        virtual AGE_API const Vector3& GetVelocity();
        virtual AGE_API void Detach();
        
        virtual AGE_API void Draw(int) PURE;
        virtual AGE_API void DrawShadow();
        virtual AGE_API void DrawShadowMap();
        virtual AGE_API void DrawGlow();
        virtual AGE_API void DrawReflected(float a1);
        virtual AGE_API void DrawReflectedParts(int a1);
        virtual AGE_API int Init(const char* a1, const Matrix34& a2, int a3);

        virtual AGE_API unsigned int SizeOf(void) PURE;

        virtual AGE_API bool IsLandmark();
        virtual AGE_API bool IsCollidable();
        virtual AGE_API bool IsTerrainCollidable();
        virtual AGE_API int GetNumLightSources();
        virtual AGE_API void GetLightInfo(int a1, cltLight* a2);
        virtual AGE_API int SetupGfxLights(const Matrix34& a1);
        virtual AGE_API phBound* GetBound(int a1);

        static void BindLuaLate(LuaState L);
        static void BindLua(LuaState L);
    };

    class LvlInstanceLuaIterator : public CppFunctor
    {
    private:
        lvlInstance* last;
        lvlInstance* end;
    public:
        LvlInstanceLuaIterator(lvlInstance* instance);
        LvlInstanceLuaIterator(lvlInstance* instance, lvlInstance* end);
        virtual ~LvlInstanceLuaIterator();

        virtual int run(lua_State* L) override;
    };
    // Lua initialization

}