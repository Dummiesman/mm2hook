// dear imgui: Renderer + Platform Binding for Angel Game Engine

#include <stdint.h>     // uint64_t
#include <cstring>      // memcpy
#include "..\imgui.h"
#include "imgui_impl_age.h"

// AGE
#include <mm2_rgl.h>
#include <mm2_common.h>
#include <mm2_gfx.h>
#include <mm2_data.h>
#include <mm2_input.h>
using namespace MM2;

// Visual Studio warnings
#ifdef _MSC_VER
#pragma warning (disable: 4127) // condition expression is constant
#endif

// Vtx struct since D3D7 can't do Vec2 positions
struct ImDrawVertAGE
{
    Vector3 pos;
    ImU32 col;
    Vector2 uv;
};

// Data
struct ImGui_ImplAGE_Data
{
    gfxViewport* Viewport;
    gfxTexture* Texture;
    double Time;
    char* ClipboardTextData;
    bool ScaledViewport;
    int ViewportSizeX;
    int ViewportSizeY;

    ImGui_ImplAGE_Data() { memset((void*)this, 0, sizeof(*this)); }
};

static ImGui_ImplAGE_Data* ImGui_ImplAGE_GetBackendData() { return ImGui::GetCurrentContext() ? (ImGui_ImplAGE_Data*)ImGui::GetIO().BackendRendererUserData : nullptr; }

static void ImGui_ImplAGE_SetupRenderState(ImDrawData* draw_data)
{
    rglEnableDisable(rglToken::RGL_BLEND, true);
    //rglEnableDisable(rglToken::RGL_DEPTH_TEST, false);
    rglEnableDisable(rglToken::RGL_LIGHTING, false);
    rglEnableDisable(rglToken::RGL_CULL_FACE, false);

    float L = draw_data->DisplayPos.x;
    float R = draw_data->DisplayPos.x + draw_data->DisplaySize.x;
    float T = draw_data->DisplayPos.y;
    float B = draw_data->DisplayPos.y + draw_data->DisplaySize.y;

    ImGui_ImplAGE_Data* bd = ImGui_ImplAGE_GetBackendData();
    bd->Viewport->Ortho(L, R, B, T, -1.0f, +1.0f);
    gfxPipeline::ForceSetViewport(bd->Viewport);
}

