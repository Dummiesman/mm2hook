#pragma once
#include <modules\node\node.h>
#include <modules\banger\banger.h>

namespace MM2
{
    // Forward declarations
    class aiTrafficLightSet;
    class aiTrafficLightInstance;

    // External declarations
    extern class aiIntersection;

    // Class definitions

    class aiTrafficLightSet : public asNode {
    private:
        aiTrafficLightInstance** TrafficLights;
        aiIntersection* Intersection;
        __int16 IntersectionID;
        __int16 CurrentLightIdx;
        __int16 LightCount;
        __int16 Type;
        __int16 State;
        float CycleTime;
        float Timer;
    public:
        void SetCycleTime(float time)  { this->CycleTime = time; }
        float GetCycleTime() const     { return this->CycleTime; }
        float GetTimeInCycle() const   { return this->Timer; }
        int GetCurrentLightIdx() const { return this->CurrentLightIdx; }
        int GetLightCount() const      { return this->LightCount; }

        aiTrafficLightInstance* GetLight(int num) 
        {
            if (num < 0 || num >= this->GetLightCount())
                return nullptr;
            return this->TrafficLights[num];
        }

        static void BindLua(LuaState L) {
            LuaBinding(L).beginExtendClass<aiTrafficLightSet, asNode>("aiTrafficLightSet")
                .addPropertyReadOnly("NumLights", &GetLightCount)
                .addPropertyReadOnly("CurrentLightIdx", &GetCurrentLightIdx)
                .addProperty("CycleTime", &GetCycleTime, &SetCycleTime)
                .addPropertyReadOnly("TimeInCycle", &GetTimeInCycle)
                .addFunction("GetLight", &GetLight)
                .endClass();
        }
    };
    ASSERT_SIZEOF(aiTrafficLightSet, 0x34);

    class aiTrafficLightInstance : public dgUnhitYBangerInstance {
    private:
        __int16 State;
    private:
        int InitLua(LPCSTR name, Vector3 const& position, Vector3 const& positionFacing)
        {
            return this->Init(name, position, positionFacing, 0, nullptr);
        }
    public:
        aiTrafficLightInstance::aiTrafficLightInstance()
        {
            // aiTrafficLightInstance ctor got inlined
            // but the inlined constructor implies flag 1 was set in it
            this->SetFlags(this->GetFlags() | 0x1);
        }
        /*
            lvlInstance virtuals
        */
        virtual AGE_API void Draw(int a1)             override     { hook::Thunk<0x53C960>::Call<void>(this, a1); }
        virtual AGE_API void DrawGlow()               override     { hook::Thunk<0x53CA70>::Call<void>(this); }
        virtual AGE_API unsigned int SizeOf(void)     override     { return hook::Thunk<0x53CD50>::Call<unsigned int>(this); }


        /* 
            aitrafficLight virtuals
        */
        virtual AGE_API int Init(LPCSTR name, Vector3 const & position, Vector3 const & positionFacing, int unused_a5, LPCSTR unused_a6)
                                                                   { return hook::Thunk<0x53C6F0>::Call<int>(this, name, &position, &positionFacing, unused_a5, unused_a6); }


        int GetState() const 
        {
            return State;
        }

        void SetState(int state) 
        {
            this->State = state;
        }

        static void BindLua(LuaState L) {
            LuaBinding(L).beginExtendClass<aiTrafficLightInstance, dgUnhitYBangerInstance>("aiTrafficLightInstance")
                .addFactory([]() {
                return new aiTrafficLightInstance();
                })
                .addFunction("Init", &InitLua)
                .addProperty("State", &GetState, &SetState)
                .endClass();
        }
    };
    ASSERT_SIZEOF(aiTrafficLightInstance, 0x40);
}