#pragma once
#include <modules\vector.h>

namespace MM2
{
    // Forward declarations
    class gfxPacket;
    class gfxPacketList;

    // External declarations


    // Class definitions
    static unsigned int gfxFVFOffset(unsigned int a1, unsigned int a2)
    {
        return hook::StaticThunk<0x4B2E80>::Call<unsigned int>(a1, a2);
    }

    static unsigned int gfxFVFSize(unsigned int a1)
    {
        return hook::StaticThunk<0x4B2E30>::Call<unsigned int>(a1);
    }

    class gfxPacket {
    public:
        static hook::Type<bool> gfxForceLVERTEX;
    public:
        gfxPacket* Next;
        IDirect3DVertexBuffer7* VertexBuffer;
        int* Positions;
        int* Vertices;
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
        Vector3 getPositionLua(int index) 
        {
            Vector3 vec;
            GetPosition(vec, index);
            return vec;
        }

        Vector3 getNormalLua(int index)
        {
            Vector3 vec;
            GetNormal(vec, index);
            return vec;
        }

        Vector2 getTexCoordLua(int index)
        {
            Vector2 vec;
            GetTexCoord(vec, index);
            return vec;
        }
    public:
        void DoLock()
        {
            hook::Thunk<0x4B4720>::Call<void>(this);
        }

        void DoUnlock()
        {
            hook::Thunk<0x4B4740>::Call<void>(this);
        }

        void GetPosition(Vector3 & out, int index)
        {
            hook::Thunk<0x4B3C10>::Call<void>(this, &out, index);
        }

        void GetNormal(Vector3 & out, int index) 
        {
            hook::Thunk<0x4B3CF0>::Call<void>(this, &out, index);
        }

        void GetTexCoord(Vector2 & out, int index)
        {
            hook::Thunk<0x4B3E20>::Call<void>(this, &out, index);
        }

        void GetTri(int* out, int index)
        {
            out[0] = Indices[(3 * index) + 0];
            out[1] = Indices[(3 * index) + 1];
            out[2] = Indices[(3 * index) + 2];
        }

        void SetNormal(const Vector3 & normal, int index) 
        {
            hook::Thunk<0x4B3D90>::Call<void>(this, &normal, index);
        }

        void SetPosition(const Vector3 & position, int index) 
        {
            hook::Thunk<0x4B3C70>::Call<void>(this, &position, index);
        }

        void SetTexCoord(const Vector2 & coord, int index) 
        {
            Vector2* texCoordPtr;
            if (this->StartVertex < 0)
            {
                int offset = gfxFVFSize(this->VertexTypeDesc) * index;
                texCoordPtr = (Vector2*)(this->Vertices + gfxFVFOffset(this->VertexTypeDesc, 0x100) + offset);
            }
            else 
            {
                int offset = gfxFVFSize(this->VertexTypeDesc) * (this->StartVertex + index);
                texCoordPtr = (Vector2*)(this->Positions + gfxFVFOffset(this->VertexTypeDesc, 0x100) + offset);
            }
            
            texCoordPtr->X = coord.X;
            texCoordPtr->Y = coord.Y;
        }

        int GetStartVertex() 
        {
            return this->StartVertex;
        }

        unsigned int GetAdjunctCount() const
        {
            return this->AdjunctCount;
        }

        unsigned int GetTriangleCount() const
        {
            return this->TriCount;
        }

        gfxPacket* GetNext() 
        {
            return this->Next;
        }

        static void BindLua(LuaState L) {
            LuaBinding(L).beginClass<gfxPacket>("gfxPacket")
                .addPropertyReadOnly("Next", &GetNext)
                .addPropertyReadOnly("NumAdjuncts", &GetAdjunctCount)
                .addPropertyReadOnly("NumTriangles", &GetTriangleCount)
                .addFunction("DoLock", &DoLock)
                .addFunction("DoUnlock", &DoUnlock)
                .addFunction("SetPosition", &SetPosition)
                .addFunction("GetPosition", &getPositionLua)
                .addFunction("SetNormal", &SetNormal)
                .addFunction("GetNormal", &getNormalLua)
                .addFunction("SetTexCoord", &SetTexCoord)
                .addFunction("GetTexCoord", &getTexCoordLua)
                .endClass();
        }
    };

    // yes, this is actually how it is in MM2
    // seems like Angel moved everything to gfxPacket

    class gfxPacketList : public gfxPacket {};
}