// NEW: Input functions
static ImGuiKey ImGui_ImplAGE_ConvertKeyCode(int keyCode)
{
    switch (keyCode) {
        case DIK_ESCAPE: { return ImGuiKey_Escape; }
        case DIK_1: { return ImGuiKey_1; }
        case DIK_2: { return ImGuiKey_2; }
        case DIK_3: { return ImGuiKey_3; }
        case DIK_4: { return ImGuiKey_4; }
        case DIK_5: { return ImGuiKey_5; }
        case DIK_6: { return ImGuiKey_6; }
        case DIK_7: { return ImGuiKey_7; }
        case DIK_8: { return ImGuiKey_8; }
        case DIK_9: { return ImGuiKey_9; }
        case DIK_0: { return ImGuiKey_0; }
        case DIK_MINUS: { return ImGuiKey_Minus; }
        case DIK_EQUALS: { return ImGuiKey_Equal; }
        case DIK_BACK: { return ImGuiKey_Backspace; }
        case DIK_TAB: { return ImGuiKey_Tab; }
        case DIK_Q: { return ImGuiKey_Q; }
        case DIK_W: { return ImGuiKey_W; }
        case DIK_E: { return ImGuiKey_E; }
        case DIK_R: { return ImGuiKey_R; }
        case DIK_T: { return ImGuiKey_T; }
        case DIK_Y: { return ImGuiKey_Y; }
        case DIK_U: { return ImGuiKey_U; }
        case DIK_I: { return ImGuiKey_I; }
        case DIK_O: { return ImGuiKey_O; }
        case DIK_P: { return ImGuiKey_P; }
        case DIK_LBRACKET: { return ImGuiKey_LeftBracket; }
        case DIK_RBRACKET: { return ImGuiKey_RightBracket; }
        case DIK_RETURN: { return ImGuiKey_Enter; }
        case DIK_LCONTROL: { return ImGuiKey_LeftCtrl; }
        case DIK_A: { return ImGuiKey_A; }
        case DIK_S: { return ImGuiKey_S; }
        case DIK_D: { return ImGuiKey_D; }
        case DIK_F: { return ImGuiKey_F; }
        case DIK_G: { return ImGuiKey_G; }
        case DIK_H: { return ImGuiKey_H; }
        case DIK_J: { return ImGuiKey_J; }
        case DIK_K: { return ImGuiKey_K; }
        case DIK_L: { return ImGuiKey_L; }
        case DIK_SEMICOLON: { return ImGuiKey_Semicolon; }
        case DIK_APOSTROPHE: { return ImGuiKey_Apostrophe; }
        case DIK_GRAVE: { return ImGuiKey_GraveAccent; }
        case DIK_LSHIFT: { return ImGuiKey_LeftShift; }
        case DIK_BACKSLASH: { return ImGuiKey_Backslash; }
        case DIK_Z: { return ImGuiKey_Z; }
        case DIK_X: { return ImGuiKey_X; }
        case DIK_C: { return ImGuiKey_C; }
        case DIK_V: { return ImGuiKey_V; }
        case DIK_B: { return ImGuiKey_B; }
        case DIK_N: { return ImGuiKey_N; }
        case DIK_M: { return ImGuiKey_M; }
        case DIK_COMMA: { return ImGuiKey_Comma; }
        case DIK_PERIOD: { return ImGuiKey_Period; }
        case DIK_SLASH: { return ImGuiKey_Slash; }
        case DIK_RSHIFT: { return ImGuiKey_RightShift; }
        case DIK_MULTIPLY: { return ImGuiKey_KeypadMultiply; }
        case DIK_LMENU: { return ImGuiKey_LeftAlt; }
        case DIK_SPACE: { return ImGuiKey_Space; }
        case DIK_CAPITAL: { return ImGuiKey_CapsLock; }
        case DIK_F1: { return ImGuiKey_F1; }
        case DIK_F2: { return ImGuiKey_F2; }
        case DIK_F3: { return ImGuiKey_F3; }
        case DIK_F4: { return ImGuiKey_F4; }
        case DIK_F5: { return ImGuiKey_F5; }
        case DIK_F6: { return ImGuiKey_F6; }
        case DIK_F7: { return ImGuiKey_F7; }
        case DIK_F8: { return ImGuiKey_F8; }
        case DIK_F9: { return ImGuiKey_F9; }
        case DIK_F10: { return ImGuiKey_F10; }
        case DIK_NUMLOCK: { return ImGuiKey_NumLock; }
        case DIK_SCROLL: { return ImGuiKey_ScrollLock; }
        case DIK_NUMPAD7: { return ImGuiKey_Keypad7; }
        case DIK_NUMPAD8: { return ImGuiKey_Keypad8; }
        case DIK_NUMPAD9: { return ImGuiKey_Keypad9; }
        case DIK_SUBTRACT: { return ImGuiKey_KeypadSubtract; }
        case DIK_NUMPAD4: { return ImGuiKey_Keypad4; }
        case DIK_NUMPAD5: { return ImGuiKey_Keypad5; }
        case DIK_NUMPAD6: { return ImGuiKey_Keypad6; }
        case DIK_ADD: { return ImGuiKey_KeypadAdd; }
        case DIK_NUMPAD1: { return ImGuiKey_Keypad1; }
        case DIK_NUMPAD2: { return ImGuiKey_Keypad2; }
        case DIK_NUMPAD3: { return ImGuiKey_Keypad3; }
        case DIK_NUMPAD0: { return ImGuiKey_Keypad0; }
        case DIK_DECIMAL: { return ImGuiKey_Period; }
        case DIK_F11: { return ImGuiKey_F11; }
        case DIK_F12: { return ImGuiKey_F12; }
        case DIK_NUMPADEQUALS: { return ImGuiKey_KeypadEqual; }
        case DIK_NUMPADENTER: { return ImGuiKey_KeyPadEnter; }
        case DIK_RCONTROL: { return ImGuiKey_RightCtrl; }
        case DIK_DIVIDE: { return ImGuiKey_KeypadDivide; }
        case DIK_RMENU: { return ImGuiKey_RightAlt; }
        case DIK_PAUSE: { return ImGuiKey_Pause; }
        case DIK_HOME: { return ImGuiKey_Home; }
        case DIK_PRIOR: { return ImGuiKey_PageUp; }
        case DIK_END: { return ImGuiKey_End; }
        case DIK_NEXT: { return ImGuiKey_PageDown; }
        case DIK_INSERT: { return ImGuiKey_Insert; }
        case DIK_DELETE: { return ImGuiKey_Delete; }
        case DIK_LWIN: { return ImGuiKey_LeftSuper; }
        case DIK_RWIN: { return ImGuiKey_RightSuper; }
    }
    return ImGuiKey_None;
}

