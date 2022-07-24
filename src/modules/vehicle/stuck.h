#pragma once
#include <mm2_common.h>
#include <modules\node\node.h>

namespace MM2
{
    // Forward declarations
    class vehStuck;

    // External declarations
    extern class vehCarSim;
    extern class phInertialCS;

    // Class definitions
    class vehStuck : public asNode
    {
    private:
        int m_State;
        float m_StuckTime;
        Vector3 m_LastImpactPos;
        float m_TimeThresh;
        float m_PosThresh;
        float m_MoveThresh;
        float m_PosThreshSqr;
        float m_MoveThreshSqr;
        float m_Turn;
        float m_Rotation;
        float m_Translation;
        phInertialCS *m_InertialCSPtr;
        vehCarSim *m_CarSimPtr;
    public:
        AGE_API vehStuck()                              { scoped_vtable x(this); hook::Thunk<0x4D5FB0>::Call<void>(this); }

        AGE_API void Init(vehCarSim *carSim, const char *name) 
                                                        { hook::Thunk<0x4D6090>::Call<void>(this, carSim, name); }
        
        AGE_API static void StuckCB(vehStuck* stuck)    { hook::StaticThunk<0x4D6040>::Call<void>(stuck); }
        void ComputeConstants()                         { vehStuck::StuckCB(this); }

        /*
            asNode virtuals
        */
        AGE_API void Reset() override                   { hook::Thunk<0x4D6060>::Call<void>(this); }
        AGE_API void Update() override                  { hook::Thunk<0x4D6140>::Call<void>(this); }
        AGE_API void FileIO(datParser& parser) override { hook::Thunk<0x4D6510>::Call<void>(this); }
        AGE_API const char* GetDirName() override       { return hook::Thunk<0x4D6080>::Call<const char*>(this); }
        AGE_API char * GetClassName() override          { return hook::Thunk<0x4D65F0>::Call<char*>(this); }

        /*
            vehStuck
        */
        float GetStuckTime() const {
            return this->m_StuckTime;
        }

        void SetStuckTime(float time) {
            this->m_StuckTime = time;
        }

        float GetTurn() const {
            return this->m_Turn;
        }

        void SetTurn(float turn) {
            this->m_Turn = turn;
        }

        float GetRotation() const {
            return this->m_Rotation;
        }

        void SetRotation(float rotation) {
            this->m_Rotation = rotation;
        }

        float GetTranslation() const {
            return this->m_Translation;
        }

        void SetTranslation(float translation) {
            this->m_Translation = translation;
        }

        float GetTimeThresh() const {
            return this->m_TimeThresh;
        }

        void SetTimeThresh(float value) {
            this->m_TimeThresh = value;
        }

        float GetPosThresh() const {
            return this->m_PosThresh;
        }

        void SetPosThresh(float value) {
            this->m_PosThresh = value;
        }

        float GetMoveThresh() const {
            return this->m_MoveThresh;
        }

        void SetMoveThresh(float value) {
            this->m_MoveThresh = value;
        }

        static void BindLua(LuaState L) {
            LuaBinding(L).beginExtendClass<vehStuck, asNode>("vehStuck")
                //properties
                .addProperty("Turn", &GetTurn, &SetTurn)
                .addProperty("Rotation", &GetRotation, &SetRotation)
                .addProperty("Translation", &GetTranslation, &SetTranslation)
                .addProperty("TimeThresh", &GetTimeThresh, &SetTimeThresh)
                .addProperty("PosThresh", &GetPosThresh, &SetPosThresh)
                .addProperty("MoveThresh", &GetMoveThresh, &SetMoveThresh)

                .addFunction("Init", &Init)
                .addFunction("ComputeConstants", &ComputeConstants)

                .endClass();
        }
    };

    ASSERT_SIZEOF(vehStuck, 0x54);
}