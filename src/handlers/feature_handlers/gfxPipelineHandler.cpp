#include "gfxPipelineHandler.h"

#include <imgui\impl\imgui_impl_age.h>
#include <imgui\impl\imgui_impl_win32.h>

using namespace MM2;

/*
    gfxPipelineHandler
*/

bool gfxPipelineHandler::g_bConsoleOpen = false;
bool sirenSoundState = false;

int HeadlightsToggleKey = 76;
int HazardLightsToggleKey = 189;
int LeftTurnSignalToggleKey = 188;
int RightTurnSignalToggleKey = 190;
int SirenLightsToggleKey = 75;
int SirenSoundsToggleKey = 74;

void gfxPipelineHandler::gfxApplySettings(void) {
    gfxInterface *gfxInterface = gfxInterfaces[gfxInterfaceChoice];

    auto deviceType = gfxInterface->DeviceType;

    enableHWTnL = (deviceType == gfxDeviceType::HardwareWithTnL);
    useBlade = (useSoftware = (deviceType == gfxDeviceType::Software));

    useInterface = gfxInterfaceChoice;
}


bool gfxPipelineHandler::HandleKeyPress(DWORD vKey)
{
    if (MM2Lua::IsEnabled())
    {
        // Inform Lua of any changes beforehand
        MM2Lua::OnKeyPress(vKey);
    }

    switch (vKey) {
        // '~'
        case VK_OEM_3:
        // '`'
        case VK_OEM_8:
        {
            // tell the game to open a chat box,
            // and then use a local variable to check if it's open
            mmGameManager *mgr = mmGameManager::Instance;
            auto gamePtr = (mgr != NULL) ? mgr->getGame() : NULL;

            if (gamePtr != NULL)
            {
                auto popup = gamePtr->getPopup();

                if (popup != NULL) {
                    // don't try opening it again if it's already open
                    if (popup->IsEnabled() && g_bConsoleOpen)
                        return true;

                    popup->ProcessChat();
                    g_bConsoleOpen = true;
                }
            }
        } return true;

        case VK_F8: {
            if (HookConfig::Read()) {
                LogFile::WriteLine("Configuration settings reloaded successfully.");
            }
        } return true;
    }

    if (vKey == HeadlightsToggleKey) {
        mmGameManager *mgr = mmGameManager::Instance;
        auto gamePtr = (mgr != NULL) ? mgr->getGame() : NULL;

        if (gamePtr != NULL)
        {
            auto popup = gamePtr->getPopup();

            if (popup != NULL) {
                if (!popup->IsEnabled()) {
                    // toggle vehicle headlights
                    vehCarModel::HeadlightsState = !vehCarModel::HeadlightsState;
                }
            }
        }
        return true;
    }

    if (vKey == HazardLightsToggleKey) {
        mmGameManager *mgr = mmGameManager::Instance;
        auto gamePtr = (mgr != NULL) ? mgr->getGame() : NULL;
        
        if (gamePtr != NULL)
        {
            auto popup = gamePtr->getPopup();

            if (popup != NULL) {
                if (!popup->IsEnabled()) {
                    // toggle hazard lights
                    vehCarModel::HazardLightsState = !vehCarModel::HazardLightsState;
                    vehCarModel::LeftSignalLightState = false;
                    vehCarModel::RightSignalLightState = false;
                }
            }
        }
        return true;
    }

    if (vKey == LeftTurnSignalToggleKey) {
        mmGameManager *mgr = mmGameManager::Instance;
        auto gamePtr = (mgr != NULL) ? mgr->getGame() : NULL;

        if (gamePtr != NULL)
        {
            auto popup = gamePtr->getPopup();

            if (popup != NULL) {
                if (!popup->IsEnabled()) {
                    // toggle left signal
                    vehCarModel::LeftSignalLightState = !vehCarModel::LeftSignalLightState;
                    vehCarModel::HazardLightsState = false;
                    vehCarModel::RightSignalLightState = false;
                }
            }
        }
        return true;
    }

    if (vKey == RightTurnSignalToggleKey) {
        mmGameManager *mgr = mmGameManager::Instance;
        auto gamePtr = (mgr != NULL) ? mgr->getGame() : NULL;

        if (gamePtr != NULL)
        {
            auto popup = gamePtr->getPopup();

            if (popup != NULL) {
                if (!popup->IsEnabled()) {
                    // toggle right signal
                    vehCarModel::RightSignalLightState = !vehCarModel::RightSignalLightState;
                    vehCarModel::HazardLightsState = false;
                    vehCarModel::LeftSignalLightState = false;
                }
            }
        }
        return true;
    }

    if (vKey == SirenLightsToggleKey) {
        mmGameManager *mgr = mmGameManager::Instance;
        auto gamePtr = (mgr != NULL) ? mgr->getGame() : NULL;

        if (gamePtr != NULL)
        {
            auto popup = gamePtr->getPopup();
            auto siren = gamePtr->getPlayer()->getCar()->getSiren();
            char *vehName = gamePtr->getPlayer()->getCar()->getCarDamage()->GetName();
            int flagsId = VehicleListPtr->GetVehicleInfo(vehName)->GetFlags();

            if (popup != NULL) {
                if (!popup->IsEnabled()) {
                    // toggle siren lights
                    if (siren != nullptr && siren->HasLights || flagsId == 8) {
                        siren->Active = !siren->Active;
                    }
                }
            }
        }
        return true;
    }

    if (vKey == SirenSoundsToggleKey) {
        mmGameManager *mgr = mmGameManager::Instance;
        auto gamePtr = (mgr != NULL) ? mgr->getGame() : NULL;

        if (gamePtr != NULL)
        {
            auto popup = gamePtr->getPopup();
            auto audio = gamePtr->getPlayer()->getCar()->getAudio();
            auto policeAudio = audio->GetPoliceCarAudioPtr();

            if (popup != NULL) {
                if (!popup->IsEnabled()) {
                    if (policeAudio != nullptr) {
                        // toggle siren sounds
                        sirenSoundState = !sirenSoundState;

                        if (sirenSoundState)
                            policeAudio->StartSiren(0);
                        else
                            policeAudio->StopSiren();
                    }
                }
            }
        }
        return true;
    }

    return false;
}