static void ImGui_ImplAGE_UpdateMouseInput()
{
    ImGuiIO& io = ImGui::GetIO();
    for (int i = 0; i <= 2; i++)
    {
        if (ioMouse::GetButtonDown(i))
            io.AddMouseButtonEvent(i, true);
        else if (ioMouse::GetButtonUp(i))
            io.AddMouseButtonEvent(i, false);
    }

    int scroll = ioMouse::GetScrollDelta();
    if (scroll != 0)
    {
        io.AddMouseWheelEvent(0.0f, (float)scroll);
    }

    io.AddMousePosEvent(ioMouse::GetX(), ioMouse::GetY());    
}

static void ImGui_ImplAGE_UpdateKeyboardInput()
{
    ImGuiIO& io = ImGui::GetIO();

    for (int i = 0; i < 256; i++)
    {
        ImGuiKey convertedKeycode = ImGui_ImplAGE_ConvertKeyCode(i);
        if (convertedKeycode != ImGuiKey_None)
        {
            if (ioKeyboard::GetKeyDown(i))
                io.AddKeyEvent(convertedKeycode, true);
            else if(ioKeyboard::GetKeyUp(i))
                io.AddKeyEvent(convertedKeycode, false);
        }
    }
}

void ImGui_ImplAGE_UpdateInput()
{
    ImGui_ImplAGE_UpdateMouseInput();
    ImGui_ImplAGE_UpdateKeyboardInput();
}

