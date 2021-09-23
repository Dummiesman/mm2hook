#include "discord-presence.h"
#include "gamesdk/discord.h"
#include <mm2_common.h>

static discord::Core* core{};
static discord::Activity activity{};
bool discordHandler::initialized = false;

static LPCSTR TranslateResult(discord::Result result)
{
    switch (result)
    {
    case discord::Result::ApplicationMismatch:
        return "ApplicationMismatch";
    case discord::Result::CaptureShortcutAlreadyListening:
        return "CaptureShortcutAlreadyListening";
    case discord::Result::Conflict:
        return "Conflict";
    case discord::Result::GetGuildTimeout:
        return "GetGuildTimeout";
    case discord::Result::InsufficientBuffer:
        return "InsufficientBuffer";
    case discord::Result::InternalError:
        return "InternalError";
    case discord::Result::InvalidAccessToken:
        return "InvalidAccessToken";
    case discord::Result::InvalidBase64:
        return "InvalidBase64";
    case discord::Result::InvalidChannel:
        return "InvalidChannel";
    case discord::Result::InvalidCommand:
        return "InvalidCommand";
    case discord::Result::InvalidDataUrl:
        return "InvalidDataUrl";
    case discord::Result::InvalidEntitlement:
        return "InvalidEntitlement";
    case discord::Result::InvalidEvent:
        return "InvalidEvent";
    case discord::Result::InvalidFilename:
        return "InvalidFilename";
    case discord::Result::InvalidFileSize:
        return "InvalidFileSize";
    case discord::Result::InvalidGiftCode:
        return "InvalidGiftCode";
    case discord::Result::InvalidGuild:
        return "InvalidGuild";
    case discord::Result::InvalidInvite:
        return "InvalidInvite";
    case discord::Result::InvalidJoinSecret:
        return "InvalidJoinSecret";
    case discord::Result::InvalidLobbySecret:
        return "InvalidLobbySecret";
    case discord::Result::InvalidOrigin:
        return "InvalidOrigin";
    case discord::Result::InvalidPayload:
        return "InvalidPayload";
    case discord::Result::InvalidPermissions:
        return "InvalidPermissions";
    case discord::Result::InvalidSecret:
        return "InvalidSecret";
    case discord::Result::InvalidVersion:
        return "InvalidVersion";
    case discord::Result::LobbyFull:
        return "LobbyFull";
    case discord::Result::LockFailed:
        return "LockFailed";
    case discord::Result::NoEligibleActivity:
        return "NoEligibleActivity";
    case discord::Result::NotAuthenticated:
        return "NotAuthenticated";
    case discord::Result::NotFetched:
        return "NotFetched";
    case discord::Result::NotFiltered:
        return "NotFiltered";
    case discord::Result::NotFound:
        return "NotFound";
    case discord::Result::NotInstalled:
        return "NotInstalled";
    case discord::Result::NotRunning:
        return "NotRunning";
    case discord::Result::OAuth2Error:
        return "OAuth2Error";
    case discord::Result::Ok:
        return "Ok";
    case discord::Result::PurchaseCanceled:
        return "PurchaseCanceled";
    case discord::Result::PurchaseError:
        return "PurchaseError";
    case discord::Result::RateLimited:
        return "RateLimited";
    case discord::Result::SelectChannelTimeout:
        return "SelectChannelTimeout";
    case discord::Result::SelectVoiceForceRequired:
        return "SelectVoiceForceRequired";
    case discord::Result::ServiceUnavailable:
        return "ServiceUnavailable";
    case discord::Result::TransactionAborted:
        return "TransactionAborted";
    case discord::Result::UnauthorizedForAchievement:
        return "UnauthorizedForAchievement";
    }
    return "UNKNOWN";
}

bool discordHandler::IsInitialized()
{
    return discordHandler::initialized;
}

void discordHandler::Initialize(int64_t appid)
{
    if (IsInitialized())
        return;

    activity.SetType(discord::ActivityType::Playing);

    auto result = discord::Core::Create(appid, DiscordCreateFlags_Default, &core);
    initialized = (result == discord::Result::Ok);

    if (result != discord::Result::Ok) 
    {
        MM2::Errorf("discordHandler::Initialize resulted in %s", TranslateResult(result));
    }
}

void discordHandler::Update()
{
    if (!IsInitialized())
        return;
    core->ActivityManager().UpdateActivity(activity, [](discord::Result result) 
    {
        MM2::Errorf("discordHandler::Update resulted in %s", TranslateResult(result));
    });
}

void discordHandler::RunCallbacks()
{
    if (!IsInitialized())
        return;
    core->RunCallbacks();
}

/*
    Getters and setters
*/
LPCSTR discordHandler::GetState()
{
    return activity.GetState();
}

