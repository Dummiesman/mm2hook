#include "mmGameMultiHandler.h"
#include <mm2_lua.h>

using namespace MM2;

/*
    mmGameMultiHandler
*/


void mmGameMultiHandler::GameMessageHook(MM2::NET_RCXHEAD* rcxhead)
{
    if (rcxhead->packetType > asNetwork::CUSTOM_PACKET_ID_START)
    {
        auto data = std::string(reinterpret_cast<const char*>(rcxhead->pData), rcxhead->length);
        MM2Lua::OnNetworkMessage(rcxhead->packetType - asNetwork::CUSTOM_PACKET_ID_START, data);
        return;
    }
    if (rcxhead->packetType == 0x1FA)
    {
        // player is "set and  ready"
        MM2Lua::OnPlayerFinishedLoading(rcxhead->srcPlayerId);
    }
    reinterpret_cast<datCallback*>(this)->Call(rcxhead);
}

void mmGameMultiHandler::HandleSystemMessage(DPMSG_GENERIC* msg)
{
    // forward leave messages to lua
    if (msg->dwType == DPSYS_DESTROYPLAYERORGROUP)
    {
        auto destroyMsg = (DPMSG_DESTROYPLAYERORGROUP*)msg;
        if (destroyMsg->dwPlayerType == DPPLAYERTYPE_PLAYER)
        {
            MM2Lua::OnPlayerLeaveGame(destroyMsg->dpId);
        }
    }

    hook::Thunk<0x571D90>::Call<void>(this, msg); // call original

    // forward join messages to Lua
    if (msg->dwType == DPSYS_CREATEPLAYERORGROUP)
    {
        auto createMsg = (DPMSG_CREATEPLAYERORGROUP*)msg;
        if (createMsg->dwPlayerType == DPPLAYERTYPE_PLAYER)
        {
            MM2Lua::OnPlayerJoinedGame(createMsg->dpId);
        }
    }
}

void mmGameMultiHandler::Install() 
{
    InstallCallback("asNetwork::GameMessage", "Hook game message handler for Lua.",
        &GameMessageHook, {
            cb::call(0x571D83),
        }
    );

    InstallCallback("asNetwork::SystemMessage", "Hook system message handler for Lua.",
        &HandleSystemMessage, {
            cb::call(0x571C20),
            cb::call(0x571D21),
        }
    );
}

