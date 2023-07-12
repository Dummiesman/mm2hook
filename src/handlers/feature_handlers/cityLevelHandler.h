#pragma once
#include "common.h"
#include "patch.h"
#include "mm2.h"

class cityLevelHandler {
public:
    static int city_numRooms;
    static int city_currentRoom;

    static void UpdateLighting();

    static void LightmapDelete(void* data);

    void DrawRooms(const MM2::gfxViewport* viewport, unsigned int p2, LPVOID roomRecs, int numRooms);
    void SetObjectDetail(int lod);

    MM2::gfxTexture* GetEnvMap(int roomNumber, MM2::Vector3 const &, float& intensity);

    static void Install();
};

