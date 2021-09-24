#pragma once
#include <mm2_common.h>
#include <dplay.h>

namespace MM2
{
    // Forward declarations
    struct LuaSessionInfo;

    // External declarations


    // Class definitions
    struct LuaSessionInfo
    {
    private:
        bool IsHost;
        int PlayerCount;
        int MaxPlayerCount;
        char SessionName[256];
        char SessionPassword[256];
    public:
        LuaSessionInfo(IDirectPlay4A* DPLAY, bool isHost)
        {
            bool populated = false;
            if (DPLAY != nullptr) 
            {
                LPVOID data = nullptr;
                DWORD dataSize = 0;
                auto result = DPLAY->GetSessionDesc(NULL, &dataSize);

                if (result == DPERR_BUFFERTOOSMALL)
                {
                    data = malloc(dataSize);
                    result = DPLAY->GetSessionDesc(data, &dataSize);    
                }
                if (result == S_OK) 
                {
                    auto desc = (DPSESSIONDESC2*)data;
                    this->PlayerCount = desc->dwCurrentPlayers;
                    this->MaxPlayerCount = desc->dwMaxPlayers;
                    strncpy(SessionName, desc->lpszSessionNameA, sizeof(SessionName));
                    strncpy(SessionPassword, desc->lpszPasswordA, sizeof(SessionPassword));
                    populated = true;
                }
                if (data != nullptr)
                    free(data);
            }
            
            if(!populated)
            {
                this->PlayerCount = 0;
                this->MaxPlayerCount = 0;
            }

            this->IsHost = isHost;
        }

        inline int getPlayerCount()
        {
            return this->PlayerCount;
        }

        inline int getMaxPlayerCount()
        {
            return this->MaxPlayerCount;
        }

        inline LPCSTR getSessionName()
        {
            return (LPCSTR)&this->SessionName;
        }

        inline LPCSTR getSessionPassword()
        {
            return (LPCSTR)&this->SessionPassword;
        }

        inline bool getIsHost()
        {
            return this->IsHost;
        }

        inline bool isPassworded()
        {
            return SessionPassword[0] != 0x00;
        }

        static void BindLua(LuaState L) {
            LuaBinding(L).beginClass<LuaSessionInfo>("LuaSessionInfo")
                .addPropertyReadOnly("Name", &getSessionName)
                .addPropertyReadOnly("Password", &getSessionPassword)
                .addPropertyReadOnly("MaxPlayers", &getMaxPlayerCount)
                .addPropertyReadOnly("NumPlayers", &getPlayerCount)
                .addPropertyReadOnly("IsHost", &getIsHost)
                .addPropertyReadOnly("IsPassworded", &isPassworded)
                .endClass();
        }
    };
}