// Render function.
void ImGui_ImplAGE_RenderDrawData(ImDrawData* draw_data)
{
    // Avoid rendering when minimized
    if (draw_data->DisplaySize.x <= 0.0f || draw_data->DisplaySize.y <= 0.0f) {
        Errorf("ImGui_ImplAGE_RenderDrawData: invalid display size (%f, %f)", draw_data->DisplaySize.x, draw_data->DisplaySize.y);
        return;
    }

    // Get vertex scaling factor
    ImGui_ImplAGE_Data* bd = ImGui_ImplAGE_GetBackendData();
    float vertexScaleX = (bd->ScaledViewport) ? (draw_data->DisplaySize.x / bd->ViewportSizeX) : 1.0f;
    float vertexScaleY = (bd->ScaledViewport) ? (draw_data->DisplaySize.y / bd->ViewportSizeY) : 1.0f;

    // Backup AGE state that will be modified
    auto prevVP = gfxCurrentViewport.get();

    bool lastAlphaEnable = rglIsEnabled(rglToken::RGL_BLEND);
    //bool lastZEnable = rglIsEnabled(rglToken::RGL_DEPTH_TEST);
    bool lastLighting = rglIsEnabled(rglToken::RGL_LIGHTING);
    bool lastCull = rglIsEnabled(rglToken::RGL_CULL_FACE);

    // Setup desired render state
    ImGui_ImplAGE_SetupRenderState(draw_data);

    // Render command lists
    for (int n = 0; n < draw_data->CmdListsCount; n++)
    {
        const ImDrawList* cmd_list = draw_data->CmdLists[n];

        // Convert to D3D7 compatible
        static ImVector<ImDrawVertAGE> vertices;
        vertices.resize(cmd_list->VtxBuffer.Size);
        for (int i = 0; i < cmd_list->VtxBuffer.Size; i++)
        {
            const ImDrawVert* src_v = &cmd_list->VtxBuffer[i];
            ImDrawVertAGE* dst_v = &vertices[i];
            
            dst_v->pos.X = src_v->pos.x * vertexScaleX;
            dst_v->pos.Y = src_v->pos.y * vertexScaleY;
            dst_v->pos.Z = 0.f;

            dst_v->uv.X = src_v->uv.x;
            dst_v->uv.Y = src_v->uv.y;

            unsigned char* cSrc= (unsigned char*)&src_v->col;
            unsigned char* cDst = (unsigned char*)&dst_v->col;
            cDst[0] = cSrc[2];
            cDst[1] = cSrc[1];
            cDst[2] = cSrc[0];
            cDst[3] = cSrc[3];
        }

        const unsigned short* indices = NULL;
        if (sizeof(ImDrawIdx) == 2)
        {
            indices = (const unsigned short*)cmd_list->IdxBuffer.Data;
        }
        else if (sizeof(ImDrawIdx) == 4)
        {
            static ImVector<unsigned short> indices_converted;
            indices_converted.resize(cmd_list->IdxBuffer.Size);
            for (int i = 0; i < cmd_list->IdxBuffer.Size; ++i)
                indices_converted[i] = (unsigned short)cmd_list->IdxBuffer.Data[i];
            indices = indices_converted.Data;
        }
            
        // Render command lists
        for (int cmd_i = 0; cmd_i < cmd_list->CmdBuffer.Size; cmd_i++)
        {
            const ImDrawCmd* pcmd = &cmd_list->CmdBuffer[cmd_i];
            if (pcmd->UserCallback)
            {
                // User callback, registered via ImDrawList::AddCallback()
                // (ImDrawCallback_ResetRenderState is a special callback value used by the user to request the renderer to reset render state.)
                if (pcmd->UserCallback == ImDrawCallback_ResetRenderState)
                    ImGui_ImplAGE_SetupRenderState(draw_data);
                else
                    pcmd->UserCallback(cmd_list, pcmd);
            }
            else
            {
                vglBindTexture((gfxTexture*)pcmd->TextureId); 

                vglBegin(MM2::gfxDrawMode::DRAWMODE_TRIANGLELIST, 0); //This doesn't render anything. But it does flush the render state for us!

                //TEST
                //g_Viewport->SetWindow(pcmd->ClipRect.x, pcmd->ClipRect.y, pcmd->ClipRect.z - pcmd->ClipRect.x, pcmd->ClipRect.w - pcmd->ClipRect.y, 0, 1);

                //
                lpD3DDev->DrawIndexedPrimitive(D3DPRIMITIVETYPE::D3DPT_TRIANGLELIST, 
                                               D3DFVF_XYZ | D3DFVF_DIFFUSE | D3DFVF_TEX1,
                                               (LPVOID)&vertices[pcmd->VtxOffset],
                                               vertices.size(),
                                               (LPWORD)&indices[pcmd->IdxOffset],
                                               pcmd->ElemCount,
                                               0);
            }
        }
    }

    rglEnableDisable(rglToken::RGL_BLEND, lastAlphaEnable);
    //rglEnableDisable(rglToken::RGL_DEPTH_TEST, lastZEnable);
    rglEnableDisable(rglToken::RGL_LIGHTING, lastLighting);
    rglEnableDisable(rglToken::RGL_CULL_FACE, lastCull);

    if (gfxCurrentViewport.get() != prevVP)
        gfxPipeline::ForceSetViewport(prevVP);
}

bool ImGui_ImplAGE_CreateDeviceObjects()
{
    // Build texture atlas
    Displayf("ImGui_ImplAGE_CreateDeviceObjects");

    ImGuiIO& io = ImGui::GetIO();
    unsigned char* pixels;
    int width, height;
    io.Fonts->GetTexDataAsRGBA32(&pixels, &width, &height);

    // Create texture
    auto img = gfxImage::Create(width, height, gfxImage::gfxImageFormat::rif8888, gfxImage::gfxImageFormat::none, 0);
    memcpy(img->pImageData, pixels, sizeof(int) * width * height);

    // Convert software texture to hardware texture.
    auto tex = gfxTexture::Create(img, true);
    
    // Store our identifier
    ImGui_ImplAGE_Data* bd = ImGui_ImplAGE_GetBackendData();
    io.Fonts->TexID = (void*)tex;
    bd->Texture = tex;

    if (tex == nullptr)
    {
        Warningf("gfxTexture::Create failure! (creating texture with %i, %i dimensions)", width, height);
    }
    return true;
}

