#pragma once
#include "common.h"
#include "patch.h"
#include "mm2.h"

class lvlHandler {
public:
    static int lvl_aiRoad;
    static int lvl_aiRoom;
    static int lvl_aiRoomId;

    int CollidePolyToLevel(MM2::phBoundPolygonal* bound,
        int* rooms,
        int roomsCount,
        int unk,
        MM2::phColliderBase* collider,
        MM2::Matrix34* unk2,
        MM2::Matrix34* unk3,
        MM2::lvlIntersection* a9,
        int* unk4,
        char unk5);
    void EnumerateSDL(int p1, MM2::SDLIteratorCB iter, void *context);

    static void SetAIRoad(const MM2::lvlSDL * lvlSDL, int road, bool p3);
    static int GetAIRoom(int room);
    static void InvalidCommand(int cmd, void *attrPtr, void *roadPtr);
    static void Install();
};

