#include "mmMirrorHandler.h"

using namespace MM2;

/*
    mmMirrorHandler
*/

bool mmMirrorHandler::IsDrawing = false;

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
}