LRESULT APIENTRY gfxPipelineHandler::gfxWindowProc(HWND hWnd, UINT uMsg, WPARAM wParam, LPARAM lParam) {
    //IMGUI
    extern IMGUI_IMPL_API LRESULT ImGui_ImplWin32_WndProcHandler(HWND hWnd, UINT msg, WPARAM wParam, LPARAM lParam);
    ImGui_ImplWin32_WndProcHandler(hWnd, uMsg, wParam, lParam);

    //
    switch (uMsg)
    {
    case WM_ACTIVATEAPP:
    {
        if ((wParam == FALSE) && datArgParser::Get("nopause"))
            return 0;
    } break;

    case WM_KEYUP:
    case WM_SYSKEYUP:
    {
        if (HandleKeyPress(wParam))
            return 0;
    } break;
    }

    return $gfxPipeline_gfxWindowProc(hWnd, uMsg, wParam, lParam);
}

void gfxPipelineHandler::gfxWindowMove(bool isOpen) {
    HDC hDC = GetDC(NULL);
    int screenWidth = GetDeviceCaps(hDC, HORZRES);
    int screenHeight = GetDeviceCaps(hDC, VERTRES);
    ReleaseDC(0, hDC);

    window_X = (screenWidth - window_iWidth) / 2;
    window_Y = (screenHeight - window_iHeight) / 2;

    // window may or may not be initialized yet
    if (isOpen) {
        MoveWindow(
            hWndMain,
            window_X,
            window_Y,
            window_iWidth,
            window_iHeight,
            0);
    }
}

void gfxPipelineHandler::gfxWindowUpdate(bool isOpen) {
    RECT rect;
    GetClientRect(hWndMain, &rect);

    MoveWindow(
        hWndMain,
        window_X,
        window_Y,
        (2 * window_iWidth - rect.right),
        (2 * window_iHeight - rect.bottom),
        isOpen /* repaint if open */);
}

