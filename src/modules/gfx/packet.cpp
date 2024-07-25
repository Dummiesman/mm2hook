#pragma once
#include "packet.h"
#include <modules\gfx\stats.h>

using namespace MM2;

/*
    gfxPacket
*/

declfield(gfxPacket::gfxForceLVERTEX)(0x6857D8);

Vector3 gfxPacket::getPositionLua(int index) 
{
    Vector3 vec;
    GetPosition(vec, index);
    return vec;
}

Vector3 gfxPacket::getNormalLua(int index)
{
    Vector3 vec;
    GetNormal(vec, index);
    return vec;
}

Vector2 gfxPacket::getTexCoordLua(int index)
{
    Vector2 vec;
    GetTexCoord(vec, index);
    return vec;
}

void gfxPacket::BeginRef()
{
    // does nothing?
    hook::StaticThunk<0x4B3320>::Call<void>();
}

bool gfxPacket::NeedsLock() const
{
    return (StartVertex >= 0 && Positions == nullptr);
}

bool gfxPacket::NeedsUnlock() const
{
    return (StartVertex >= 0 && Positions != nullptr);
}

void gfxPacket::DoLock()
{
    if (NeedsLock()) 
    {
        hook::Thunk<0x4B4720>::Call<void>(this);
    }
}

void gfxPacket::DoUnlock()
{
    if (NeedsUnlock())
    {
        hook::Thunk<0x4B4740>::Call<void>(this);
    }
}

void gfxPacket::GetPosition(Vector3 & out, int index)
{
    hook::Thunk<0x4B3C10>::Call<void>(this, &out, index);
}

void gfxPacket::GetNormal(Vector3 & out, int index) 
{
    hook::Thunk<0x4B3CF0>::Call<void>(this, &out, index);
}

void gfxPacket::GetTexCoord(Vector2 & out, int index)
{
    hook::Thunk<0x4B3E20>::Call<void>(this, &out, index);
}

void gfxPacket::GetTri(int* out, int index)
{
    out[0] = Indices[(3 * index) + 0];
    out[1] = Indices[(3 * index) + 1];
    out[2] = Indices[(3 * index) + 2];
}

void gfxPacket::SetTri(int tri0, int tri1, int tri2, int index)
{
    Indices[(3 * index) + 0] = tri0;
    Indices[(3 * index) + 1] = tri1;
    Indices[(3 * index) + 2] = tri2;
}

void gfxPacket::SetNormal(const Vector3 & normal, int index) 
{
    hook::Thunk<0x4B3D90>::Call<void>(this, &normal, index);
}

void gfxPacket::SetPosition(const Vector3 & position, int index) 
{
    hook::Thunk<0x4B3C70>::Call<void>(this, &position, index);
}

void gfxPacket::SetTexCoord(const Vector2 & coord, int index) 
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

int gfxPacket::GetStartVertex() const
{
    return this->StartVertex;
}

int MM2::gfxPacket::GetFVF() const
{
    return this->VertexTypeDesc;
}

unsigned int gfxPacket::GetAdjunctCount() const
{
    return this->AdjunctCount;
}

unsigned int gfxPacket::GetTriangleCount() const
{
    return this->TriCount;
}

gfxPacket*gfxPacket:: GetNext() 
{
    return this->Next;
}

void gfxPacket::Draw(int a2)
{
    gfxVerts += this->AdjunctCount;
    gfxTris += this->TriCount / 3;
    hook::Thunk<0x4B3340>::Call<void>(this, a2);
}

void MM2::gfxPacket::Draw(const Matrix44* a2, int a3)
{
    gfxVerts += this->AdjunctCount;
    gfxTris += this->TriCount / 3;
    hook::Thunk<0x4B34E0>::Call<void>(this, a2, a3);
}

void gfxPacket::DrawList(gfxPacketList const * list)
{
    hook::StaticThunk<0x4B4870>::Call<void>(list);
}

void gfxPacket::BindLua(LuaState L) {
    LuaBinding(L).beginClass<gfxPacket>("gfxPacket")
        .addPropertyReadOnly("Next", &GetNext)
        .addPropertyReadOnly("NumAdjuncts", &GetAdjunctCount)
        .addPropertyReadOnly("NumTriangles", &GetTriangleCount)
        .addPropertyReadOnly("FVF", &GetFVF)
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