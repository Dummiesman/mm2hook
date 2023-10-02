#pragma once
#include "common.h"
#include "patch.h"
#include "mm2.h"

class gfxPipelineHandler {
public:
    static  bool g_bConsoleOpen;

    static bool HandleKeyPress(DWORD vKey);

    static void gfxApplySettings();
    static LRESULT APIENTRY gfxWindowProc(HWND hWnd, UINT uMsg, WPARAM wParam, LPARAM lParam);
    static void gfxWindowMove(bool isOpen);
    static void gfxWindowUpdate(bool isOpen);
    static void SetRes(int width, int height, int cdepth, int zdepth, bool parseArgs);
    static void gfxWindowCreate(LPCSTR lpWindowName);
    static void gfxSetTexReduceSize(int);
    static void BeginFrame();

    static void Install();
};

