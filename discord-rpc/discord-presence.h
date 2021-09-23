#pragma once
#include "common.h"
#include "patch.h"

class discordHandler : handler_t {
private:
    static bool initialized;
public:
    static void Initialize(int64_t appid);
    static bool IsInitialized();
    static void Update();
    static void RunCallbacks();

    static LPCSTR GetSmallText();
    static LPCSTR GetLargeText();
    static LPCSTR GetSmallImage();
    static LPCSTR GetLargeImage();
    static LPCSTR GetDetails();
    static LPCSTR GetState();
    static int64_t GetStartTimestamp();
    static int64_t GetEndTimestamp();

    static void SetSmallText(LPCSTR str);
    static void SetLargeText(LPCSTR str);
    static void SetSmallImage(LPCSTR str);
    static void SetLargeImage(LPCSTR str);
    static void SetDetails(LPCSTR str);
    static void SetState(LPCSTR str);
    static void SetStartTimestamp(int64_t timestamp);
    static void SetEndTimestamp(int64_t timestamp);

    static int GetMaxPartySize();
    static int GetCurrentPartySize();
    static void SetMaxPartySize(int size);
    static void SetCurrentPartySize(int size);

    static void BindLua(lua_State* L);
    static void Install(void);
};