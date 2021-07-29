#pragma once
#include "common.h"
#include "patch.h"
#include "mm2.h"

class lvlHandler {
public:
    static int lvl_aiRoad;
    static int lvl_aiRoom;
    static int lvl_aiRoomId;


    void EnumerateSDL(int p1, MM2::SDLIteratorCB iter, void *context);

    static void SetAIRoad(const MM2::lvlSDL * lvlSDL, int road, bool p3);
    static int GetAIRoom(int room);
    static void InvalidCommand(int cmd, void *attrPtr, void *roadPtr);
    static void Install();
};