void discordHandler::SetState(LPCSTR str)
{
    activity.SetState(str);
}

LPCSTR discordHandler::GetDetails()
{
    return activity.GetDetails();
}

void discordHandler::SetDetails(LPCSTR str)
{
    activity.SetDetails(str);
}

LPCSTR discordHandler::GetSmallImage()
{
    return activity.GetAssets().GetSmallImage();
}

void discordHandler::SetSmallImage(LPCSTR str)
{
    activity.GetAssets().SetSmallImage(str);
}

LPCSTR discordHandler::GetLargeImage()
{
    return activity.GetAssets().GetLargeImage();
}

void discordHandler::SetLargeImage(LPCSTR str)
{
    activity.GetAssets().SetLargeImage(str);
}

LPCSTR discordHandler::GetSmallText()
{
    return activity.GetAssets().GetSmallText();
}

void discordHandler::SetSmallText(LPCSTR text)
{
    activity.GetAssets().SetSmallText(text);
}

LPCSTR discordHandler::GetLargeText()
{
    return activity.GetAssets().GetLargeText();
}

void  discordHandler::SetLargeText(LPCSTR text)
{
    activity.GetAssets().SetLargeText(text);
}

int64_t discordHandler::GetStartTimestamp()
{
    return activity.GetTimestamps().GetStart();
}

void discordHandler::SetStartTimestamp(int64_t timestamp)
{
    activity.GetTimestamps().SetStart(timestamp);
}

int64_t discordHandler::GetEndTimestamp()
{
    return activity.GetTimestamps().GetEnd();
}

void discordHandler::SetEndTimestamp(int64_t timestamp)
{
    activity.GetTimestamps().SetEnd(timestamp);
}

static void test()
{
 
}

void discordHandler::BindLua(lua_State* L) {
    LuaBinding(L).beginModule("DiscordRichPresence")
        .addProperty("Initialized", &discordHandler::IsInitialized)

        .addProperty("State", &GetState, &SetState)
        .addProperty("Details", &GetDetails, &SetDetails)
        .addProperty("SmallImage", &GetSmallImage, &SetSmallImage)
        .addProperty("LargeImage", &GetLargeImage, &SetLargeImage)
        .addProperty("SmallText", &GetSmallText, &SetSmallText)
        .addProperty("LargeText", &GetLargeText, &SetLargeText)
        .addProperty("StartTimestamp", &GetStartTimestamp, &SetStartTimestamp)
        .addProperty("EndTimestamp", &GetEndTimestamp, &SetEndTimestamp)

        .addFunction("Initialize", &Initialize)
        .addFunction("Update", &Update)
        .addFunction("RunCallbacks", &RunCallbacks)

        .endModule();
}

void discordHandler::Install() {
    /*TODO: REIMPLEMENT
    bool discordLoaded = (cfgUseRichPresence && LoadDiscordModule());

    if (discordLoaded)
    {
        InstallCallback("asNetwork::CreateSession", "Update the multiplayer status to on when creating the lobby.",
            &DetectHostMPLobby, {
                cb::call(0x4117C5),     //mmInterface::CreateSession
            }
        );
        InstallCallback("asNetwork::JoinSession", "Update the multiplayer status to on when joining the lobby.",
            &DetectJoinMPLobby, {
                cb::call(0x572782),     //asNetwork::JoinSession(int, char *)
            }
        );
        InstallCallback("asNetwork::JoinLobbySession", "Update the multiplayer status to on when joining the lobby session.",
            &DetectJoinMPLobbySession, {
                cb::call(0x409C6F),     //mmInterface::MessageCallback
                cb::call(0x410126),     //mmInterface::InitLobby
            }
        );
        InstallCallback("asNetwork::Disconnect", "Update the multiplayer status to off when exiting the lobby.",
            &DetectDisconnectMPLobby, {
                cb::call(0x40D394),     //mmInterface::Switch
            }
        );
        InstallCallback("asNetwork::CloseSession", "Update the multiplayer status to off when exiting the current game.",
            &DetectDisconnectMPGame, {
                cb::call(0x43B159),     //mmGameMulti::QuitNetwork
            }
        );
        InstallCallback("asNetwork::GetNumPlayers", "Updates the number of players in a lobby",
            &RefreshNumPlayersLobby, {
                cb::call(0x4111B1),     //mmInterface::RefreshPlayers
            }
        );

        InitDiscord();
        g_mm2Info.UpdatePresence(presence);

        LogFile::WriteLine("[discord] Rich Presence initialized successfully.");
    }
    else if (cfgUseRichPresence)
    {
        Warningf("**** Discord Rich Presence was NOT loaded! ****");
    }
    */
}