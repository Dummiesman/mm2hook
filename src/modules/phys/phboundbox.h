#pragma once
#include "phboundpolygonal.h"

namespace MM2
{
    // Forward declarations
    class phBoundBox;

    // External declarations


    // Class definitions
    class phBoundBox : public phBoundPolygonal
    {
    private:
        Vector3 Size;
        Vector3 Vertices[8];
        phPolygon Polygons[6];
        phMaterial** Materials;
    public:
        phBoundBox() : phBoundPolygonal()
        {
            //scoped_vtable x(this);
            hook::Thunk<0x479ED0>::Call<void>(this);
        }

        ~phBoundBox()
        {
            scoped_vtable x(this);
            hook::Thunk<0x47A160>::Call<void>(this);
        }

        Vector3 getSize()
        {
            return this->Size;
        }

        AGE_API virtual phMaterial const * GetMaterial(int idx) override  
                                                              { return hook::Thunk<0x47A180 >::Call<phMaterial const*>(this); }

        AGE_API void SetSize(Vector3 const & size)            { hook::Thunk<0x47A1A0>::Call<void>(this, &size); }

        AGE_API virtual bool TestProbePoint(phSegment& a1, phIntersectionPoint* a2, float a3) override
                                                              { return hook::Thunk<0x480060>::Call<bool>(this, &a1, a2, a3); }
        AGE_API virtual bool TestAIPoint(phSegment& a1, phIntersectionPoint* a2) override
                                                              { return hook::Thunk<0x480080>::Call<bool>(this, &a1, a2); }
        AGE_API virtual int TestEdge(phSegment& a1, phIntersection* a2, int a3) override
                                                              { return hook::Thunk<0x47F9B0>::Call<int>(&a1, a2, a3); }
        AGE_API virtual bool TestProbe(phSegment& a1, phIntersection* a2, float a3) override
                                                              { return hook::Thunk<0x480010>::Call<bool>(this, &a1, a2, a3); }

        void CreateOffset(Vector3 & offset)                   { hook::Thunk<0x47A240>::Call<void>(this, &offset); }

        static void BindLua(LuaState L) {
            LuaBinding(L).beginExtendClass<phBoundBox, phBoundPolygonal>("phBoundBox")
                .addProperty("Size", &getSize, &SetSize)
                .endClass();
        }
    };
    ASSERT_SIZEOF(phBoundBox, 0x314);

    // Lua initialization

}