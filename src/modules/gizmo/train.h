#pragma once
#include <modules\node\node.h>
#include <modules\vehicle\aisubwayaudio.h>
#include <modules\banger\banger.h>

namespace MM2
{
    // Forward declarations
    class gizTrain;
    class gizTrainCar;
    class gizTrainMgr;

    // External declarations
    extern class gizPathspline;
    extern class dgPath;

    // Class definitions
    class gizTrainCar : public dgUnhitMtxBangerInstance
    {
    private:
        gizPathspline* Pathspline;
        int dword_5c;
    public:
        AGE_API bool IsFirstStop()                          { return hook::Thunk<0x578B20>::Call<bool>(this); }
        AGE_API bool IsLastStop()                           { return hook::Thunk<0x578B30>::Call<bool>(this); }

        static void BindLua(LuaState L) {
            LuaBinding(L).beginExtendClass<gizTrainCar, dgUnhitMtxBangerInstance>("gizTrainCar")
                .addPropertyReadOnly("AtFirstStop", &IsFirstStop)
                .addPropertyReadOnly("AtLastStop", &IsLastStop)
                .endClass();
        }
    };

    class gizTrain 
    {
    private:
        int State;
        char Direction;
        float TimeInStation;
        float SpeedFactor;
        gizTrainCar TrainCars[3];
        aiSubwayAudio Audio;
    public:
        AGE_API void Init(LPCSTR modelName, dgPath* path)   { hook::Thunk<0x578C60>::Call<void>(this, modelName, path); }
        AGE_API void Reset()                                { hook::Thunk<0x578C20>::Call<void>(this); }
        AGE_API void Update()                               { hook::Thunk<0x578CD0>::Call<void>(this); }
        AGE_API bool InStation()                            { return hook::Thunk<0x578EF0>::Call<bool>(this); }
        AGE_API void CalcTrainAccel()                       { hook::Thunk<0x578CC0>::Call<void>(this); }

        float GetTimeInStation() const                      { return TimeInStation; }
        float GetSpeedFactor() const                        { return SpeedFactor; }
        Vector3 GetPosition()                               { return TrainCars[1].GetPosition(); }
        aiSubwayAudio* GetAudio()                           { return &Audio; }

        gizTrainCar* GetCar(int num)
        {
            if (num < 0 || num >= 3)
                return nullptr;
            return &TrainCars[num];
        }

        // lua
        static void BindLua(LuaState L) {
            LuaBinding(L).beginClass<gizTrain>("gizTrain")
                .addPropertyReadOnly("InStation", &InStation)
                .addPropertyReadOnly("TimeInStation", &GetTimeInStation)
                .addFunction("GetCar", &GetCar)
                .addFunction("GetPosition", &GetPosition)
                .endClass();
        }
    };

    class gizTrainMgr : public asNode
    {
    private:
        int TrainCount;
        dgPathSet* PathSet;
        gizTrain* Trains;
    public:
        /*
            asNode virtuals
        */
        AGE_API void Reset() override               { hook::Thunk<0x578FF0>::Call<void>(this); }
        AGE_API void Update() override              { hook::Thunk<0x579170>::Call<void>(this); }


        /*
            gizTrainMgr
        */
        AGE_API bool Init(LPCSTR modelName, LPCSTR pathsetName, LPCSTR raceDir)
                                                            { return hook::Thunk<0x579020>::Call<bool>(this, modelName, pathsetName, raceDir); }

        int GetTrainCount() const { return TrainCount; }
        gizTrain* GetTrain(int id)
        {
            if (id < 0 || id >= GetTrainCount())
                return nullptr; 
            return &Trains[id]; 
        }

        // lua
        static void BindLua(LuaState L) {
            LuaBinding(L).beginExtendClass<gizTrainMgr, asNode>("gizTrainMgr")
                .addPropertyReadOnly("NumTrains", &GetTrainCount)
                .addFunction("GetTrain", &GetTrain)
                .endClass();
        }
    };

}