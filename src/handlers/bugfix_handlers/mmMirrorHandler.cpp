#include "mmMirrorHandler.h"

using namespace MM2;

/*
    mmMirrorHandler
*/

bool mmMirrorHandler::IsDrawing = false;

void mmMirrorHandler::Clear(DWORD flags, int color, D3DVALUE dvZ, DWORD dwStencil)
{
    // Instead of clearing gfxPipeline with pure black, use the asCullManager color
    color = asCullManager::Instance->GetClearColor().PackColorBGRA();
    gfxPipeline::Clear(flags, color, dvZ, dwStencil);
}

void mmMirrorHandler::Cull() {
    mmMirrorHandler::IsDrawing = true;
    hook::Thunk<0x42B8C0>::Call<void>(this); // call original
    mmMirrorHandler::IsDrawing = false;
}

void mmMirrorHandler::Install() {
    InstallVTableHook("mmMirror::Cull",
        &Cull, {
            0x5B0B80 ,
        }
    );

    InstallCallback("mmMirror::Cull", "Fixes bad clear color.",
        &Clear, {
            cb::call(0x42B974),
        }
    );
}