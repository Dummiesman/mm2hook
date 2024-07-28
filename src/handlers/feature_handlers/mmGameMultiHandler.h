#pragma once
#include "common.h"
#include "patch.h"
#include "mm2.h"

class mmGameMultiHandler {
public:
    void GameMessageHook(MM2::NET_RCXHEAD* rcxhead);
    void HandleSystemMessage(DPMSG_GENERIC* msg);
    static void Install();
};

