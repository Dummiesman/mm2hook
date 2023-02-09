#pragma once
#include <modules\phys.h>

namespace MM2
{
    // Forward declarations
    class phBoundPolygonal;
    struct phHotEdge;

    // External declarations

    // Class definitions
    struct phHotEdge
    {
        ushort Indices[2];
    
        short getIndex(int i)
        {
            if (i < 0 || i >= 2)
                return -1;
            return (short)this->Indices[i];
        }
        
        static void BindLua(LuaState L) {
            LuaBinding(L).beginClass<phHotEdge>("phHotEdge")
                .addFunction("GetIndex", &getIndex)
                .endClass();
        }
    };

    class phBoundPolygonal : public phBound
    {
    private:
        int VertexCount;
        int PolygonCount;
        int EdgeCount;
        Vector3* VertexPtr;
        phPolygon* PolygonsPtr;
        phHotEdge* HotEdgePtr;
    public:
        int GetVertexCount() const
        {
            return VertexCount;
        }

        int GetPolygonCount() const
        {
            return PolygonCount;
        }

        int GetEdgeCount() const
        {
            return EdgeCount;
        }

        phPolygon* GetPolygon(int index) 
        {
            if(PolygonsPtr == nullptr || index < 0 || index >= PolygonCount)
                return nullptr;
            return &PolygonsPtr[index];
        }

        phHotEdge* GetEdge(int index)
        {
            if (HotEdgePtr == nullptr || index < 0 || index >= EdgeCount)
                return nullptr;
            return &HotEdgePtr[index];
        }

        Vector3* GetVertexSafe(int index)
        {
            if (VertexPtr == nullptr || index < 0 || index >= VertexCount)
                return nullptr;
            return &VertexPtr[index];
        }

        static void BindLua(LuaState L) {
            LuaBinding(L).beginExtendClass<phBoundPolygonal, phBound>("phBoundPolygonal")
                .addFunction("GetPolygon", &GetPolygon)
                .addFunction("GetEdge", &GetEdge)
                .addFunction("GetVertex", &GetVertexSafe)
                .addPropertyReadOnly("NumPolygons", &GetPolygonCount)
                .addPropertyReadOnly("NumEdges", &GetEdgeCount)
                .addPropertyReadOnly("NumVertices", &GetVertexCount)
                .endClass();
        }
    };
    ASSERT_SIZEOF(phBoundPolygonal, 0x64);

    // Lua initialization

}