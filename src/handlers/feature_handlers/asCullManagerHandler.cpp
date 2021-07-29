#include "asCullManagerHandler.h"

using namespace MM2;

static ConfigValue<int> cfgMaxCullables("MaxCullables", 1024);
static ConfigValue<int> cfgMaxCullables2D("MaxCullables2D", 256);

/*
    asCullManagerHandler
*/

void asCullManagerHandler::Init(int maxCullables, int maxCullables2D) {
    maxCullables = cfgMaxCullables;
    maxCullables2D = cfgMaxCullables2D;

    LogFile::Format("[asCullManager::Init]: Max Cullables = %d, %d\n", maxCullables, maxCullables2D);

    hook::Thunk<0x4A1290>::Call<void>(this, maxCullables, maxCullables2D);
}

void asCullManagerHandler::Install() {
    InstallCallback("asCullManager::Init", "Increases max cullables.",
        &Init, {
            cb::call(0x401D5C),
        }
    );
}

