#pragma once
#include <mm2_common.h>
#include <modules\node\node.h>

namespace MM2
{
    // Forward declarations
    class vehAero;

    // External declarations
    extern class vehCarSim;

    // Class definitions
    class vehAero : public asNode {
    private:
        BOOL m_EnableAero;
        vehCarSim* m_CarSimPtr;
        Vector3 m_AngCDamp; // Ang Const
        Vector3 m_AngVelDamp; // Ang
        Vector3 m_AngVel2Damp;
        float m_Drag;
        float m_Down;
    public:
        AGE_API vehAero();

        /*
            asNode virtuals
        */

        AGE_API void Update() override;
        AGE_API void FileIO(datParser& parser) override;
        AGE_API char* GetClassName() override;

        /*
            vehAero
        */

        bool GetEnabled() const;
        void SetEnabled(bool enabled);

        float GetDown() const;
        void SetDown(float down);

        float GetDrag() const;
        void SetDrag(float drag);

        Vector3 GetAngCDamp() const;
        void SetAngCDamp(Vector3 const& ang);

        Vector3 GetAngVelDamp() const;
        void SetAngVelDamp(Vector3 const& ang);

        Vector3 GetAngVel2Damp() const;
        void SetAngVel2Damp(Vector3 const& ang); 

        static void BindLua(LuaState L);
    };

    ASSERT_SIZEOF(vehAero, 0x4C);
}