void gfxPipelineHandler::SetRes(int width, int height, int cdepth, int zdepth, bool parseArgs) {
    if (datArgParser::Get("ref")) {
        useSoftware = 1;
        useReference = 1;
    }
    else if (datArgParser::Get("blade") || datArgParser::Get("bladed")) {
        useSoftware = 1;
        useBlade = 1;
    }
    else if (datArgParser::Get("swage")) {
        useSoftware = 1;
        useAgeSoftware = 1;
    }
    else if (datArgParser::Get("sw")) {
        useSoftware = 1;
    }

    if (datArgParser::Get("sysmem")) {
        useSysMem = 1;
    }
    if (datArgParser::Get("triple")) {
        tripleBuffer = 1;
    }

    if (datArgParser::Get("nomultitexture") || datArgParser::Get("nomt")) {
        useMultiTexture = 0;
    }
    if (datArgParser::Get("novblank") || datArgParser::Get("novsync")) {
        novblank = 1;
    }
    if (datArgParser::Get("nohwtnl")) {
        enableHWTnL = 0;
    }

    if (datArgParser::Get("primary")) {
        useInterface = 0;
    }
    else {
        datArgParser::Get("display", 0, &useInterface);
    }
    if (datArgParser::Get("single")) {
        pageFlip = 0;
    }

    if (datArgParser::Get("window") || datArgParser::Get("windowed")) {
        inWindow = 1;
    }
    else if (datArgParser::Get("fs") || datArgParser::Get("fullscreen")) {
        inWindow = 0;
    }

    int bitDepth = 0;
    if (datArgParser::Get("bpp", 0, &bitDepth) || datArgParser::Get("bitdepth", 0, &bitDepth)) {
        cdepth = bitDepth;
        zdepth = bitDepth;
    }
    else {
        datArgParser::Get("cdepth", 0, &cdepth);
        datArgParser::Get("zdepth", 0, &zdepth);
    }

    // We don't want to set the width/height if we are in a menu, it just fucks it up
    if (MMSTATE->SplashScreen != 0) {
        if (datArgParser::Get("max")) {
            HDC hDC = GetDC(NULL);
            width = GetDeviceCaps(hDC, HORZRES);
            height = GetDeviceCaps(hDC, VERTRES);
            ReleaseDC(0, hDC);
        }
        else {
            datArgParser::Get("width", 0, &width);
            datArgParser::Get("height", 0, &height);
        }

        // datArgParser::Get("width",  0, &width);
        // datArgParser::Get("height", 0, &height);
    }

    useSysMem = useSoftware;

    window_iWidth = width;
    window_iHeight = height;

    window_fWidth = float(width);
    window_fHeight = float(height);

    window_ColorDepth = cdepth;
    window_ZDepth = zdepth;

    gfxTexture::sm_Allow32 = (cdepth == 32);

    LogFile::Format("[gfxPipeline::SetRes]: 32-bit textures are%s allowed.\n", (gfxTexture::sm_Allow32) ? "" : "n't");
    LogFile::Format("[gfxPipeline::SetRes]: %dx%dx%dx%d\n", width, height, cdepth, zdepth);

    if (lpDD)
    {
        if (inWindow)
        {
            gfxWindowMove(true);
            gfxWindowUpdate(true);
        }
        else {
            DDSURFACEDESC2 ddSurfaceDesc;

            ddSurfaceDesc.dwSize = 0x7C;

            if ((lpDD->GetDisplayMode(&ddSurfaceDesc) != DD_OK)
                || (ddSurfaceDesc.dwWidth != window_iWidth)
                || (ddSurfaceDesc.dwHeight != window_iHeight))
            {
                if (lpDD->SetDisplayMode(
                    window_iWidth,
                    window_iHeight,
                    window_ColorDepth,
                    0,
                    0) != DD_OK) {
                    LogFile::WriteLine("[gfxPipeline::SetRes]: SHIT! Failed to set the display mode!");
                }
            }
        }
    }

    ioMouse::InvWidth = (1.0f / window_fWidth);
    ioMouse::InvHeight = (1.0f / window_fHeight);
}

