#pragma once

namespace MM2
{
    // Forward declarations
    class gfxPacket;
    class gfxPacketList;

    // External declarations


    // Class definitions
    class gfxPacket {
    public:
        static hook::Type<bool> gfxForceLVERTEX;

        gfxPacket *Next;
        IDirect3DVertexBuffer7 *VertexBuffer;
        int *Positions;
        void *Vertices;
        ushort VertexTypeDesc;
        short StartVertex;
        ushort AdjunctCount;
        ushort TriCount;
        ushort *Indices;
        byte* unk_1C;
        byte* unk_20;
        byte unk_24;
        uint unk_28;

        void DoLock()
        {
            hook::Thunk<0x4B4720>::Call<void>(this);
        }

        void DoUnlock()
        {
            hook::Thunk<0x4B4740>::Call<void>(this);
        }

        void GetPosition(Vector3* out, int index)
        {
            hook::Thunk<0x4B3C10>::Call<void>(this, out, index);
        }

        void GetNormal(Vector3* out, int index) {
            hook::Thunk<0x4B3CF0>::Call<void>(this, out, index);
        }

        void GetTexCoord(Vector2* out, int index)
        {
            hook::Thunk<0x4B3E20>::Call<void>(this, out, index);
        }

        void GetTri(int* out, int index)
        {
            out[0] = Indices[(3 * index) + 0];
            out[1] = Indices[(3 * index) + 1];
            out[2] = Indices[(3 * index) + 2];
        }

        void SetNormal(const Vector3& normal, int index) {
            hook::Thunk<0x4B3D90>::Call<void>(this, &normal, index);
        }

        void SetPosition(const Vector3& position, int index) {
            hook::Thunk<0x4B3C70>::Call<void>(this, &position, index);
        }
    };

    // yes, this is actually how it is in MM2
    // seems like Angel moved everything to gfxPacket

    class gfxPacketList : public gfxPacket {};

    // Lua initialization

}