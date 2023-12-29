#pragma once
#include <mm2_common.h>
#include <lobject.h>

namespace MM2
{
    // Forward declarations
    class DMusicManager;

    // External declarations


    // Class definitions
    class DMusicObject {
    private:
        int WaveBuffer;
        int dword_04;
        int SegmentPtr;
        int dword_0c;
        int Loader;
        int Performance;
        int dword_18;
        int NumMotifs;
        int NumSegments;
        int CurrentSegment;
        int LastSegment;
        int dword_2c;
        void* SegmentWrappers;
        int m_pMotifSegmentWrapper;
    public:
        AGE_API bool IsPlaying() const                { return hook::Thunk<0x5162B0>::Call<bool>(this); }
        AGE_API int PlayMotif(int a2, int a3)         { return hook::Thunk<0x5168A0>::Call<int>(this, a2, a3); }
        AGE_API int PlaySegment(int a2)               { return hook::Thunk<0x516860>::Call<int>(this, a2); }
        AGE_API int SetGrooveLevel(char a2, char a3)  { return hook::Thunk<0x516FF0>::Call<int>(this, a2, a3); }
        AGE_API int SegmentSwitch(int newSeg, int flags1, int flags2)   
                                                      { return hook::Thunk<0x516E90>::Call<int>(this, newSeg, flags1, flags2); }

        static void BindLua(LuaState L);
    };    
}