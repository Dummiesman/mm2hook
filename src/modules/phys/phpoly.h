#pragma once
#include <mm2_common.h>

namespace MM2
{
    // Forward declarations
    struct phPolygon;

    // External declarations


    // Class definitions
    struct phPolygon
    {
        Vector3 Normal;
        float Radius;
        Vector4 EdgeNormalCross[4];
        ushort Indices[4];
        ushort Indices2[4];

        /*
            Lua Helpers
        */
        short GetIndex(int i) const
        {
            if (i < 0 || i >= 4)
                return -1;
            return (short)this->Indices[i];
        }

        void SetIndex(int i, ushort index)
        {
            if (i >= 0 && i < 4)
                this->Indices[i] = index;
        }

        char GetMaterialIndex() const
        {
            return *(char*)&Radius;
        }

        void SetMaterialIndex(char index) 
        {
            *(char*)&Radius = index;
        }

        bool IsQuad() const
        {
            return this->Indices[3] != 0;
        }

        static void BindLua(LuaState L) {
            LuaBinding(L).beginClass<phPolygon>("phPolygon")
                .addPropertyReadOnly("IsQuad", &IsQuad)
                .addVariable("Radius", &phPolygon::Radius)
                .addVariable("Normal", &phPolygon::Normal)
                .addProperty("MaterialIndex", &GetMaterialIndex, &SetMaterialIndex)
                .addFunction("GetIndex", &GetIndex)
                .addFunction("SetIndex", &SetIndex)
                .endClass();
        }
    };


    // Lua initialization

}