#pragma once
#include <modules\vector.h>

namespace MM2
{
    // Forward declarations
    class gfxPacket;
    class gfxPacketList;

    // External declarations


    // Class definitions
    static unsigned int gfxFVFOffset(unsigned int fvf, unsigned int flag)
    {
        return hook::StaticThunk<0x4B2E80>::Call<unsigned int>(fvf, flag);
    }

    static unsigned int gfxFVFSize(unsigned int fvf)
    {
        return hook::StaticThunk<0x4B2E30>::Call<unsigned int>(fvf);
    }

    struct OrthoMapVertex
    {
        Vector3 position;
        int _reserved;
        int diffuse;
        int specular;
        Vector2 uv;
    };

    struct VertexVNT
    {
        Vector3 position;
        Vector3 normal;
        Vector2 uv;
    };

    class gfxPacket {
    private:
        static hook::Type<OrthoMapVertex> OrthoMapVerts;
    public:
        static hook::Type<bool> gfxForceLVERTEX;
    public:
        gfxPacket* Next;
        IDirect3DVertexBuffer7* VertexBuffer;
        char* Positions;
        char* Vertices;
        ushort VertexTypeDesc;
        short StartVertex;
        ushort AdjunctCount;
        ushort TriCount;
        ushort* Indices;
        byte* unk_1C;
        byte* unk_20;
        byte unk_24;
        uint unk_28;
    private:
        Vector3 getPositionLua(int index);
        Vector3 getNormalLua(int index);
        Vector2 getTexCoordLua(int index);
    public:
        static void BeginRef();
        bool NeedsLock() const;
        bool NeedsUnlock() const;
        void DoLock();
        void DoUnlock();
        void GetPosition(Vector3& out, int index);
        void GetNormal(Vector3& out, int index);
        void GetTexCoord(Vector2& out, int index);
        void GetTri(int* out, int index);
        void SetTri(int tri0, int tri1, int tri2, int index);
        void SetNormal(const Vector3& normal, int index);
        void SetPosition(const Vector3& position, int index);
        void SetTexCoord(const Vector2& coord, int index);
        int GetStartVertex() const;
        int GetFVF() const;
        unsigned int GetAdjunctCount() const;
        unsigned int GetTriangleCount() const;
        gfxPacket* GetNext();
        void Draw(int a2 = -1);
        void Draw(const Matrix44* a2, int a3 = -1);
        void OrthoMap(float scale);
        void OrthoMap(float scale, Vector3 offset);
        static void DrawList(gfxPacketList const* list);
        static void BindLua(LuaState L);
    };

    // yes, this is actually how it is in MM2
    // seems like Angel moved everything to gfxPacket

    class gfxPacketList : public gfxPacket {};
}