void ImGui_ImplAGE_InvalidateDeviceObjects()
{
    Displayf("ImGui_ImplAGE_InvalidateDeviceObjects");
    
    ImGuiIO& io = ImGui::GetIO();
    ImGui_ImplAGE_Data* bd = ImGui_ImplAGE_GetBackendData();
    if (bd->Texture)
    {
        delete bd->Texture;
        io.Fonts->SetTexID(0);
    }
}

bool ImGui_ImplAGE_Init(gfxViewport *viewport)
{
    ImGuiIO& io = ImGui::GetIO();
    IM_ASSERT(io.BackendRendererUserData == nullptr && "Already initialized a renderer backend!");

    // Setup back-end capabilities flags
    io.BackendFlags |= ImGuiBackendFlags_HasMouseCursors;       // We can honor GetMouseCursor() values (optional)
    io.BackendRendererName = io.BackendRendererName = "imgui_impl_AGE";
    io.BackendRendererUserData = (void*)IM_NEW(ImGui_ImplAGE_Data)();
    
    // Set data
    ImGui_ImplAGE_Data* bd = ImGui_ImplAGE_GetBackendData();
    bd->Viewport = viewport;

    return true;
}

IMGUI_IMPL_API bool ImGui_ImplAGE_Init(MM2::gfxViewport* viewport, int resX, int resY)
{
    return ImGui_ImplAGE_Init(viewport);

    ImGui_ImplAGE_Data* bd = ImGui_ImplAGE_GetBackendData();
    bd->ScaledViewport = true;
    bd->ViewportSizeX = resX;
    bd->ViewportSizeY = resY;
}

void ImGui_ImplAGE_Shutdown()
{
    ImGui_ImplAGE_Data* bd = ImGui_ImplAGE_GetBackendData();
    ImGuiIO& io = ImGui::GetIO();
    ImGui_ImplAGE_InvalidateDeviceObjects();

    io.BackendRendererUserData = nullptr;
    io.BackendRendererName = nullptr;
    IM_DELETE(bd);
}

// You can read the io.WantCaptureMouse, io.WantCaptureKeyboard flags to tell if dear imgui wants to use your inputs.
// - When io.WantCaptureMouse is true, do not dispatch mouse input data to your main application.
// - When io.WantCaptureKeyboard is true, do not dispatch keyboard input data to your main application.
// Generally you may always pass all inputs to dear imgui, and hide them from your application based on those two flags.
bool ImGui_ImplAGE_ProcessEvent(void* ev)
{
    ImGuiIO& io = ImGui::GetIO();
    return false;
}

static void ImGui_ImplAGE_UpdateMouseCursor()
{
    ImGuiIO& io = ImGui::GetIO();
    if (io.ConfigFlags & ImGuiConfigFlags_NoMouseCursorChange)
        return;
}

void ImGui_ImplAGE_NewFrame()
{
    ImGuiIO& io = ImGui::GetIO();

    // Check if we need to create device objects again
    ImGui_ImplAGE_Data* bd = ImGui_ImplAGE_GetBackendData();
    if (!bd->Texture)
        ImGui_ImplAGE_CreateDeviceObjects();

    // Setup display size (every frame to accommodate for window resizing)
    int w, h;
    w = window_iWidth;
    h = window_iHeight;
    io.DisplaySize = ImVec2((float)w, (float)h);

    // Setup time step
    double current_time = datTimeManager::ElapsedTime;
    io.DeltaTime = bd->Time > 0.0 ? (float)(current_time - bd->Time) : (float)(1.0f / 60.0f);
    bd->Time = current_time;

    ImGui_ImplAGE_UpdateMouseCursor();
}
