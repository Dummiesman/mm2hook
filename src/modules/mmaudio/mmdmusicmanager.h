#pragma once
#include <modules\directmusic\dmusicmanager.h>

namespace MM2
{
    // Forward declarations
    class MMDMusicManager;

    class MMDMusicManager : public DMusicManager {
    private:
        char buffer[0x50];
    protected:
        static hook::Field<0x24, int> _finalLapSegmentIndex;
        static hook::Field<0x2C, int> _idleSegmentIndex;
        static hook::Field<0x30, int> _idleChaseSegmentIndex;
        static hook::Field<0x34, int> _pauseSegmentIndex;
        static hook::Field<0x38, int> _returnSegmentIndex;
        static hook::Field<0x3C, int> _chaseSegmentIndex;
        static hook::Field<0x40, int> _resultsSegmentIndex;
        static hook::Field<0x44, int> _startSegmentIndex;
    public:
        static hook::Type<MMDMusicManager*> Instance;

        int GetFinalLapSegmentIndex() const         { return _finalLapSegmentIndex.get(this); }
        int GetIdleSegmentIndex() const             { return _idleSegmentIndex.get(this); }
        int GetIdleChaseSegmentIndex() const        { return _idleChaseSegmentIndex.get(this); }
        int GetPauseSegmentIndex() const            { return _pauseSegmentIndex.get(this); }
        int GetReturnSegmentIndex() const           { return _returnSegmentIndex.get(this); }
        int GetChaseSegmentIndex() const            { return _chaseSegmentIndex.get(this); }
        int GetResultsSegmentIndex() const          { return _resultsSegmentIndex.get(this); }
        int GetStartSegmentIndex() const            { return _startSegmentIndex.get(this); }

        void EchoOn(float delay, float volumeScale)
        {
            hook::Thunk<0x51A2D0>::Call<void>(this, delay, volumeScale);
        }

        void EchoOff()
        {
            hook::Thunk<0x51A3A0>::Call<void>(this);
        }

        static void BindLua(LuaState L) {
            LuaBinding(L).beginExtendClass<MMDMusicManager, DMusicManager>("MMDMusicManager")
            .addPropertyReadOnly("FinalLapSegmentIndex", &GetFinalLapSegmentIndex)
            .addPropertyReadOnly("IdleSegmentIndex", &GetIdleSegmentIndex)
            .addPropertyReadOnly("IdleChaseSegmentIndex", &GetIdleChaseSegmentIndex)
            .addPropertyReadOnly("PauseSegmentIndex", &GetPauseSegmentIndex)
            .addPropertyReadOnly("ReturnSegmentIndex", &GetReturnSegmentIndex)
            .addPropertyReadOnly("ChaseSegmentIndex", &GetChaseSegmentIndex)
            .addPropertyReadOnly("ResultsSegmentIndex", &GetResultsSegmentIndex)
            .addPropertyReadOnly("StartSegmentIndex", &GetStartSegmentIndex)
            .addFunction("EchoOn", &EchoOn)
            .addFunction("EchoOn", &EchoOff)
            .endClass();
        }
    };
}