void gfxPipelineHandler::gfxWindowCreate(LPCSTR lpWindowName) {
    if (hWndMain)
        return;

    if (lpWindowTitle)
        lpWindowName = lpWindowTitle;

    if (!ATOM_Class)
    {
        WNDCLASSA wc = {
            CS_HREDRAW | CS_VREDRAW,    /* style */
            gfxWindowProc,              /* lpfnWndProc */
            0,                          /* cbClsExtra */
            0,                          /* cbWndExtra */
            0,                          /* hInstance */
            LoadIconA(GetModuleHandleA(NULL), IconID ? IconID : IDI_APPLICATION),
            /* hIcon */
            LoadCursorA(0, IDC_ARROW),  /* hCursor */
            CreateSolidBrush(NULL),     /* hbrBackground */
            NULL,                       /* lpszMenuName */
            "gfxWindow",                /* lpszClassName */
        };

        ATOM_Class = RegisterClassA(&wc);
    }

    DWORD dwStyle = WS_POPUP;

    if (inWindow)
    {
        if (hWndParent)
        {
            dwStyle = WS_CHILD;
        }
        else if (hasBorder = !(datArgParser::Get("noborder") || datArgParser::Get("borderless")))
        {
            dwStyle |= (WS_CAPTION | WS_SYSMENU | WS_MINIMIZEBOX);
        }
    }
    else
    {
        dwStyle |= WS_SYSMENU;
    }

    // update the position
    gfxWindowMove(false);

    HWND hWND = CreateWindowExA(
        WS_EX_APPWINDOW,
        "gfxWindow",
        lpWindowName,
        dwStyle,
        window_X,
        window_Y,
        640,
        480,
        hWndParent,
        0,
        0,
        0);

    hWndMain = hWND;

    if (inWindow)
        gfxWindowUpdate(false);

    SetCursor(NULL);
    ShowCursor(FALSE);

    ShowWindow(hWND, TRUE);
    UpdateWindow(hWND);
    SetFocus(hWND);
}

void gfxPipelineHandler::gfxSetTexReduceSize(int) {
    static const int gfxReduceSizes[4] = {
        64,     // Low
        128,    // Medium
        256,    // High
        0,      // Very High (unlimited)
    };

    gfxTexReduceSize = gfxReduceSizes[*gfxTexQuality];
}

void gfxPipelineHandler::Install() {
    InstallPatch("Enables pointer in windowed mode.", { 0x90, 0x90 }, {
        0x4F136E,
    });

    InstallCallback("gfxPipeline::SetRes", "Custom implementation allowing for more control of the window.",
        &SetRes, {
            cb::jmp(0x4A8CE0),
        }
    );

    InstallCallback("gfxPipeline::gfxWindowCreate", "Custom implementation allowing for more control of the window.",
        &gfxWindowCreate, {
            cb::jmp(0x4A8A90),
        }
    );

    InstallCallback("gfxApplySettings", "Custom implementation allowing for more control of the graphical settings.",
        &gfxApplySettings, {
            cb::jmp(0x4AC870),
        }
    );

    InstallCallback("gfxLoadVideoDatabase", "Disables 'badvideo.txt' file.",
        &ReturnFalse, {
            cb::call(0x4AC4F9),
        }
    );

    ConfigValue<int> cfgHeadlightsToggleKey("HeadlightsToggleKey", 76);
    ConfigValue<int> cfgHazardLightsToggleKey("HazardLightsToggleKey", 189);
    ConfigValue<int> cfgLeftTurnSignalToggleKey("LeftTurnSignalToggleKey", 188);
    ConfigValue<int> cfgRightTurnSignalToggleKey("RightTurnSignalToggleKey", 190);
    ConfigValue<int> cfgSirenLightsToggleKey("SirenLightsToggleKey", 75);
    ConfigValue<int> cfgSirenSoundsToggleKey("SirenSoundsToggleKey", 74);

    HeadlightsToggleKey = cfgHeadlightsToggleKey.Get();
    HazardLightsToggleKey = cfgHazardLightsToggleKey.Get();
    LeftTurnSignalToggleKey = cfgLeftTurnSignalToggleKey.Get();
    RightTurnSignalToggleKey = cfgRightTurnSignalToggleKey.Get();
    SirenLightsToggleKey = cfgSirenLightsToggleKey.Get();
    SirenSoundsToggleKey = cfgSirenSoundsToggleKey.Get();
}

