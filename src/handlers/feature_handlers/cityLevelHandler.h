#pragma once
#include "common.h"
#include "patch.h"
#include "mm2.h"

class cityLevelHandler {
public:
    static int city_numRooms;
    static int city_currentRoom;

    void DrawRooms(const MM2::gfxViewport* viewport, unsigned int p2, LPVOID roomRecs, int numRooms);
    void PostUpdate();
    void SetObjectDetail(int lod);

    static void Install();
};

