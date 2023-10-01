#include "aero.h"

namespace MM2
{
    AGE_API vehAero::vehAero() { scoped_vtable x(this); hook::Thunk<0x4D9320>::Call<void>(this); }

    /*
        asNode virtuals
    */

    AGE_API void vehAero::Update()  { hook::Thunk<0x4D9360>::Call<void>(this); }
    AGE_API void vehAero::FileIO(datParser& parser)  { hook::Thunk<0x4D96E0>::Call<void>(this); }
    AGE_API char* vehAero::GetClassName()  { return hook::Thunk<0x4D9790>::Call<char*>(this); }

    /*
        vehAero
    */
    bool vehAero::GetEnabled() const { return this->m_EnableAero == TRUE; }
    void vehAero::SetEnabled(bool enabled) { this->m_EnableAero = (enabled) ? TRUE : FALSE; }

    float vehAero::GetDown() const { return this->m_Down; }
    void vehAero::SetDown(float down) { this->m_Down = down; }

    float vehAero::GetDrag() const { return this->m_Drag; }
    void vehAero::SetDrag(float drag) { this->m_Drag = drag; }

    Vector3 vehAero::GetAngCDamp() const { return this->m_AngCDamp; }
    void vehAero::SetAngCDamp(Vector3 const& ang) { this->m_AngCDamp = ang; }

    Vector3 vehAero::GetAngVelDamp() const { return this->m_AngVelDamp; }
    void vehAero::SetAngVelDamp(Vector3 const& ang) { this->m_AngVelDamp = ang; }

    Vector3 vehAero::GetAngVel2Damp() const { return this->m_AngVel2Damp; }
    void vehAero::SetAngVel2Damp(Vector3 const& ang) { this->m_AngVel2Damp = ang; }

    void vehAero::BindLua(LuaState L) {
        LuaBinding(L).beginExtendClass<vehAero, asNode>("vehAero")
            //properties
            .addProperty("Enabled", &GetEnabled, &SetEnabled)
            .addProperty("AngCDamp", &GetAngCDamp, &SetAngCDamp)
            .addProperty("AngVelDamp", &GetAngVelDamp, &SetAngVelDamp)
            .addProperty("AngVel2Damp", &GetAngVel2Damp, &SetAngVel2Damp)
            .addProperty("Drag", &GetDrag, &SetDrag)
            .addProperty("Down", &GetDown, &SetDown)

            .endClass();
    }
}