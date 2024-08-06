#pragma once
#include "common.h"
#include "patch.h"
#include "mm2.h"

class BridgeFerryHandler {
public:
    void InitBridge(const char* name, const MM2::Matrix34& matrix);
    void Cull();
    int IsVisibleFerry(MM2::gfxViewport const& viewport);
    void DrawFerry(int lod);    
    void DrawBridge(int lod);

    static void Install();
};