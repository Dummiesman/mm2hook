#pragma once
#include <mm2_common.h>
#include <modules\node\node.h>

namespace MM2
{
    // Forward declarations
    class audList;

    // External declarations
    extern class audControl;

    // Class definitions
    class AudStreamingMusic : public asNode {
    private:
        audControl* m_Control;
        int m_Stream;
        int dword_20;
        int dword_24;
        int m_PlaylistCount;
        int m_PlaylistSongCounts[10];
        char** m_PlaylistSongs[10];
    private:
        bool luaStreamingMusicPlay(int playlistIndex, int songIndex, bool loop)
        {
            return this->StreamingMusicPlay(playlistIndex, songIndex, loop) == TRUE;
        }
    public:
        static hook::Type<AudStreamingMusic*> Instance; // TODO: Move, this represents AUDMUSICPTR which is set externally

        void StreamingMusicSetVolume(float volume)              { hook::Thunk<0x50EC70>::Call<void>(this, volume); }
        void StreamingMusicStop()                               { hook::Thunk<0x50EC50>::Call<void>(this); }
        int StreamingMusicInit(LPCSTR playlist)                 { return hook::Thunk<0x50E910>::Call<int>(this, playlist); } // returns playlist count
        BOOL StreamingMusicPlay(int playlistIndex, int songIndex, bool loop)
                                                                { return hook::Thunk<0x50EB40>::Call<BOOL>(this, playlistIndex, songIndex, loop); }

        int StreamingMusicGetListCount() const                  { return m_PlaylistCount; }
        int StreamingMusicGetSongCount(int listIndex) const     { return (listIndex >= 0 && listIndex < 10) ? m_PlaylistSongCounts[listIndex] : 0; };

        static void BindLua(LuaState L) {
            LuaBinding(L).beginExtendClass<AudStreamingMusic, asNode>("AudStreamingMusic")
                .addFunction("GetPlaylistCount", &StreamingMusicGetListCount)
                .addFunction("GetSongCount", &StreamingMusicGetSongCount)

                .addFunction("Init", &StreamingMusicInit)
                .addFunction("Stop", &StreamingMusicStop)
                .addFunction("Play", &StreamingMusicPlay)
                .endClass();
        }

    };
    ASSERT_SIZEOF(AudStreamingMusic, 0x7C);
}