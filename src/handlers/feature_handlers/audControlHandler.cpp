#include "audControlHandler.h"

using namespace MM2;

/*
    audControlHandler
*/

void audControlHandler::Install() {
    InstallCallback("audControl::Create", "Use rewritten create function with better error handling.",
        &audControl::Create, {
            cb::call(0x50D600),
            cb::call(0x50D735),
            cb::call(0x50E530),
            cb::call(0x50EB5F),
        }
    );
}